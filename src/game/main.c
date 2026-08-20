/*
 * X-Wing vs TIE Fighter Static Recompilation - Entry Point
 *
 * Memory layout, register model, VEH crash handler, dispatch lookups.
 * Deliberately minimal: no per-function overrides yet — those get added
 * as runtime bringup finds them (mirrors how XWA's main.c grew).
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "recomp/recomp_types.h"
#include "recomp/gen/recomp_funcs.h"

/* ============================================================
 * Global Register Model
 * ============================================================ */

uint32_t g_eax = 0, g_ecx = 0, g_edx = 0, g_esp = 0;
uint32_t g_ebx = 0, g_esi = 0, g_edi = 0;
uint16_t g_seg_cs = 0, g_seg_ds = 0, g_seg_es = 0;
uint16_t g_seg_fs = 0, g_seg_gs = 0, g_seg_ss = 0;

ptrdiff_t g_mem_base = 0;          /* 0 = identity mapping */
uint32_t g_fs_seg[256] = {0};      /* simulated TEB */

uint32_t g_icall_trace[ICALL_TRACE_SIZE] = {0};
uint32_t g_icall_trace_idx = 0;
uint32_t g_icall_count = 0;
uint32_t g_call_depth = 0, g_call_depth_max = 0;
uint32_t g_total_calls = 0, g_total_icalls = 0;
int g_heap_check_enabled = 0;
uint32_t g_heap_check_last_ok_call = 0, g_heap_check_last_ok_va = 0;
char g_trace_ring[TRACE_RING_SIZE][TRACE_ENTRY_SIZE];
uint32_t g_trace_ring_idx = 0;
FILE* g_trace_file = NULL;

/* ============================================================
 * Memory Layout (from config/pe_analysis.json — Z_XVT__.EXE)
 *
 *   .text   VA 0x00401000  vsize 0x113204  raw 0x000400  rawsize 0x113400
 *   .rdata  VA 0x00515000  vsize 0x001D89  raw 0x113800  rawsize 0x001E00
 *   .data   VA 0x00517000  vsize 0x638BE0  raw 0x115600  rawsize 0x012800
 *   .idata  VA 0x00B50000  vsize 0x0013C4  raw 0x127E00  rawsize 0x001400
 *
 * .text is mapped as DATA (embedded jump tables / string constants that the
 * lifted code still reads through MEM macros); we *are* the code.
 * ============================================================ */

#define XVT_STACK_SIZE   0x00800000  /* 8 MB guest stack, OS-placed (see below) */
#define XVT_REGION_START 0x00400000  /* image base */
#define XVT_REGION_END   0x00B60000  /* past .idata end (0xB513C4), page-rounded */
#define XVT_REGION_SIZE  (XVT_REGION_END - XVT_REGION_START)

static void* g_region_alloc = NULL;

/* ============================================================
 * FILE* registry — native CRT wrappers validate guest FILE*s
 * ============================================================ */

#define RECOMP_MAX_FP 64
static FILE* g_recomp_fps[RECOMP_MAX_FP];
void recomp_fp_register(FILE* fp) { if (!fp) return; for (int i=0;i<RECOMP_MAX_FP;i++) if (!g_recomp_fps[i]) { g_recomp_fps[i]=fp; return; } }
void recomp_fp_unregister(FILE* fp) { for (int i=0;i<RECOMP_MAX_FP;i++) if (g_recomp_fps[i]==fp) { g_recomp_fps[i]=NULL; return; } }
int  recomp_fp_valid(FILE* fp) { if (!fp) return 0; for (int i=0;i<RECOMP_MAX_FP;i++) if (g_recomp_fps[i]==fp) return 1; return 0; }

/* ============================================================
 * Dispatch Lookups
 * ============================================================ */

recomp_func_t recomp_lookup(uint32_t va) {
    int lo = 0, hi = (int)recomp_dispatch_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        uint32_t mid_va = recomp_dispatch_table[mid].address;
        if (mid_va == va) return recomp_dispatch_table[mid].func;
        else if (mid_va < va) lo = mid + 1;
        else hi = mid - 1;
    }
    return NULL;
}

