/* Import bridges: bootstrapped by tools/gen_bridges.py, then HAND-MAINTAINED.
 *
 * Each bridge reads arguments from the simulated stack, calls the real Win32
 * API, and returns the result in g_eax. Regenerating overwrites the hand edits
 * below (WndProc swap in RegisterClassA, native registration in GetProcAddress,
 * DirectX creates routed to COM mocks) - re-apply them if you ever do.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/recomp_types.h"

/* Generic stdcall function pointer types by arg count */
typedef uint32_t (__stdcall *STDFN0)(void);
typedef uint32_t (__stdcall *STDFN1)(uint32_t);
typedef uint32_t (__stdcall *STDFN2)(uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN3)(uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN4)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN5)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN6)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN7)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN8)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN9)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN10)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN11)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN12)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN13)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef uint32_t (__stdcall *STDFN14)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/* ======== ADVAPI32.dll ======== */

static void bridge_RegSetValueExA_00B503F4(void) { /* ADVAPI32.dll:RegSetValueExA (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("ADVAPI32.dll"), "RegSetValueExA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_RegOpenKeyExA_00B503F8(void) { /* ADVAPI32.dll:RegOpenKeyExA (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("ADVAPI32.dll"), "RegOpenKeyExA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_RegQueryValueExA_00B503FC(void) { /* ADVAPI32.dll:RegQueryValueExA (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("ADVAPI32.dll"), "RegQueryValueExA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_RegCloseKey_00B50400(void) { /* ADVAPI32.dll:RegCloseKey (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("ADVAPI32.dll"), "RegCloseKey");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

/* ======== DDRAW.dll ======== */

static void bridge_DirectDrawCreate_00B50408(void) { /* DDRAW.dll:DirectDrawCreate */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DDRAW.dll:DirectDrawCreate\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

/* ======== DINPUT.dll ======== */

static void bridge_DirectInputCreateA_00B50410(void) { /* DINPUT.dll:DirectInputCreateA */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DINPUT.dll:DirectInputCreateA\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

/* ======== DPLAYX.dll ======== */

static void bridge_ordinal_4_00B50418(void) { /* DPLAYX.dll:ordinal_4 */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DPLAYX.dll:ordinal_4\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

static void bridge_ordinal_1_00B5041C(void) { /* DPLAYX.dll:ordinal_1 */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DPLAYX.dll:ordinal_1\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

static void bridge_ordinal_2_00B50420(void) { /* DPLAYX.dll:ordinal_2 */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DPLAYX.dll:ordinal_2\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

/* ======== DSOUND.dll ======== */

static void bridge_DirectSoundCreate_00B50428(void) { /* DSOUND.dll:DirectSoundCreate */
    static int w = 0;
    if (!w) { fprintf(stderr, "STUB: DSOUND.dll:DirectSoundCreate\n"); w = 1; }
    g_eax = 0; g_esp += 4;
}

/* ======== GDI32.dll ======== */

static void bridge_StretchBlt_00B50430(void) { /* GDI32.dll:StretchBlt (11 args) */
    static STDFN11 fn = NULL;
    if (!fn) fn = (STDFN11)GetProcAddress(LoadLibraryA("GDI32.dll"), "StretchBlt");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    uint32_t a8 = MEM32(g_esp + 36);
    uint32_t a9 = MEM32(g_esp + 40);
    uint32_t a10 = MEM32(g_esp + 44);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    g_esp += 48;
}

static void bridge_SetPixel_00B50434(void) { /* GDI32.dll:SetPixel (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetPixel");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_CreateCompatibleDC_00B50438(void) { /* GDI32.dll:CreateCompatibleDC (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("GDI32.dll"), "CreateCompatibleDC");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetPixel_00B5043C(void) { /* GDI32.dll:GetPixel (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("GDI32.dll"), "GetPixel");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_DeleteDC_00B50440(void) { /* GDI32.dll:DeleteDC (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("GDI32.dll"), "DeleteDC");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetObjectA_00B50444(void) { /* GDI32.dll:GetObjectA (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("GDI32.dll"), "GetObjectA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_SetTextCharacterExtra_00B50448(void) { /* GDI32.dll:SetTextCharacterExtra (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetTextCharacterExtra");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetTextExtentPoint32A_00B5044C(void) { /* GDI32.dll:GetTextExtentPoint32A (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("GDI32.dll"), "GetTextExtentPoint32A");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_CreateFontA_00B50450(void) { /* GDI32.dll:CreateFontA (14 args) */
    static STDFN14 fn = NULL;
    if (!fn) fn = (STDFN14)GetProcAddress(LoadLibraryA("GDI32.dll"), "CreateFontA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    uint32_t a8 = MEM32(g_esp + 36);
    uint32_t a9 = MEM32(g_esp + 40);
    uint32_t a10 = MEM32(g_esp + 44);
    uint32_t a11 = MEM32(g_esp + 48);
    uint32_t a12 = MEM32(g_esp + 52);
    uint32_t a13 = MEM32(g_esp + 56);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    g_esp += 60;
}

static void bridge_SetTextColor_00B50454(void) { /* GDI32.dll:SetTextColor (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetTextColor");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetBkColor_00B50458(void) { /* GDI32.dll:SetBkColor (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetBkColor");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetMapMode_00B5045C(void) { /* GDI32.dll:SetMapMode (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetMapMode");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_ExtTextOutA_00B50460(void) { /* GDI32.dll:ExtTextOutA (8 args) */
    static STDFN8 fn = NULL;
    if (!fn) fn = (STDFN8)GetProcAddress(LoadLibraryA("GDI32.dll"), "ExtTextOutA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7);
    g_esp += 36;
}

static void bridge_SelectObject_00B50464(void) { /* GDI32.dll:SelectObject (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SelectObject");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetBkMode_00B50468(void) { /* GDI32.dll:SetBkMode (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("GDI32.dll"), "SetBkMode");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetStockObject_00B5046C(void) { /* GDI32.dll:GetStockObject (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("GDI32.dll"), "GetStockObject");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_DeleteObject_00B50470(void) { /* GDI32.dll:DeleteObject (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("GDI32.dll"), "DeleteObject");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

/* ======== KERNEL32.dll ======== */

static void bridge__lclose_00B50478(void) { /* KERNEL32.dll:_lclose (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "_lclose");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge__lread_00B5047C(void) { /* KERNEL32.dll:_lread (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "_lread");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge__lopen_00B50480(void) { /* KERNEL32.dll:_lopen (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "_lopen");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetModuleHandleA_00B50484(void) { /* KERNEL32.dll:GetModuleHandleA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetModuleHandleA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_FindFirstFileA_00B50488(void) { /* KERNEL32.dll:FindFirstFileA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FindFirstFileA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_LockResource_00B5048C(void) { /* KERNEL32.dll:LockResource (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LockResource");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_FindResourceA_00B50490(void) { /* KERNEL32.dll:FindResourceA (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FindResourceA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_GetVolumeInformationA_00B50494(void) { /* KERNEL32.dll:GetVolumeInformationA (8 args) */
    static STDFN8 fn = NULL;
    if (!fn) fn = (STDFN8)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetVolumeInformationA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7);
    g_esp += 36;
}

static void bridge_EnterCriticalSection_00B50498(void) { /* KERNEL32.dll:EnterCriticalSection (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "EnterCriticalSection");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LeaveCriticalSection_00B5049C(void) { /* KERNEL32.dll:LeaveCriticalSection (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LeaveCriticalSection");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ResumeThread_00B504A0(void) { /* KERNEL32.dll:ResumeThread (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "ResumeThread");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LoadResource_00B504A4(void) { /* KERNEL32.dll:LoadResource (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LoadResource");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_InitializeCriticalSection_00B504A8(void) { /* KERNEL32.dll:InitializeCriticalSection (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "InitializeCriticalSection");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_DeleteCriticalSection_00B504AC(void) { /* KERNEL32.dll:DeleteCriticalSection (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "DeleteCriticalSection");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_CloseHandle_00B504B0(void) { /* KERNEL32.dll:CloseHandle (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CloseHandle");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_TerminateThread_00B504B4(void) { /* KERNEL32.dll:TerminateThread (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TerminateThread");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetThreadPriority_00B504B8(void) { /* KERNEL32.dll:SetThreadPriority (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetThreadPriority");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SuspendThread_00B504BC(void) { /* KERNEL32.dll:SuspendThread (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SuspendThread");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_CompareStringW_00B504C0(void) { /* KERNEL32.dll:CompareStringW (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CompareStringW");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_CompareStringA_00B504C4(void) { /* KERNEL32.dll:CompareStringA (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CompareStringA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_SetEndOfFile_00B504C8(void) { /* KERNEL32.dll:SetEndOfFile (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetEndOfFile");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetLocaleInfoW_00B504CC(void) { /* KERNEL32.dll:GetLocaleInfoW (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetLocaleInfoW");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetLocaleInfoA_00B504D0(void) { /* KERNEL32.dll:GetLocaleInfoA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetLocaleInfoA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_CreateFileA_00B504D4(void) { /* KERNEL32.dll:CreateFileA (7 args) */
    static STDFN7 fn = NULL;
    if (!fn) fn = (STDFN7)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CreateFileA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6);
    g_esp += 32;
}

static void bridge_GetThreadPriority_00B504D8(void) { /* KERNEL32.dll:GetThreadPriority (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetThreadPriority");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetTickCount_00B504DC(void) { /* KERNEL32.dll:GetTickCount (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetTickCount");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetStringTypeW_00B504E0(void) { /* KERNEL32.dll:GetStringTypeW (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetStringTypeW");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetStringTypeA_00B504E4(void) { /* KERNEL32.dll:GetStringTypeA (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetStringTypeA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_LCMapStringW_00B504E8(void) { /* KERNEL32.dll:LCMapStringW (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LCMapStringW");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_LCMapStringA_00B504EC(void) { /* KERNEL32.dll:LCMapStringA (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LCMapStringA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_GetOEMCP_00B504F0(void) { /* KERNEL32.dll:GetOEMCP (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetOEMCP");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetACP_00B504F4(void) { /* KERNEL32.dll:GetACP (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetACP");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_WideCharToMultiByte_00B504F8(void) { /* KERNEL32.dll:WideCharToMultiByte (8 args) */
    static STDFN8 fn = NULL;
    if (!fn) fn = (STDFN8)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "WideCharToMultiByte");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7);
    g_esp += 36;
}

static void bridge_GetEnvironmentStringsW_00B504FC(void) { /* KERNEL32.dll:GetEnvironmentStringsW (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetEnvironmentStringsW");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_FreeEnvironmentStringsW_00B50500(void) { /* KERNEL32.dll:FreeEnvironmentStringsW (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FreeEnvironmentStringsW");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetEnvironmentStrings_00B50504(void) { /* KERNEL32.dll:GetEnvironmentStrings (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetEnvironmentStrings");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_MultiByteToWideChar_00B50508(void) { /* KERNEL32.dll:MultiByteToWideChar (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "MultiByteToWideChar");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_FreeEnvironmentStringsA_00B5050C(void) { /* KERNEL32.dll:FreeEnvironmentStringsA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FreeEnvironmentStringsA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_UnhandledExceptionFilter_00B50510(void) { /* KERNEL32.dll:UnhandledExceptionFilter (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "UnhandledExceptionFilter");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_RtlUnwind_00B50514(void) { /* KERNEL32.dll:RtlUnwind (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "RtlUnwind");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetUserDefaultLCID_00B50518(void) { /* KERNEL32.dll:GetUserDefaultLCID (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetUserDefaultLCID");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_IsValidCodePage_00B5051C(void) { /* KERNEL32.dll:IsValidCodePage (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "IsValidCodePage");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_IsValidLocale_00B50520(void) { /* KERNEL32.dll:IsValidLocale (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "IsValidLocale");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetCPInfo_00B50524(void) { /* KERNEL32.dll:GetCPInfo (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCPInfo");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetFileType_00B50528(void) { /* KERNEL32.dll:GetFileType (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetFileType");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_SetHandleCount_00B5052C(void) { /* KERNEL32.dll:SetHandleCount (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetHandleCount");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ReadFile_00B50530(void) { /* KERNEL32.dll:ReadFile (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "ReadFile");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_FlushFileBuffers_00B50534(void) { /* KERNEL32.dll:FlushFileBuffers (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FlushFileBuffers");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_DebugBreak_00B50538(void) { /* KERNEL32.dll:DebugBreak (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "DebugBreak");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetCurrentThread_00B5053C(void) { /* KERNEL32.dll:GetCurrentThread (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCurrentThread");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_TlsGetValue_00B50540(void) { /* KERNEL32.dll:TlsGetValue (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TlsGetValue");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_SetLastError_00B50544(void) { /* KERNEL32.dll:SetLastError (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetLastError");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_FindNextFileA_00B50548(void) { /* KERNEL32.dll:FindNextFileA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FindNextFileA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_FindClose_00B5054C(void) { /* KERNEL32.dll:FindClose (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FindClose");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GlobalHandle_00B50550(void) { /* KERNEL32.dll:GlobalHandle (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GlobalHandle");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GlobalAlloc_00B50554(void) { /* KERNEL32.dll:GlobalAlloc (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GlobalAlloc");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GlobalLock_00B50558(void) { /* KERNEL32.dll:GlobalLock (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GlobalLock");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetLogicalDriveStringsA_00B5055C(void) { /* KERNEL32.dll:GetLogicalDriveStringsA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetLogicalDriveStringsA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GlobalUnlock_00B50560(void) { /* KERNEL32.dll:GlobalUnlock (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GlobalUnlock");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GlobalFree_00B50564(void) { /* KERNEL32.dll:GlobalFree (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GlobalFree");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_VirtualProtect_00B50568(void) { /* KERNEL32.dll:VirtualProtect (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "VirtualProtect");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetDriveTypeA_00B5056C(void) { /* KERNEL32.dll:GetDriveTypeA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetDriveTypeA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_Sleep_00B50570(void) { /* KERNEL32.dll:Sleep (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "Sleep");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LocalAlloc_00B50574(void) { /* KERNEL32.dll:LocalAlloc (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LocalAlloc");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_OutputDebugStringA_00B50578(void) { /* KERNEL32.dll:OutputDebugStringA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "OutputDebugStringA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_CreateProcessA_00B5057C(void) { /* KERNEL32.dll:CreateProcessA (10 args) */
    static STDFN10 fn = NULL;
    if (!fn) fn = (STDFN10)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CreateProcessA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    uint32_t a8 = MEM32(g_esp + 36);
    uint32_t a9 = MEM32(g_esp + 40);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    g_esp += 44;
}

static void bridge_SetStdHandle_00B50580(void) { /* KERNEL32.dll:SetStdHandle (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetStdHandle");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetConsoleCtrlHandler_00B50584(void) { /* KERNEL32.dll:SetConsoleCtrlHandler (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetConsoleCtrlHandler");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetFilePointer_00B50588(void) { /* KERNEL32.dll:SetFilePointer (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetFilePointer");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_IsBadReadPtr_00B5058C(void) { /* KERNEL32.dll:IsBadReadPtr (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "IsBadReadPtr");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_HeapValidate_00B50590(void) { /* KERNEL32.dll:HeapValidate (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapValidate");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_FatalAppExitA_00B50594(void) { /* KERNEL32.dll:FatalAppExitA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "FatalAppExitA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_ExitThread_00B50598(void) { /* KERNEL32.dll:ExitThread (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "ExitThread");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LocalFree_00B5059C(void) { /* KERNEL32.dll:LocalFree (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LocalFree");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_HeapAlloc_00B505A0(void) { /* KERNEL32.dll:HeapAlloc (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapAlloc");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_TlsFree_00B505A4(void) { /* KERNEL32.dll:TlsFree (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TlsFree");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_TlsAlloc_00B505A8(void) { /* KERNEL32.dll:TlsAlloc (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TlsAlloc");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_VirtualAlloc_00B505AC(void) { /* KERNEL32.dll:VirtualAlloc (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "VirtualAlloc");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    if (getenv("XVT_TRACE_VA"))
        fprintf(stderr, "  [VA] VirtualAlloc(0x%08X, 0x%X, 0x%X, 0x%X) -> 0x%08X (err %lu)\n",
                a0, a1, a2, a3, g_eax, GetLastError());
    g_esp += 20;
}

static void bridge_VirtualFree_00B505B0(void) { /* KERNEL32.dll:VirtualFree (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "VirtualFree");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_HeapCreate_00B505B4(void) { /* KERNEL32.dll:HeapCreate (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapCreate");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_HeapDestroy_00B505B8(void) { /* KERNEL32.dll:HeapDestroy (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapDestroy");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_HeapFree_00B505BC(void) { /* KERNEL32.dll:HeapFree (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapFree");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_HeapReAlloc_00B505C0(void) { /* KERNEL32.dll:HeapReAlloc (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "HeapReAlloc");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetModuleFileNameA_00B505C4(void) { /* KERNEL32.dll:GetModuleFileNameA (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetModuleFileNameA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_InterlockedIncrement_00B505C8(void) { /* KERNEL32.dll:InterlockedIncrement (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "InterlockedIncrement");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LoadLibraryA_00B505CC(void) { /* KERNEL32.dll:LoadLibraryA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "LoadLibraryA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetProcAddress_00B505D0(void) { /* KERNEL32.dll:GetProcAddress (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetProcAddress");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    /* The game will later ICALL the raw DLL address it just received; register
     * it (with its arg count) so recomp_native_call can bridge that call. */
    if (g_eax && a1 > 0xFFFF) {
        extern int lookup_native_nargs(const char* name);
        const char* name = (const char*)(uintptr_t)a1;
        int nargs = lookup_native_nargs(name);
        if (nargs >= 0) recomp_register_native(g_eax, name, nargs);
        else fprintf(stderr, "[!] GetProcAddress(\"%s\") -> 0x%08X: arg count unknown,"
                             " add it to g_known_native_funcs in main.c\n", name, g_eax);
    }
    g_esp += 12;
}

static void bridge_WriteFile_00B505D4(void) { /* KERNEL32.dll:WriteFile (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "WriteFile");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_InterlockedDecrement_00B505D8(void) { /* KERNEL32.dll:InterlockedDecrement (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "InterlockedDecrement");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetStdHandle_00B505DC(void) { /* KERNEL32.dll:GetStdHandle (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetStdHandle");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_IsBadWritePtr_00B505E0(void) { /* KERNEL32.dll:IsBadWritePtr (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "IsBadWritePtr");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetStartupInfoA_00B505E4(void) { /* KERNEL32.dll:GetStartupInfoA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetStartupInfoA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_DeleteFileA_00B505E8(void) { /* KERNEL32.dll:DeleteFileA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "DeleteFileA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetTimeZoneInformation_00B505EC(void) { /* KERNEL32.dll:GetTimeZoneInformation (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetTimeZoneInformation");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetSystemTime_00B505F0(void) { /* KERNEL32.dll:GetSystemTime (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetSystemTime");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetLocalTime_00B505F4(void) { /* KERNEL32.dll:GetLocalTime (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetLocalTime");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ExitProcess_00B505F8(void) { /* KERNEL32.dll:ExitProcess (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "ExitProcess");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_TerminateProcess_00B505FC(void) { /* KERNEL32.dll:TerminateProcess (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TerminateProcess");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetCurrentProcess_00B50600(void) { /* KERNEL32.dll:GetCurrentProcess (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCurrentProcess");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetLastError_00B50604(void) { /* KERNEL32.dll:GetLastError (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetLastError");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetCurrentThreadId_00B50608(void) { /* KERNEL32.dll:GetCurrentThreadId (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCurrentThreadId");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_TlsSetValue_00B5060C(void) { /* KERNEL32.dll:TlsSetValue (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "TlsSetValue");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetCommandLineA_00B50610(void) { /* KERNEL32.dll:GetCommandLineA (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCommandLineA");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetVersion_00B50614(void) { /* KERNEL32.dll:GetVersion (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetVersion");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_SetEnvironmentVariableA_00B50618(void) { /* KERNEL32.dll:SetEnvironmentVariableA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetEnvironmentVariableA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_GetCurrentDirectoryA_00B5061C(void) { /* KERNEL32.dll:GetCurrentDirectoryA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetCurrentDirectoryA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetCurrentDirectoryA_00B50620(void) { /* KERNEL32.dll:SetCurrentDirectoryA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "SetCurrentDirectoryA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetFullPathNameA_00B50624(void) { /* KERNEL32.dll:GetFullPathNameA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "GetFullPathNameA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_CreateThread_00B50628(void) { /* KERNEL32.dll:CreateThread (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("KERNEL32.dll"), "CreateThread");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

/* ======== MSACM32.dll ======== */

static void bridge_acmStreamUnprepareHeader_00B50630(void) { /* MSACM32.dll:acmStreamUnprepareHeader (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("MSACM32.dll"), "acmStreamUnprepareHeader");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_acmStreamPrepareHeader_00B50634(void) { /* MSACM32.dll:acmStreamPrepareHeader (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("MSACM32.dll"), "acmStreamPrepareHeader");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_acmStreamClose_00B50638(void) { /* MSACM32.dll:acmStreamClose (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("MSACM32.dll"), "acmStreamClose");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_acmStreamSize_00B5063C(void) { /* MSACM32.dll:acmStreamSize (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("MSACM32.dll"), "acmStreamSize");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_acmStreamConvert_00B50640(void) { /* MSACM32.dll:acmStreamConvert (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("MSACM32.dll"), "acmStreamConvert");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

/* ======== USER32.dll ======== */

static void bridge_RegisterClassA_00B50648(void) { /* USER32.dll:RegisterClassA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "RegisterClassA");
    uint32_t a0 = MEM32(g_esp + 4);
    /* Windows would call the guest WNDPROC at its original VA, where there is
     * no executable code. Swap in a native trampoline that re-enters the guest
     * through the dispatch table (WNDCLASSA.lpfnWndProc is at offset 4). */
    if (a0) {
        extern uint32_t recomp_register_wndproc(uint32_t guest_va);
        uint32_t guest_proc = MEM32(a0 + 4);
        if (guest_proc) MEM32(a0 + 4) = recomp_register_wndproc(guest_proc);
    }
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_GetSystemMetrics_00B5064C(void) { /* USER32.dll:GetSystemMetrics (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "GetSystemMetrics");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_wsprintfA_00B50650(void) { /* USER32.dll:wsprintfA */
    /* wsprintfA is cdecl varargs */
    uint32_t buf = MEM32(g_esp + 4);
    uint32_t fmt = MEM32(g_esp + 8);
    g_eax = (uint32_t)wvsprintfA((LPSTR)(uintptr_t)buf, (LPCSTR)(uintptr_t)fmt, (va_list)(void*)ADDR(g_esp + 12));
    g_esp += 4; /* cdecl: caller cleans */
}

static void bridge_SetFocus_00B50654(void) { /* USER32.dll:SetFocus (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "SetFocus");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_MessageBoxA_00B50658(void) { /* USER32.dll:MessageBoxA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("USER32.dll"), "MessageBoxA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_DispatchMessageA_00B5065C(void) { /* USER32.dll:DispatchMessageA (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "DispatchMessageA");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_TranslateMessage_00B50660(void) { /* USER32.dll:TranslateMessage (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "TranslateMessage");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_UpdateWindow_00B50664(void) { /* USER32.dll:UpdateWindow (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "UpdateWindow");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ShowCursor_00B50668(void) { /* USER32.dll:ShowCursor (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "ShowCursor");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_SetForegroundWindow_00B5066C(void) { /* USER32.dll:SetForegroundWindow (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "SetForegroundWindow");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_PeekMessageA_00B50670(void) { /* USER32.dll:PeekMessageA (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("USER32.dll"), "PeekMessageA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_DefWindowProcA_00B50674(void) { /* USER32.dll:DefWindowProcA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("USER32.dll"), "DefWindowProcA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetMessageA_00B50678(void) { /* USER32.dll:GetMessageA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("USER32.dll"), "GetMessageA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_GetForegroundWindow_00B5067C(void) { /* USER32.dll:GetForegroundWindow (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("USER32.dll"), "GetForegroundWindow");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_GetCursorPos_00B50680(void) { /* USER32.dll:GetCursorPos (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "GetCursorPos");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_LoadImageA_00B50684(void) { /* USER32.dll:LoadImageA (6 args) */
    static STDFN6 fn = NULL;
    if (!fn) fn = (STDFN6)GetProcAddress(LoadLibraryA("USER32.dll"), "LoadImageA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5);
    g_esp += 28;
}

static void bridge_GetDC_00B50688(void) { /* USER32.dll:GetDC (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "GetDC");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ReleaseDC_00B5068C(void) { /* USER32.dll:ReleaseDC (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "ReleaseDC");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_SetRect_00B50690(void) { /* USER32.dll:SetRect (5 args) */
    static STDFN5 fn = NULL;
    if (!fn) fn = (STDFN5)GetProcAddress(LoadLibraryA("USER32.dll"), "SetRect");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4);
    g_esp += 24;
}

static void bridge_FindWindowA_00B50694(void) { /* USER32.dll:FindWindowA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "FindWindowA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_ShowWindowAsync_00B50698(void) { /* USER32.dll:ShowWindowAsync (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "ShowWindowAsync");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_LoadIconA_00B5069C(void) { /* USER32.dll:LoadIconA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "LoadIconA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_LoadCursorA_00B506A0(void) { /* USER32.dll:LoadCursorA (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "LoadCursorA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_MessageBeep_00B506A4(void) { /* USER32.dll:MessageBeep (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "MessageBeep");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_SetCursorPos_00B506A8(void) { /* USER32.dll:SetCursorPos (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("USER32.dll"), "SetCursorPos");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_CreateWindowExA_00B506AC(void) { /* USER32.dll:CreateWindowExA (12 args) */
    static STDFN12 fn = NULL;
    if (!fn) fn = (STDFN12)GetProcAddress(LoadLibraryA("USER32.dll"), "CreateWindowExA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    uint32_t a4 = MEM32(g_esp + 20);
    uint32_t a5 = MEM32(g_esp + 24);
    uint32_t a6 = MEM32(g_esp + 28);
    uint32_t a7 = MEM32(g_esp + 32);
    uint32_t a8 = MEM32(g_esp + 36);
    uint32_t a9 = MEM32(g_esp + 40);
    uint32_t a10 = MEM32(g_esp + 44);
    uint32_t a11 = MEM32(g_esp + 48);
    if (fn) g_eax = fn(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    g_esp += 52;
}

static void bridge_GetKeyboardState_00B506B0(void) { /* USER32.dll:GetKeyboardState (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "GetKeyboardState");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_DestroyWindow_00B506B4(void) { /* USER32.dll:DestroyWindow (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "DestroyWindow");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_PostQuitMessage_00B506B8(void) { /* USER32.dll:PostQuitMessage (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "PostQuitMessage");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_SetCapture_00B506BC(void) { /* USER32.dll:SetCapture (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "SetCapture");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_ReleaseCapture_00B506C0(void) { /* USER32.dll:ReleaseCapture (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("USER32.dll"), "ReleaseCapture");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_SetCursor_00B506C4(void) { /* USER32.dll:SetCursor (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("USER32.dll"), "SetCursor");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_PostMessageA_00B506C8(void) { /* USER32.dll:PostMessageA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("USER32.dll"), "PostMessageA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

/* ======== WINMM.dll ======== */

static void bridge_auxGetNumDevs_00B506D0(void) { /* WINMM.dll:auxGetNumDevs (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("WINMM.dll"), "auxGetNumDevs");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_auxGetVolume_00B506D4(void) { /* WINMM.dll:auxGetVolume (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("WINMM.dll"), "auxGetVolume");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_auxGetDevCapsA_00B506D8(void) { /* WINMM.dll:auxGetDevCapsA (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("WINMM.dll"), "auxGetDevCapsA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_waveInClose_00B506DC(void) { /* WINMM.dll:waveInClose (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("WINMM.dll"), "waveInClose");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_mciSendCommandA_00B506E0(void) { /* WINMM.dll:mciSendCommandA (4 args) */
    static STDFN4 fn = NULL;
    if (!fn) fn = (STDFN4)GetProcAddress(LoadLibraryA("WINMM.dll"), "mciSendCommandA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    uint32_t a3 = MEM32(g_esp + 16);
    if (fn) g_eax = fn(a0, a1, a2, a3);
    g_esp += 20;
}

static void bridge_auxSetVolume_00B506E4(void) { /* WINMM.dll:auxSetVolume (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("WINMM.dll"), "auxSetVolume");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_waveInStop_00B506E8(void) { /* WINMM.dll:waveInStop (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("WINMM.dll"), "waveInStop");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_waveInReset_00B506EC(void) { /* WINMM.dll:waveInReset (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("WINMM.dll"), "waveInReset");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_waveInStart_00B506F0(void) { /* WINMM.dll:waveInStart (1 args) */
    static STDFN1 fn = NULL;
    if (!fn) fn = (STDFN1)GetProcAddress(LoadLibraryA("WINMM.dll"), "waveInStart");
    uint32_t a0 = MEM32(g_esp + 4);
    if (fn) g_eax = fn(a0);
    g_esp += 8;
}

static void bridge_joyGetPosEx_00B506F4(void) { /* WINMM.dll:joyGetPosEx (2 args) */
    static STDFN2 fn = NULL;
    if (!fn) fn = (STDFN2)GetProcAddress(LoadLibraryA("WINMM.dll"), "joyGetPosEx");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    if (fn) g_eax = fn(a0, a1);
    g_esp += 12;
}

static void bridge_waveInAddBuffer_00B506F8(void) { /* WINMM.dll:waveInAddBuffer (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("WINMM.dll"), "waveInAddBuffer");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_timeGetTime_00B506FC(void) { /* WINMM.dll:timeGetTime (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("WINMM.dll"), "timeGetTime");
    if (fn) g_eax = fn();
    g_esp += 4;
}

static void bridge_joyGetDevCapsA_00B50700(void) { /* WINMM.dll:joyGetDevCapsA (3 args) */
    static STDFN3 fn = NULL;
    if (!fn) fn = (STDFN3)GetProcAddress(LoadLibraryA("WINMM.dll"), "joyGetDevCapsA");
    uint32_t a0 = MEM32(g_esp + 4);
    uint32_t a1 = MEM32(g_esp + 8);
    uint32_t a2 = MEM32(g_esp + 12);
    if (fn) g_eax = fn(a0, a1, a2);
    g_esp += 16;
}

static void bridge_joyGetNumDevs_00B50704(void) { /* WINMM.dll:joyGetNumDevs (0 args) */
    static STDFN0 fn = NULL;
    if (!fn) fn = (STDFN0)GetProcAddress(LoadLibraryA("WINMM.dll"), "joyGetNumDevs");
    if (fn) g_eax = fn();
    g_esp += 4;
}

/* ======== Bridge Registration ======== */

extern recomp_dispatch_entry_t g_import_bridges[];
extern int g_import_bridge_count;

void register_import_bridges(void) {
    /* 188 import bridges */
    MEM32(0x00B503F4) = 0xBB000001u;
    g_import_bridges[0].address = 0xBB000001u;
    g_import_bridges[0].func = bridge_RegSetValueExA_00B503F4;
    MEM32(0x00B503F8) = 0xBB000002u;
    g_import_bridges[1].address = 0xBB000002u;
    g_import_bridges[1].func = bridge_RegOpenKeyExA_00B503F8;
    MEM32(0x00B503FC) = 0xBB000003u;
    g_import_bridges[2].address = 0xBB000003u;
    g_import_bridges[2].func = bridge_RegQueryValueExA_00B503FC;
    MEM32(0x00B50400) = 0xBB000004u;
    g_import_bridges[3].address = 0xBB000004u;
    g_import_bridges[3].func = bridge_RegCloseKey_00B50400;
    MEM32(0x00B50408) = 0xBB000005u;
    g_import_bridges[4].address = 0xBB000005u;
    g_import_bridges[4].func = bridge_DirectDrawCreate_00B50408;
    MEM32(0x00B50410) = 0xBB000006u;
    g_import_bridges[5].address = 0xBB000006u;
    g_import_bridges[5].func = bridge_DirectInputCreateA_00B50410;
    MEM32(0x00B50418) = 0xBB000007u;
    g_import_bridges[6].address = 0xBB000007u;
    g_import_bridges[6].func = bridge_ordinal_4_00B50418;
    MEM32(0x00B5041C) = 0xBB000008u;
    g_import_bridges[7].address = 0xBB000008u;
    g_import_bridges[7].func = bridge_ordinal_1_00B5041C;
    MEM32(0x00B50420) = 0xBB000009u;
    g_import_bridges[8].address = 0xBB000009u;
    g_import_bridges[8].func = bridge_ordinal_2_00B50420;
    MEM32(0x00B50428) = 0xBB00000Au;
    g_import_bridges[9].address = 0xBB00000Au;
    g_import_bridges[9].func = bridge_DirectSoundCreate_00B50428;
    MEM32(0x00B50430) = 0xBB00000Bu;
    g_import_bridges[10].address = 0xBB00000Bu;
    g_import_bridges[10].func = bridge_StretchBlt_00B50430;
    MEM32(0x00B50434) = 0xBB00000Cu;
    g_import_bridges[11].address = 0xBB00000Cu;
    g_import_bridges[11].func = bridge_SetPixel_00B50434;
    MEM32(0x00B50438) = 0xBB00000Du;
    g_import_bridges[12].address = 0xBB00000Du;
    g_import_bridges[12].func = bridge_CreateCompatibleDC_00B50438;
    MEM32(0x00B5043C) = 0xBB00000Eu;
    g_import_bridges[13].address = 0xBB00000Eu;
    g_import_bridges[13].func = bridge_GetPixel_00B5043C;
    MEM32(0x00B50440) = 0xBB00000Fu;
    g_import_bridges[14].address = 0xBB00000Fu;
    g_import_bridges[14].func = bridge_DeleteDC_00B50440;
    MEM32(0x00B50444) = 0xBB000010u;
    g_import_bridges[15].address = 0xBB000010u;
    g_import_bridges[15].func = bridge_GetObjectA_00B50444;
    MEM32(0x00B50448) = 0xBB000011u;
    g_import_bridges[16].address = 0xBB000011u;
    g_import_bridges[16].func = bridge_SetTextCharacterExtra_00B50448;
    MEM32(0x00B5044C) = 0xBB000012u;
    g_import_bridges[17].address = 0xBB000012u;
    g_import_bridges[17].func = bridge_GetTextExtentPoint32A_00B5044C;
    MEM32(0x00B50450) = 0xBB000013u;
    g_import_bridges[18].address = 0xBB000013u;
    g_import_bridges[18].func = bridge_CreateFontA_00B50450;
    MEM32(0x00B50454) = 0xBB000014u;
    g_import_bridges[19].address = 0xBB000014u;
    g_import_bridges[19].func = bridge_SetTextColor_00B50454;
    MEM32(0x00B50458) = 0xBB000015u;
    g_import_bridges[20].address = 0xBB000015u;
    g_import_bridges[20].func = bridge_SetBkColor_00B50458;
    MEM32(0x00B5045C) = 0xBB000016u;
    g_import_bridges[21].address = 0xBB000016u;
    g_import_bridges[21].func = bridge_SetMapMode_00B5045C;
    MEM32(0x00B50460) = 0xBB000017u;
    g_import_bridges[22].address = 0xBB000017u;
    g_import_bridges[22].func = bridge_ExtTextOutA_00B50460;
    MEM32(0x00B50464) = 0xBB000018u;
    g_import_bridges[23].address = 0xBB000018u;
    g_import_bridges[23].func = bridge_SelectObject_00B50464;
    MEM32(0x00B50468) = 0xBB000019u;
    g_import_bridges[24].address = 0xBB000019u;
    g_import_bridges[24].func = bridge_SetBkMode_00B50468;
    MEM32(0x00B5046C) = 0xBB00001Au;
    g_import_bridges[25].address = 0xBB00001Au;
    g_import_bridges[25].func = bridge_GetStockObject_00B5046C;
    MEM32(0x00B50470) = 0xBB00001Bu;
    g_import_bridges[26].address = 0xBB00001Bu;
    g_import_bridges[26].func = bridge_DeleteObject_00B50470;
    MEM32(0x00B50478) = 0xBB00001Cu;
    g_import_bridges[27].address = 0xBB00001Cu;
    g_import_bridges[27].func = bridge__lclose_00B50478;
    MEM32(0x00B5047C) = 0xBB00001Du;
    g_import_bridges[28].address = 0xBB00001Du;
    g_import_bridges[28].func = bridge__lread_00B5047C;
    MEM32(0x00B50480) = 0xBB00001Eu;
    g_import_bridges[29].address = 0xBB00001Eu;
    g_import_bridges[29].func = bridge__lopen_00B50480;
    MEM32(0x00B50484) = 0xBB00001Fu;
    g_import_bridges[30].address = 0xBB00001Fu;
    g_import_bridges[30].func = bridge_GetModuleHandleA_00B50484;
    MEM32(0x00B50488) = 0xBB000020u;
    g_import_bridges[31].address = 0xBB000020u;
    g_import_bridges[31].func = bridge_FindFirstFileA_00B50488;
    MEM32(0x00B5048C) = 0xBB000021u;
    g_import_bridges[32].address = 0xBB000021u;
    g_import_bridges[32].func = bridge_LockResource_00B5048C;
    MEM32(0x00B50490) = 0xBB000022u;
    g_import_bridges[33].address = 0xBB000022u;
    g_import_bridges[33].func = bridge_FindResourceA_00B50490;
    MEM32(0x00B50494) = 0xBB000023u;
    g_import_bridges[34].address = 0xBB000023u;
    g_import_bridges[34].func = bridge_GetVolumeInformationA_00B50494;
    MEM32(0x00B50498) = 0xBB000024u;
    g_import_bridges[35].address = 0xBB000024u;
    g_import_bridges[35].func = bridge_EnterCriticalSection_00B50498;
    MEM32(0x00B5049C) = 0xBB000025u;
    g_import_bridges[36].address = 0xBB000025u;
    g_import_bridges[36].func = bridge_LeaveCriticalSection_00B5049C;
    MEM32(0x00B504A0) = 0xBB000026u;
    g_import_bridges[37].address = 0xBB000026u;
    g_import_bridges[37].func = bridge_ResumeThread_00B504A0;
    MEM32(0x00B504A4) = 0xBB000027u;
    g_import_bridges[38].address = 0xBB000027u;
    g_import_bridges[38].func = bridge_LoadResource_00B504A4;
    MEM32(0x00B504A8) = 0xBB000028u;
    g_import_bridges[39].address = 0xBB000028u;
    g_import_bridges[39].func = bridge_InitializeCriticalSection_00B504A8;
    MEM32(0x00B504AC) = 0xBB000029u;
    g_import_bridges[40].address = 0xBB000029u;
    g_import_bridges[40].func = bridge_DeleteCriticalSection_00B504AC;
    MEM32(0x00B504B0) = 0xBB00002Au;
    g_import_bridges[41].address = 0xBB00002Au;
    g_import_bridges[41].func = bridge_CloseHandle_00B504B0;
    MEM32(0x00B504B4) = 0xBB00002Bu;
    g_import_bridges[42].address = 0xBB00002Bu;
    g_import_bridges[42].func = bridge_TerminateThread_00B504B4;
    MEM32(0x00B504B8) = 0xBB00002Cu;
    g_import_bridges[43].address = 0xBB00002Cu;
    g_import_bridges[43].func = bridge_SetThreadPriority_00B504B8;
    MEM32(0x00B504BC) = 0xBB00002Du;
    g_import_bridges[44].address = 0xBB00002Du;
    g_import_bridges[44].func = bridge_SuspendThread_00B504BC;
    MEM32(0x00B504C0) = 0xBB00002Eu;
    g_import_bridges[45].address = 0xBB00002Eu;
    g_import_bridges[45].func = bridge_CompareStringW_00B504C0;
    MEM32(0x00B504C4) = 0xBB00002Fu;
    g_import_bridges[46].address = 0xBB00002Fu;
    g_import_bridges[46].func = bridge_CompareStringA_00B504C4;
    MEM32(0x00B504C8) = 0xBB000030u;
    g_import_bridges[47].address = 0xBB000030u;
    g_import_bridges[47].func = bridge_SetEndOfFile_00B504C8;
    MEM32(0x00B504CC) = 0xBB000031u;
    g_import_bridges[48].address = 0xBB000031u;
    g_import_bridges[48].func = bridge_GetLocaleInfoW_00B504CC;
    MEM32(0x00B504D0) = 0xBB000032u;
    g_import_bridges[49].address = 0xBB000032u;
    g_import_bridges[49].func = bridge_GetLocaleInfoA_00B504D0;
    MEM32(0x00B504D4) = 0xBB000033u;
    g_import_bridges[50].address = 0xBB000033u;
    g_import_bridges[50].func = bridge_CreateFileA_00B504D4;
    MEM32(0x00B504D8) = 0xBB000034u;
    g_import_bridges[51].address = 0xBB000034u;
    g_import_bridges[51].func = bridge_GetThreadPriority_00B504D8;
    MEM32(0x00B504DC) = 0xBB000035u;
    g_import_bridges[52].address = 0xBB000035u;
    g_import_bridges[52].func = bridge_GetTickCount_00B504DC;
    MEM32(0x00B504E0) = 0xBB000036u;
    g_import_bridges[53].address = 0xBB000036u;
    g_import_bridges[53].func = bridge_GetStringTypeW_00B504E0;
    MEM32(0x00B504E4) = 0xBB000037u;
    g_import_bridges[54].address = 0xBB000037u;
    g_import_bridges[54].func = bridge_GetStringTypeA_00B504E4;
    MEM32(0x00B504E8) = 0xBB000038u;
    g_import_bridges[55].address = 0xBB000038u;
    g_import_bridges[55].func = bridge_LCMapStringW_00B504E8;
    MEM32(0x00B504EC) = 0xBB000039u;
    g_import_bridges[56].address = 0xBB000039u;
    g_import_bridges[56].func = bridge_LCMapStringA_00B504EC;
    MEM32(0x00B504F0) = 0xBB00003Au;
    g_import_bridges[57].address = 0xBB00003Au;
    g_import_bridges[57].func = bridge_GetOEMCP_00B504F0;
    MEM32(0x00B504F4) = 0xBB00003Bu;
    g_import_bridges[58].address = 0xBB00003Bu;
    g_import_bridges[58].func = bridge_GetACP_00B504F4;
    MEM32(0x00B504F8) = 0xBB00003Cu;
    g_import_bridges[59].address = 0xBB00003Cu;
    g_import_bridges[59].func = bridge_WideCharToMultiByte_00B504F8;
    MEM32(0x00B504FC) = 0xBB00003Du;
    g_import_bridges[60].address = 0xBB00003Du;
    g_import_bridges[60].func = bridge_GetEnvironmentStringsW_00B504FC;
    MEM32(0x00B50500) = 0xBB00003Eu;
    g_import_bridges[61].address = 0xBB00003Eu;
    g_import_bridges[61].func = bridge_FreeEnvironmentStringsW_00B50500;
    MEM32(0x00B50504) = 0xBB00003Fu;
    g_import_bridges[62].address = 0xBB00003Fu;
    g_import_bridges[62].func = bridge_GetEnvironmentStrings_00B50504;
    MEM32(0x00B50508) = 0xBB000040u;
    g_import_bridges[63].address = 0xBB000040u;
    g_import_bridges[63].func = bridge_MultiByteToWideChar_00B50508;
    MEM32(0x00B5050C) = 0xBB000041u;
    g_import_bridges[64].address = 0xBB000041u;
    g_import_bridges[64].func = bridge_FreeEnvironmentStringsA_00B5050C;
    MEM32(0x00B50510) = 0xBB000042u;
    g_import_bridges[65].address = 0xBB000042u;
    g_import_bridges[65].func = bridge_UnhandledExceptionFilter_00B50510;
    MEM32(0x00B50514) = 0xBB000043u;
    g_import_bridges[66].address = 0xBB000043u;
    g_import_bridges[66].func = bridge_RtlUnwind_00B50514;
    MEM32(0x00B50518) = 0xBB000044u;
    g_import_bridges[67].address = 0xBB000044u;
    g_import_bridges[67].func = bridge_GetUserDefaultLCID_00B50518;
    MEM32(0x00B5051C) = 0xBB000045u;
    g_import_bridges[68].address = 0xBB000045u;
    g_import_bridges[68].func = bridge_IsValidCodePage_00B5051C;
    MEM32(0x00B50520) = 0xBB000046u;
    g_import_bridges[69].address = 0xBB000046u;
    g_import_bridges[69].func = bridge_IsValidLocale_00B50520;
    MEM32(0x00B50524) = 0xBB000047u;
    g_import_bridges[70].address = 0xBB000047u;
    g_import_bridges[70].func = bridge_GetCPInfo_00B50524;
    MEM32(0x00B50528) = 0xBB000048u;
    g_import_bridges[71].address = 0xBB000048u;
    g_import_bridges[71].func = bridge_GetFileType_00B50528;
    MEM32(0x00B5052C) = 0xBB000049u;
    g_import_bridges[72].address = 0xBB000049u;
    g_import_bridges[72].func = bridge_SetHandleCount_00B5052C;
    MEM32(0x00B50530) = 0xBB00004Au;
    g_import_bridges[73].address = 0xBB00004Au;
    g_import_bridges[73].func = bridge_ReadFile_00B50530;
    MEM32(0x00B50534) = 0xBB00004Bu;
    g_import_bridges[74].address = 0xBB00004Bu;
    g_import_bridges[74].func = bridge_FlushFileBuffers_00B50534;
    MEM32(0x00B50538) = 0xBB00004Cu;
    g_import_bridges[75].address = 0xBB00004Cu;
    g_import_bridges[75].func = bridge_DebugBreak_00B50538;
    MEM32(0x00B5053C) = 0xBB00004Du;
    g_import_bridges[76].address = 0xBB00004Du;
    g_import_bridges[76].func = bridge_GetCurrentThread_00B5053C;
    MEM32(0x00B50540) = 0xBB00004Eu;
    g_import_bridges[77].address = 0xBB00004Eu;
    g_import_bridges[77].func = bridge_TlsGetValue_00B50540;
    MEM32(0x00B50544) = 0xBB00004Fu;
    g_import_bridges[78].address = 0xBB00004Fu;
    g_import_bridges[78].func = bridge_SetLastError_00B50544;
    MEM32(0x00B50548) = 0xBB000050u;
    g_import_bridges[79].address = 0xBB000050u;
    g_import_bridges[79].func = bridge_FindNextFileA_00B50548;
    MEM32(0x00B5054C) = 0xBB000051u;
    g_import_bridges[80].address = 0xBB000051u;
    g_import_bridges[80].func = bridge_FindClose_00B5054C;
    MEM32(0x00B50550) = 0xBB000052u;
    g_import_bridges[81].address = 0xBB000052u;
    g_import_bridges[81].func = bridge_GlobalHandle_00B50550;
    MEM32(0x00B50554) = 0xBB000053u;
    g_import_bridges[82].address = 0xBB000053u;
    g_import_bridges[82].func = bridge_GlobalAlloc_00B50554;
    MEM32(0x00B50558) = 0xBB000054u;
    g_import_bridges[83].address = 0xBB000054u;
    g_import_bridges[83].func = bridge_GlobalLock_00B50558;
    MEM32(0x00B5055C) = 0xBB000055u;
    g_import_bridges[84].address = 0xBB000055u;
    g_import_bridges[84].func = bridge_GetLogicalDriveStringsA_00B5055C;
    MEM32(0x00B50560) = 0xBB000056u;
    g_import_bridges[85].address = 0xBB000056u;
    g_import_bridges[85].func = bridge_GlobalUnlock_00B50560;
    MEM32(0x00B50564) = 0xBB000057u;
    g_import_bridges[86].address = 0xBB000057u;
    g_import_bridges[86].func = bridge_GlobalFree_00B50564;
    MEM32(0x00B50568) = 0xBB000058u;
    g_import_bridges[87].address = 0xBB000058u;
    g_import_bridges[87].func = bridge_VirtualProtect_00B50568;
    MEM32(0x00B5056C) = 0xBB000059u;
    g_import_bridges[88].address = 0xBB000059u;
    g_import_bridges[88].func = bridge_GetDriveTypeA_00B5056C;
    MEM32(0x00B50570) = 0xBB00005Au;
    g_import_bridges[89].address = 0xBB00005Au;
    g_import_bridges[89].func = bridge_Sleep_00B50570;
    MEM32(0x00B50574) = 0xBB00005Bu;
    g_import_bridges[90].address = 0xBB00005Bu;
    g_import_bridges[90].func = bridge_LocalAlloc_00B50574;
    MEM32(0x00B50578) = 0xBB00005Cu;
    g_import_bridges[91].address = 0xBB00005Cu;
    g_import_bridges[91].func = bridge_OutputDebugStringA_00B50578;
    MEM32(0x00B5057C) = 0xBB00005Du;
    g_import_bridges[92].address = 0xBB00005Du;
    g_import_bridges[92].func = bridge_CreateProcessA_00B5057C;
    MEM32(0x00B50580) = 0xBB00005Eu;
    g_import_bridges[93].address = 0xBB00005Eu;
    g_import_bridges[93].func = bridge_SetStdHandle_00B50580;
    MEM32(0x00B50584) = 0xBB00005Fu;
    g_import_bridges[94].address = 0xBB00005Fu;
    g_import_bridges[94].func = bridge_SetConsoleCtrlHandler_00B50584;
    MEM32(0x00B50588) = 0xBB000060u;
    g_import_bridges[95].address = 0xBB000060u;
    g_import_bridges[95].func = bridge_SetFilePointer_00B50588;
    MEM32(0x00B5058C) = 0xBB000061u;
    g_import_bridges[96].address = 0xBB000061u;
    g_import_bridges[96].func = bridge_IsBadReadPtr_00B5058C;
    MEM32(0x00B50590) = 0xBB000062u;
    g_import_bridges[97].address = 0xBB000062u;
    g_import_bridges[97].func = bridge_HeapValidate_00B50590;
    MEM32(0x00B50594) = 0xBB000063u;
    g_import_bridges[98].address = 0xBB000063u;
    g_import_bridges[98].func = bridge_FatalAppExitA_00B50594;
    MEM32(0x00B50598) = 0xBB000064u;
    g_import_bridges[99].address = 0xBB000064u;
    g_import_bridges[99].func = bridge_ExitThread_00B50598;
    MEM32(0x00B5059C) = 0xBB000065u;
    g_import_bridges[100].address = 0xBB000065u;
    g_import_bridges[100].func = bridge_LocalFree_00B5059C;
    MEM32(0x00B505A0) = 0xBB000066u;
    g_import_bridges[101].address = 0xBB000066u;
    g_import_bridges[101].func = bridge_HeapAlloc_00B505A0;
    MEM32(0x00B505A4) = 0xBB000067u;
    g_import_bridges[102].address = 0xBB000067u;
    g_import_bridges[102].func = bridge_TlsFree_00B505A4;
    MEM32(0x00B505A8) = 0xBB000068u;
    g_import_bridges[103].address = 0xBB000068u;
    g_import_bridges[103].func = bridge_TlsAlloc_00B505A8;
    MEM32(0x00B505AC) = 0xBB000069u;
    g_import_bridges[104].address = 0xBB000069u;
    g_import_bridges[104].func = bridge_VirtualAlloc_00B505AC;
    MEM32(0x00B505B0) = 0xBB00006Au;
    g_import_bridges[105].address = 0xBB00006Au;
    g_import_bridges[105].func = bridge_VirtualFree_00B505B0;
    MEM32(0x00B505B4) = 0xBB00006Bu;
    g_import_bridges[106].address = 0xBB00006Bu;
    g_import_bridges[106].func = bridge_HeapCreate_00B505B4;
    MEM32(0x00B505B8) = 0xBB00006Cu;
    g_import_bridges[107].address = 0xBB00006Cu;
    g_import_bridges[107].func = bridge_HeapDestroy_00B505B8;
    MEM32(0x00B505BC) = 0xBB00006Du;
    g_import_bridges[108].address = 0xBB00006Du;
    g_import_bridges[108].func = bridge_HeapFree_00B505BC;
    MEM32(0x00B505C0) = 0xBB00006Eu;
    g_import_bridges[109].address = 0xBB00006Eu;
    g_import_bridges[109].func = bridge_HeapReAlloc_00B505C0;
    MEM32(0x00B505C4) = 0xBB00006Fu;
    g_import_bridges[110].address = 0xBB00006Fu;
    g_import_bridges[110].func = bridge_GetModuleFileNameA_00B505C4;
    MEM32(0x00B505C8) = 0xBB000070u;
    g_import_bridges[111].address = 0xBB000070u;
    g_import_bridges[111].func = bridge_InterlockedIncrement_00B505C8;
    MEM32(0x00B505CC) = 0xBB000071u;
    g_import_bridges[112].address = 0xBB000071u;
    g_import_bridges[112].func = bridge_LoadLibraryA_00B505CC;
    MEM32(0x00B505D0) = 0xBB000072u;
    g_import_bridges[113].address = 0xBB000072u;
    g_import_bridges[113].func = bridge_GetProcAddress_00B505D0;
    MEM32(0x00B505D4) = 0xBB000073u;
    g_import_bridges[114].address = 0xBB000073u;
    g_import_bridges[114].func = bridge_WriteFile_00B505D4;
    MEM32(0x00B505D8) = 0xBB000074u;
    g_import_bridges[115].address = 0xBB000074u;
    g_import_bridges[115].func = bridge_InterlockedDecrement_00B505D8;
    MEM32(0x00B505DC) = 0xBB000075u;
    g_import_bridges[116].address = 0xBB000075u;
    g_import_bridges[116].func = bridge_GetStdHandle_00B505DC;
    MEM32(0x00B505E0) = 0xBB000076u;
    g_import_bridges[117].address = 0xBB000076u;
    g_import_bridges[117].func = bridge_IsBadWritePtr_00B505E0;
    MEM32(0x00B505E4) = 0xBB000077u;
    g_import_bridges[118].address = 0xBB000077u;
    g_import_bridges[118].func = bridge_GetStartupInfoA_00B505E4;
    MEM32(0x00B505E8) = 0xBB000078u;
    g_import_bridges[119].address = 0xBB000078u;
    g_import_bridges[119].func = bridge_DeleteFileA_00B505E8;
    MEM32(0x00B505EC) = 0xBB000079u;
    g_import_bridges[120].address = 0xBB000079u;
    g_import_bridges[120].func = bridge_GetTimeZoneInformation_00B505EC;
    MEM32(0x00B505F0) = 0xBB00007Au;
    g_import_bridges[121].address = 0xBB00007Au;
    g_import_bridges[121].func = bridge_GetSystemTime_00B505F0;
    MEM32(0x00B505F4) = 0xBB00007Bu;
    g_import_bridges[122].address = 0xBB00007Bu;
    g_import_bridges[122].func = bridge_GetLocalTime_00B505F4;
    MEM32(0x00B505F8) = 0xBB00007Cu;
    g_import_bridges[123].address = 0xBB00007Cu;
    g_import_bridges[123].func = bridge_ExitProcess_00B505F8;
    MEM32(0x00B505FC) = 0xBB00007Du;
    g_import_bridges[124].address = 0xBB00007Du;
    g_import_bridges[124].func = bridge_TerminateProcess_00B505FC;
    MEM32(0x00B50600) = 0xBB00007Eu;
    g_import_bridges[125].address = 0xBB00007Eu;
    g_import_bridges[125].func = bridge_GetCurrentProcess_00B50600;
    MEM32(0x00B50604) = 0xBB00007Fu;
    g_import_bridges[126].address = 0xBB00007Fu;
    g_import_bridges[126].func = bridge_GetLastError_00B50604;
    MEM32(0x00B50608) = 0xBB000080u;
    g_import_bridges[127].address = 0xBB000080u;
    g_import_bridges[127].func = bridge_GetCurrentThreadId_00B50608;
    MEM32(0x00B5060C) = 0xBB000081u;
    g_import_bridges[128].address = 0xBB000081u;
    g_import_bridges[128].func = bridge_TlsSetValue_00B5060C;
    MEM32(0x00B50610) = 0xBB000082u;
    g_import_bridges[129].address = 0xBB000082u;
    g_import_bridges[129].func = bridge_GetCommandLineA_00B50610;
    MEM32(0x00B50614) = 0xBB000083u;
    g_import_bridges[130].address = 0xBB000083u;
    g_import_bridges[130].func = bridge_GetVersion_00B50614;
    MEM32(0x00B50618) = 0xBB000084u;
    g_import_bridges[131].address = 0xBB000084u;
    g_import_bridges[131].func = bridge_SetEnvironmentVariableA_00B50618;
    MEM32(0x00B5061C) = 0xBB000085u;
    g_import_bridges[132].address = 0xBB000085u;
    g_import_bridges[132].func = bridge_GetCurrentDirectoryA_00B5061C;
    MEM32(0x00B50620) = 0xBB000086u;
    g_import_bridges[133].address = 0xBB000086u;
    g_import_bridges[133].func = bridge_SetCurrentDirectoryA_00B50620;
    MEM32(0x00B50624) = 0xBB000087u;
    g_import_bridges[134].address = 0xBB000087u;
    g_import_bridges[134].func = bridge_GetFullPathNameA_00B50624;
    MEM32(0x00B50628) = 0xBB000088u;
    g_import_bridges[135].address = 0xBB000088u;
    g_import_bridges[135].func = bridge_CreateThread_00B50628;
    MEM32(0x00B50630) = 0xBB000089u;
    g_import_bridges[136].address = 0xBB000089u;
    g_import_bridges[136].func = bridge_acmStreamUnprepareHeader_00B50630;
    MEM32(0x00B50634) = 0xBB00008Au;
    g_import_bridges[137].address = 0xBB00008Au;
    g_import_bridges[137].func = bridge_acmStreamPrepareHeader_00B50634;
    MEM32(0x00B50638) = 0xBB00008Bu;
    g_import_bridges[138].address = 0xBB00008Bu;
    g_import_bridges[138].func = bridge_acmStreamClose_00B50638;
    MEM32(0x00B5063C) = 0xBB00008Cu;
    g_import_bridges[139].address = 0xBB00008Cu;
    g_import_bridges[139].func = bridge_acmStreamSize_00B5063C;
    MEM32(0x00B50640) = 0xBB00008Du;
    g_import_bridges[140].address = 0xBB00008Du;
    g_import_bridges[140].func = bridge_acmStreamConvert_00B50640;
    MEM32(0x00B50648) = 0xBB00008Eu;
    g_import_bridges[141].address = 0xBB00008Eu;
    g_import_bridges[141].func = bridge_RegisterClassA_00B50648;
    MEM32(0x00B5064C) = 0xBB00008Fu;
    g_import_bridges[142].address = 0xBB00008Fu;
    g_import_bridges[142].func = bridge_GetSystemMetrics_00B5064C;
    MEM32(0x00B50650) = 0xBB000090u;
    g_import_bridges[143].address = 0xBB000090u;
    g_import_bridges[143].func = bridge_wsprintfA_00B50650;
    MEM32(0x00B50654) = 0xBB000091u;
    g_import_bridges[144].address = 0xBB000091u;
    g_import_bridges[144].func = bridge_SetFocus_00B50654;
    MEM32(0x00B50658) = 0xBB000092u;
    g_import_bridges[145].address = 0xBB000092u;
    g_import_bridges[145].func = bridge_MessageBoxA_00B50658;
    MEM32(0x00B5065C) = 0xBB000093u;
    g_import_bridges[146].address = 0xBB000093u;
    g_import_bridges[146].func = bridge_DispatchMessageA_00B5065C;
    MEM32(0x00B50660) = 0xBB000094u;
    g_import_bridges[147].address = 0xBB000094u;
    g_import_bridges[147].func = bridge_TranslateMessage_00B50660;
    MEM32(0x00B50664) = 0xBB000095u;
    g_import_bridges[148].address = 0xBB000095u;
    g_import_bridges[148].func = bridge_UpdateWindow_00B50664;
    MEM32(0x00B50668) = 0xBB000096u;
    g_import_bridges[149].address = 0xBB000096u;
    g_import_bridges[149].func = bridge_ShowCursor_00B50668;
    MEM32(0x00B5066C) = 0xBB000097u;
    g_import_bridges[150].address = 0xBB000097u;
    g_import_bridges[150].func = bridge_SetForegroundWindow_00B5066C;
    MEM32(0x00B50670) = 0xBB000098u;
    g_import_bridges[151].address = 0xBB000098u;
    g_import_bridges[151].func = bridge_PeekMessageA_00B50670;
    MEM32(0x00B50674) = 0xBB000099u;
    g_import_bridges[152].address = 0xBB000099u;
    g_import_bridges[152].func = bridge_DefWindowProcA_00B50674;
    MEM32(0x00B50678) = 0xBB00009Au;
    g_import_bridges[153].address = 0xBB00009Au;
    g_import_bridges[153].func = bridge_GetMessageA_00B50678;
    MEM32(0x00B5067C) = 0xBB00009Bu;
    g_import_bridges[154].address = 0xBB00009Bu;
    g_import_bridges[154].func = bridge_GetForegroundWindow_00B5067C;
    MEM32(0x00B50680) = 0xBB00009Cu;
    g_import_bridges[155].address = 0xBB00009Cu;
    g_import_bridges[155].func = bridge_GetCursorPos_00B50680;
    MEM32(0x00B50684) = 0xBB00009Du;
    g_import_bridges[156].address = 0xBB00009Du;
    g_import_bridges[156].func = bridge_LoadImageA_00B50684;
    MEM32(0x00B50688) = 0xBB00009Eu;
    g_import_bridges[157].address = 0xBB00009Eu;
    g_import_bridges[157].func = bridge_GetDC_00B50688;
    MEM32(0x00B5068C) = 0xBB00009Fu;
    g_import_bridges[158].address = 0xBB00009Fu;
    g_import_bridges[158].func = bridge_ReleaseDC_00B5068C;
    MEM32(0x00B50690) = 0xBB0000A0u;
    g_import_bridges[159].address = 0xBB0000A0u;
    g_import_bridges[159].func = bridge_SetRect_00B50690;
    MEM32(0x00B50694) = 0xBB0000A1u;
    g_import_bridges[160].address = 0xBB0000A1u;
    g_import_bridges[160].func = bridge_FindWindowA_00B50694;
    MEM32(0x00B50698) = 0xBB0000A2u;
    g_import_bridges[161].address = 0xBB0000A2u;
    g_import_bridges[161].func = bridge_ShowWindowAsync_00B50698;
    MEM32(0x00B5069C) = 0xBB0000A3u;
    g_import_bridges[162].address = 0xBB0000A3u;
    g_import_bridges[162].func = bridge_LoadIconA_00B5069C;
    MEM32(0x00B506A0) = 0xBB0000A4u;
    g_import_bridges[163].address = 0xBB0000A4u;
    g_import_bridges[163].func = bridge_LoadCursorA_00B506A0;
    MEM32(0x00B506A4) = 0xBB0000A5u;
    g_import_bridges[164].address = 0xBB0000A5u;
    g_import_bridges[164].func = bridge_MessageBeep_00B506A4;
    MEM32(0x00B506A8) = 0xBB0000A6u;
    g_import_bridges[165].address = 0xBB0000A6u;
    g_import_bridges[165].func = bridge_SetCursorPos_00B506A8;
    MEM32(0x00B506AC) = 0xBB0000A7u;
    g_import_bridges[166].address = 0xBB0000A7u;
    g_import_bridges[166].func = bridge_CreateWindowExA_00B506AC;
    MEM32(0x00B506B0) = 0xBB0000A8u;
    g_import_bridges[167].address = 0xBB0000A8u;
    g_import_bridges[167].func = bridge_GetKeyboardState_00B506B0;
    MEM32(0x00B506B4) = 0xBB0000A9u;
    g_import_bridges[168].address = 0xBB0000A9u;
    g_import_bridges[168].func = bridge_DestroyWindow_00B506B4;
    MEM32(0x00B506B8) = 0xBB0000AAu;
    g_import_bridges[169].address = 0xBB0000AAu;
    g_import_bridges[169].func = bridge_PostQuitMessage_00B506B8;
    MEM32(0x00B506BC) = 0xBB0000ABu;
    g_import_bridges[170].address = 0xBB0000ABu;
    g_import_bridges[170].func = bridge_SetCapture_00B506BC;
    MEM32(0x00B506C0) = 0xBB0000ACu;
    g_import_bridges[171].address = 0xBB0000ACu;
    g_import_bridges[171].func = bridge_ReleaseCapture_00B506C0;
    MEM32(0x00B506C4) = 0xBB0000ADu;
    g_import_bridges[172].address = 0xBB0000ADu;
    g_import_bridges[172].func = bridge_SetCursor_00B506C4;
    MEM32(0x00B506C8) = 0xBB0000AEu;
    g_import_bridges[173].address = 0xBB0000AEu;
    g_import_bridges[173].func = bridge_PostMessageA_00B506C8;
    MEM32(0x00B506D0) = 0xBB0000AFu;
    g_import_bridges[174].address = 0xBB0000AFu;
    g_import_bridges[174].func = bridge_auxGetNumDevs_00B506D0;
    MEM32(0x00B506D4) = 0xBB0000B0u;
    g_import_bridges[175].address = 0xBB0000B0u;
    g_import_bridges[175].func = bridge_auxGetVolume_00B506D4;
    MEM32(0x00B506D8) = 0xBB0000B1u;
    g_import_bridges[176].address = 0xBB0000B1u;
    g_import_bridges[176].func = bridge_auxGetDevCapsA_00B506D8;
    MEM32(0x00B506DC) = 0xBB0000B2u;
    g_import_bridges[177].address = 0xBB0000B2u;
    g_import_bridges[177].func = bridge_waveInClose_00B506DC;
    MEM32(0x00B506E0) = 0xBB0000B3u;
    g_import_bridges[178].address = 0xBB0000B3u;
    g_import_bridges[178].func = bridge_mciSendCommandA_00B506E0;
    MEM32(0x00B506E4) = 0xBB0000B4u;
    g_import_bridges[179].address = 0xBB0000B4u;
    g_import_bridges[179].func = bridge_auxSetVolume_00B506E4;
    MEM32(0x00B506E8) = 0xBB0000B5u;
    g_import_bridges[180].address = 0xBB0000B5u;
    g_import_bridges[180].func = bridge_waveInStop_00B506E8;
    MEM32(0x00B506EC) = 0xBB0000B6u;
    g_import_bridges[181].address = 0xBB0000B6u;
    g_import_bridges[181].func = bridge_waveInReset_00B506EC;
    MEM32(0x00B506F0) = 0xBB0000B7u;
    g_import_bridges[182].address = 0xBB0000B7u;
    g_import_bridges[182].func = bridge_waveInStart_00B506F0;
    MEM32(0x00B506F4) = 0xBB0000B8u;
    g_import_bridges[183].address = 0xBB0000B8u;
    g_import_bridges[183].func = bridge_joyGetPosEx_00B506F4;
    MEM32(0x00B506F8) = 0xBB0000B9u;
    g_import_bridges[184].address = 0xBB0000B9u;
    g_import_bridges[184].func = bridge_waveInAddBuffer_00B506F8;
    MEM32(0x00B506FC) = 0xBB0000BAu;
    g_import_bridges[185].address = 0xBB0000BAu;
    g_import_bridges[185].func = bridge_timeGetTime_00B506FC;
    MEM32(0x00B50700) = 0xBB0000BBu;
    g_import_bridges[186].address = 0xBB0000BBu;
    g_import_bridges[186].func = bridge_joyGetDevCapsA_00B50700;
    MEM32(0x00B50704) = 0xBB0000BCu;
    g_import_bridges[187].address = 0xBB0000BCu;
    g_import_bridges[187].func = bridge_joyGetNumDevs_00B50704;
    g_import_bridge_count = 188;
    printf("[*] Registered %d import bridges\n", g_import_bridge_count);
}