/* Manual per-function overrides. Empty until bringup needs one. */
static const recomp_dispatch_entry_t g_manual_overrides[] = {
    { 0, NULL }
};
static const int g_manual_override_count = 0;

recomp_func_t recomp_lookup_manual(uint32_t va) {
    for (int i = 0; i < g_manual_override_count; i++)
        if (g_manual_overrides[i].address == va) return g_manual_overrides[i].func;
    return NULL;
}

#define MAX_IMPORT_BRIDGES 256
recomp_dispatch_entry_t g_import_bridges[MAX_IMPORT_BRIDGES];
int g_import_bridge_count = 0;

recomp_func_t recomp_lookup_import(uint32_t va) {
    for (int i = 0; i < g_import_bridge_count; i++)
        if (g_import_bridges[i].address == va) return g_import_bridges[i].func;
    return NULL;
}

/* Given a HOST code address (crash EIP), name the guest function whose host
 * entry is the greatest value <= it. MSVC lays lifted functions out roughly in
 * table order. Linear scan — only runs on a crash. */
static uint32_t guest_func_for_host(uintptr_t host_addr, uint32_t* out_off) {
    uintptr_t best_fn = 0; uint32_t best_va = 0;
    for (uint32_t i = 0; i < recomp_dispatch_count; i++) {
        uintptr_t fn = (uintptr_t)recomp_dispatch_table[i].func;
        if (fn <= host_addr && fn > best_fn) { best_fn = fn; best_va = recomp_dispatch_table[i].address; }
    }
    if (best_fn && (host_addr - best_fn) < 0x20000) { *out_off = (uint32_t)(host_addr - best_fn); return best_va; }
    *out_off = 0; return 0;
}

/* ============================================================
 * Dynamic Native Function Registry
 *
 * GetProcAddress hands the game a real DLL address; when it later ICALLs
 * that address we bridge the call using the arg count registered here.
 * ============================================================ */

typedef struct { const char* name; int nargs; } native_func_info_t;
static const native_func_info_t g_known_native_funcs[] = {
    { "GetVersion", 0 }, { "GetVersionExA", 1 }, { "GetTickCount", 0 },
    { "IsProcessorFeaturePresent", 1 }, { "IsDebuggerPresent", 0 },
    { "GetCurrentProcessId", 0 }, { "GetCurrentThreadId", 0 }, { "GetCurrentProcess", 0 },
    { "QueryPerformanceCounter", 1 }, { "QueryPerformanceFrequency", 1 },
    { "GetSystemInfo", 1 }, { "GlobalMemoryStatus", 1 },
    { "GetModuleHandleA", 1 }, { "GetProcAddress", 2 }, { "LoadLibraryA", 1 }, { "FreeLibrary", 1 },
    { "CreateFileA", 7 }, { "ReadFile", 5 }, { "WriteFile", 5 }, { "CloseHandle", 1 },
    { "SetFilePointer", 4 }, { "GetFileSize", 2 }, { "DeleteFileA", 1 },
    { "GetLastError", 0 }, { "SetLastError", 1 }, { "Sleep", 1 },
    { "VirtualAlloc", 4 }, { "VirtualFree", 3 }, { "HeapAlloc", 3 }, { "HeapFree", 3 },
    { "InitializeCriticalSection", 1 }, { "DeleteCriticalSection", 1 },
    { "EnterCriticalSection", 1 }, { "LeaveCriticalSection", 1 },
    { "WaitForSingleObject", 2 }, { "CreateEventA", 4 }, { "SetEvent", 1 },
    { "OutputDebugStringA", 1 }, { "GetModuleFileNameA", 3 },
    { "MessageBoxA", 4 }, { "GetDesktopWindow", 0 }, { "ShowWindow", 2 },
    { "SetWindowPos", 7 }, { "GetSystemMetrics", 1 }, { "ShowCursor", 1 },
    { "PeekMessageA", 5 }, { "GetMessageA", 4 }, { "DispatchMessageA", 1 },
    { "DefWindowProcA", 4 }, { "RegisterClassA", 1 }, { "CreateWindowExA", 12 },
    { "GetDC", 1 }, { "ReleaseDC", 2 }, { "GetDeviceCaps", 2 },
    { "RegOpenKeyExA", 5 }, { "RegCloseKey", 1 }, { "RegQueryValueExA", 6 },
    { "timeGetTime", 0 }, { "timeBeginPeriod", 1 }, { "timeEndPeriod", 1 },
    { "joyGetNumDevs", 0 }, { "joyGetDevCapsA", 3 }, { "joyGetPosEx", 2 },
    { "DirectDrawCreate", 3 }, { "DirectDrawEnumerateA", 2 },
    { "DirectInputCreateA", 4 }, { "DirectSoundCreate", 3 },
    { "DirectPlayCreate", 3 }, { "DirectPlayEnumerateA", 2 },
    { NULL, 0 }
};

int lookup_native_nargs(const char* name) {
    for (int i = 0; g_known_native_funcs[i].name; i++)
        if (strcmp(g_known_native_funcs[i].name, name) == 0) return g_known_native_funcs[i].nargs;
    return -1;
}

#define MAX_NATIVE_FUNCS 128
typedef struct { uint32_t addr; int nargs; char name[64]; } native_reg_entry_t;
static native_reg_entry_t g_native_reg[MAX_NATIVE_FUNCS];
static int g_native_reg_count = 0;

void recomp_register_native(uint32_t addr, const char* name, int nargs) {
    for (int i = 0; i < g_native_reg_count; i++) if (g_native_reg[i].addr == addr) return;
    if (g_native_reg_count >= MAX_NATIVE_FUNCS) { fprintf(stderr, "WARNING: native registry full\n"); return; }
    native_reg_entry_t* e = &g_native_reg[g_native_reg_count++];
    e->addr = addr; e->nargs = nargs;
    strncpy(e->name, name, sizeof(e->name) - 1); e->name[sizeof(e->name) - 1] = '\0';
    fprintf(stderr, "[*] Registered native: %s @ 0x%08X (%d args)\n", name, addr, nargs);
}

typedef uint32_t (__stdcall *STDFN0_t)(void);
typedef uint32_t (__stdcall *STDFN1_t)(uint32_t);
typedef uint32_t (__stdcall *STDFN2_t)(uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN3_t)(uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN4_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN5_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN6_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN7_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN8_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
typedef uint32_t (__stdcall *STDFN12_t)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);

int recomp_native_call(uint32_t va) {
    for (int i = 0; i < g_native_reg_count; i++) {
        if (g_native_reg[i].addr != va) continue;
        int n = g_native_reg[i].nargs;
        uint32_t a[12] = {0};
        for (int j = 0; j < n && j < 12; j++) a[j] = MEM32(g_esp + 4 + j * 4);
        uint32_t r = 0;
        void* fn = (void*)(uintptr_t)va;
        switch (n) {
            case 0:  r = ((STDFN0_t)fn)(); break;
            case 1:  r = ((STDFN1_t)fn)(a[0]); break;
            case 2:  r = ((STDFN2_t)fn)(a[0],a[1]); break;
            case 3:  r = ((STDFN3_t)fn)(a[0],a[1],a[2]); break;
            case 4:  r = ((STDFN4_t)fn)(a[0],a[1],a[2],a[3]); break;
            case 5:  r = ((STDFN5_t)fn)(a[0],a[1],a[2],a[3],a[4]); break;
            case 6:  r = ((STDFN6_t)fn)(a[0],a[1],a[2],a[3],a[4],a[5]); break;
            case 7:  r = ((STDFN7_t)fn)(a[0],a[1],a[2],a[3],a[4],a[5],a[6]); break;
            case 8:  r = ((STDFN8_t)fn)(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]); break;
            case 12: r = ((STDFN12_t)fn)(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11]); break;
            default: fprintf(stderr, "WARNING: native %s has %d args (unsupported)\n", g_native_reg[i].name, n); break;
        }
        g_eax = r;
        g_esp += 4 + n * 4;  /* stdcall: callee pops args */
        return 1;
    }
    return 0;
}

/* ============================================================
 * Native WndProc Bridge
 *
 * Windows calls the window procedure directly, but the guest's procedure is
 * lifted C, not executable code at its original VA — calling it faults. The
 * RegisterClassA bridge swaps lpfnWndProc for one of these trampolines, which
 * re-enters the guest through the dispatch table.
 *
 * Re-entrant: Windows can call this while guest code is mid-execution (e.g.
 * during CreateWindowExA), so the whole register model is saved and restored.
 * ============================================================ */

#define MAX_WNDPROCS 4
static uint32_t g_guest_wndproc[MAX_WNDPROCS];
static int g_wndproc_count = 0;

static LRESULT wndproc_dispatch(int slot, HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    uint32_t va = g_guest_wndproc[slot];
    recomp_func_t fn = recomp_lookup_manual(va);
    if (!fn) fn = recomp_lookup(va);
    if (!fn) {
        fprintf(stderr, "!!! WndProc slot %d: no recompiled function at 0x%08X\n", slot, va);
        return DefWindowProcA(hwnd, msg, wp, lp);
    }

    uint32_t s_eax = g_eax, s_ecx = g_ecx, s_edx = g_edx, s_esp = g_esp;
    uint32_t s_ebx = g_ebx, s_esi = g_esi, s_edi = g_edi;

    PUSH32(g_esp, (uint32_t)lp);
    PUSH32(g_esp, (uint32_t)wp);
    PUSH32(g_esp, (uint32_t)msg);
    PUSH32(g_esp, (uint32_t)(uintptr_t)hwnd);
    PUSH32(g_esp, 0xDEAD0000u);
    fn();
    LRESULT r = (LRESULT)g_eax;

    g_eax = s_eax; g_ecx = s_ecx; g_edx = s_edx; g_esp = s_esp;
    g_ebx = s_ebx; g_esi = s_esi; g_edi = s_edi;
    return r;
}

#define WNDPROC_TRAMPOLINE(n) \
    static LRESULT CALLBACK wndproc_tramp##n(HWND h, UINT m, WPARAM w, LPARAM l) { \
        return wndproc_dispatch(n, h, m, w, l); }
WNDPROC_TRAMPOLINE(0)
WNDPROC_TRAMPOLINE(1)
WNDPROC_TRAMPOLINE(2)
WNDPROC_TRAMPOLINE(3)
static const WNDPROC g_wndproc_tramps[MAX_WNDPROCS] = {
    wndproc_tramp0, wndproc_tramp1, wndproc_tramp2, wndproc_tramp3
};

/* Called from the RegisterClassA bridge: takes the guest WNDPROC address,
 * returns the native trampoline to register in its place. */
uint32_t recomp_register_wndproc(uint32_t guest_va) {
    for (int i = 0; i < g_wndproc_count; i++)
        if (g_guest_wndproc[i] == guest_va) return (uint32_t)(uintptr_t)g_wndproc_tramps[i];
    if (g_wndproc_count >= MAX_WNDPROCS) {
        fprintf(stderr, "!!! WndProc trampolines exhausted (guest 0x%08X)\n", guest_va);
        return guest_va;
    }
    int slot = g_wndproc_count++;
    g_guest_wndproc[slot] = guest_va;
    fprintf(stderr, "[*] WndProc slot %d: guest 0x%08X -> native %p\n",
            slot, guest_va, (void*)g_wndproc_tramps[slot]);
    return (uint32_t)(uintptr_t)g_wndproc_tramps[slot];
}

/* ============================================================
 * VEH Crash Handler — dump state, don't try to continue
 * ============================================================ */

static LONG WINAPI veh_handler(EXCEPTION_POINTERS* ep) {
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    /* Ignore the noise Windows raises normally */
    if (code == DBG_PRINTEXCEPTION_C || code == 0x406D1388 /* thread name */ ||
        code == DBG_PRINTEXCEPTION_WIDE_C)
        return EXCEPTION_CONTINUE_SEARCH;

    static int reentry = 0;
    if (reentry++) return EXCEPTION_CONTINUE_SEARCH;

    uint32_t off = 0;
    uint32_t va = guest_func_for_host((uintptr_t)ep->ExceptionRecord->ExceptionAddress, &off);

    fprintf(stderr, "\n!!! EXCEPTION 0x%08lX at host %p\n", code, ep->ExceptionRecord->ExceptionAddress);
    if (va) fprintf(stderr, "    guest: sub_%08X + 0x%X\n", va, off);
    if (code == EXCEPTION_ACCESS_VIOLATION)
        fprintf(stderr, "    %s address 0x%08IX\n",
                ep->ExceptionRecord->ExceptionInformation[0] ? "write to" : "read from",
                ep->ExceptionRecord->ExceptionInformation[1]);
    fprintf(stderr, "    EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESP=%08X ESI=%08X EDI=%08X\n",
            g_eax, g_ecx, g_edx, g_ebx, g_esp, g_esi, g_edi);
    fprintf(stderr, "    calls=%u icalls=%u depth=%u (max %u)\n",
            g_total_calls, g_total_icalls, g_call_depth, g_call_depth_max);

    fprintf(stderr, "    last ICALL targets:\n");
    for (int i = 8; i > 0; i--) {
        uint32_t idx = (g_icall_trace_idx - i) & (ICALL_TRACE_SIZE - 1);
        if (g_icall_trace[idx]) fprintf(stderr, "      [-%d] 0x%08X\n", i, g_icall_trace[idx]);
    }

    fprintf(stderr, "    call trace (last 32):\n");
    uint32_t start = (g_trace_ring_idx > 32) ? g_trace_ring_idx - 32 : 0;
    for (uint32_t i = start; i < g_trace_ring_idx; i++) {
        uint32_t idx = i & (TRACE_RING_SIZE - 1);
        if (g_trace_ring[idx][0]) fprintf(stderr, "      %s", g_trace_ring[idx]);
    }
    fflush(stderr);
    reentry--;
    return EXCEPTION_CONTINUE_SEARCH;
}

/* ============================================================
 * Memory Setup
 *
 * The 32-bit process heap normally reserves ~0x400000-0xBFB000 — exactly
 * where the guest image lives. Move the process heap out of the way first,
 * or heap growth writes metadata into the guest's .data.
 * ============================================================ */

static void relocate_process_heap(void) {
    HANDLE oldHeap = GetProcessHeap();
    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(oldHeap, &mbi, sizeof(mbi))) return;
    uintptr_t base = (uintptr_t)mbi.AllocationBase;
    if (base >= XVT_REGION_END || base + mbi.RegionSize <= XVT_REGION_START) {
        printf("[*] Process heap at %p does not conflict\n", (void*)base);
        return;
    }
    HANDLE newHeap = HeapCreate(0, 16 * 1024 * 1024, 0);
    if (!newHeap) { fprintf(stderr, "WARNING: HeapCreate failed\n"); return; }
    /* PEB->ProcessHeap is at TEB->Peb (fs:[0x30]) + 0x18 on x86 */
    {
        void** peb = (void**)__readfsdword(0x30);
        peb[0x18 / sizeof(void*)] = newHeap;
    }
    printf("[*] Relocated process heap: %p -> %p (was in guest range)\n", oldHeap, newHeap);
}

static int setup_memory(const char* exe_path) {
    relocate_process_heap();

    /* Map the guest image range at its original VAs. On a 32-bit process the
     * default heap already RESERVES ~0x400000-0xBFB000, so a plain reserve
     * fails (error 487) — commit region by region into whatever is there. */
    printf("[*] Mapping guest image range 0x%08X-0x%08X (%u KB)\n",
           XVT_REGION_START, XVT_REGION_END, (unsigned)(XVT_REGION_SIZE / 1024));
    {
        MEMORY_BASIC_INFORMATION mbi;
        uintptr_t scan = XVT_REGION_START;
        size_t got = 0;
        while (scan < XVT_REGION_END) {
            if (!VirtualQuery((void*)scan, &mbi, sizeof(mbi)) || mbi.RegionSize == 0) break;
            SIZE_T sz = mbi.RegionSize;
            if (scan + sz > XVT_REGION_END) sz = XVT_REGION_END - scan;
            if (mbi.State == MEM_COMMIT) {
                got += sz;
            } else {
                DWORD flags = (mbi.State == MEM_FREE) ? (MEM_RESERVE | MEM_COMMIT) : MEM_COMMIT;
                if (VirtualAlloc((void*)scan, sz, flags, PAGE_READWRITE)) got += sz;
                else printf("    could not map 0x%08X-0x%08X (state 0x%lX, err %lu)\n",
                            (uint32_t)scan, (uint32_t)(scan + sz), mbi.State, GetLastError());
            }
            scan += mbi.RegionSize;
        }
        if (got < (size_t)XVT_REGION_SIZE) {
            fprintf(stderr, "FATAL: only mapped %u of %u KB of the guest image range\n",
                    (unsigned)(got / 1024), (unsigned)(XVT_REGION_SIZE / 1024));
            return 0;
        }
        g_region_alloc = (void*)(uintptr_t)XVT_REGION_START;
    }
    g_mem_base = 0;  /* identity mapping: guest VA == host address */

    /* Guest stack: the low VAs the original process used (0x100000-0x400000)
     * hold this process's own mapped sections, so let the OS place it. With
     * identity mapping any host address serves as a guest stack address. */
    {
        void* stk = VirtualAlloc(NULL, XVT_STACK_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!stk) { fprintf(stderr, "FATAL: guest stack allocation failed\n"); return 0; }
        g_esp = (uint32_t)((uintptr_t)stk + XVT_STACK_SIZE - 16);
        printf("[*] Guest stack at %p-%p\n", stk, (char*)stk + XVT_STACK_SIZE);
    }

    /* Load the original image's sections at their virtual addresses.
     * .text included: lifted code still reads jump tables / strings from it. */
    FILE* f = fopen(exe_path, "rb");
    if (!f) { fprintf(stderr, "FATAL: cannot open %s\n", exe_path); return 0; }
    struct { uint32_t va, raw, size; const char* name; } secs[] = {
        { 0x00401000, 0x000400, 0x113400, ".text"  },
        { 0x00515000, 0x113800, 0x001E00, ".rdata" },
        { 0x00517000, 0x115600, 0x012800, ".data"  },
        { 0x00B50000, 0x127E00, 0x001400, ".idata" },
    };
    for (int i = 0; i < 4; i++) {
        fseek(f, secs[i].raw, SEEK_SET);
        size_t n = fread((void*)ADDR(secs[i].va), 1, secs[i].size, f);
        printf("[*] Loaded %-6s VA 0x%08X  %u/%u bytes\n",
               secs[i].name, secs[i].va, (unsigned)n, secs[i].size);
        if (n != secs[i].size) { fprintf(stderr, "FATAL: short read on %s\n", secs[i].name); fclose(f); return 0; }
    }
    fclose(f);
    return 1;
}

/* ============================================================
 * Entry Point
 * ============================================================ */

int main(int argc, char* argv[]) {
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("=== X-Wing vs TIE Fighter Static Recompilation ===\n");
    printf("[*] Dispatch table: %u functions\n", recomp_dispatch_count);

    const char* exe_path = (argc > 1) ? argv[1] : "game_data/Z_XVT__.EXE";

    AddVectoredExceptionHandler(1, veh_handler);
    SetUnhandledExceptionFilter(veh_handler);

    if (!setup_memory(exe_path)) { fprintf(stderr, "FATAL: memory setup failed\n"); return 1; }
    printf("[*] Guest memory ready (ESP = 0x%08X, g_mem_base = %lld)\n",
           g_esp, (long long)g_mem_base);

    { extern void register_import_bridges(void); register_import_bridges(); }

    timeBeginPeriod(1);

    /* Entry: PE entry point (guest CRT startup) by default. Override with
     * XVT_ENTRY=0x4XXXXX once WinMain is located, to skip guest CRT init. */
    uint32_t entry = 0x004FD970;
    { const char* e = getenv("XVT_ENTRY"); if (e) entry = (uint32_t)strtoul(e, NULL, 0); }

    recomp_func_t fn = recomp_lookup(entry);
    if (!fn) { fprintf(stderr, "FATAL: no recompiled function at 0x%08X\n", entry); return 1; }

    printf("[*] Entering guest at 0x%08X ...\n\n", entry);
    fflush(stdout);

    PUSH32(g_esp, 0xDEAD0000u);  /* return address sentinel */
    fn();

    printf("\n[*] Guest returned, eax = 0x%08X (calls=%u icalls=%u)\n",
           g_eax, g_total_calls, g_total_icalls);
    return 0;
}
