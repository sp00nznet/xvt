# X-Wing vs TIE Fighter Static Recompilation

## Project Overview
Static recompilation of Star Wars: X-Wing vs TIE Fighter (1997) and its Balance of Power expansion from the original Win32 PE32 x86 binaries into clean, compilable C that runs natively on modern Windows with a modern graphics backend.

**Primary purpose: cross-validate the [X-Wing Alliance recomp](https://github.com/sp00nznet/xwa).** XvT is the predecessor of the same Totally Games flight engine. Reproducing the XWA methodology here confirms the toolchain is general and lets us corroborate/clarify shared engine internals — especially the DirectPlay single-player session + per-flight-group craft-create subsystem that currently blocks visible 3D flight in XWA.

## Source Media
- Retail + multiplayer ISOs and a `Disc.rar` (Balance of Power) are on disk under the parent directory `G:\recomp\pc\xwingvstiefighter\`. They are git-ignored (user-supplied, copyrighted).
- Extracted to git-ignored `game_data/`: `Z_XVT__.EXE` (the recomp target, 1,369,088 B) and `XWINGTIE.EXE` (launcher).

## Target Binary — `Z_XVT__.EXE` (verified; full data in `config/pe_analysis.json`)
- x86 PE32, image base `0x00400000`, entry point `0x004FD970`, linker 4.20 (VC++ 4.x), **no copy protection**.
- Sections (VA / vsize / raw offset / raw size):
  `.text` 0x401000 / 0x113204 / 0x400 / 0x113400 —
  `.rdata` 0x515000 / 0x1D89 / 0x113800 / 0x1E00 —
  `.data` 0x517000 / 0x638BE0 / 0x115600 / 0x12800 —
  `.idata` 0xB50000 / 0x13C4 / 0x127E00 / 0x1400.
- 188 imports from 10 DLLs; IAT starts at `0x00B503F4`.

## Engine Lineage (shared with XWA)
- DirectDraw (2D surfaces), Direct3D Immediate Mode with execute buffers (DX5/6 era), DirectSound, DirectInput, DirectPlay.
- Flight rendering uses SOFTWARE rasterization writing the DirectDraw framebuffer (same as XWA).
- Object/flight-group model and the DirectPlay-driven craft instantiation are the key structures to compare against XWA memory notes #48–75.

## Toolchain (vendored from XWA into `tools/`, with fixes — see docs/roadmap.md Phase 5)
```
python -m tools game_data/Z_XVT__.EXE --all -o src/game/recomp/gen --split 1000 \
    --functions-json config/functions.json     # ~35 min; regenerates all lifted C
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
./build/Release/xvt_recomp.exe game_data/Z_XVT__.EXE
```
- `tools/gen_bridges.py config/pe_analysis.json src/game/imports.c` bootstraps
  `imports.c`, which is then **hand-maintained** (DirectX creates route to COM
  mocks) — do not regenerate over hand edits.
- `tools/lift_extra.py <exe> 0xADDR...` lifts individual missed functions.
- `XVT_ENTRY=0x...` overrides the guest entry point (default: PE entry 0x4FD970).

## Key Patterns (identical to XWA)
- Global register model: g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp (ebp local per function).
- Memory access: MEM8/16/32/MEMF macros over an identity-mapped region via ADDR(va).
- Indirect calls: RECOMP_ICALL 3-tier dispatch (manual overrides, auto table, import bridges).
- Callee-saved ebx/esi/edi auto-preserved by RECOMP_CALL/RECOMP_ICALL.
- Conditions pattern-matched from flag-setter to consumer (CMP_EQ, TEST_Z, etc.).
- COM mocks at 0xBBxxxxxx vtable markers.

## Build Requirements
- CMake 3.20+, Visual Studio 2022 (MSVC x86), Python 3.10+ with capstone, pefile.

## Memory Layout (differs from XWA — do NOT copy XWA's constants)
- Guest image range `0x00400000-0x00B60000` is committed *inside* the default
  heap's existing reservation; a plain `VirtualAlloc` reserve there fails (err 487).
- The guest stack is **OS-placed**, not at 0x100000: this process's own mapped
  sections occupy XvT's original low addresses. Identity mapping (`g_mem_base=0`)
  makes any host address a valid guest stack address.

## Status
Phase 5 complete — builds, links, and executes guest CRT startup. See
`docs/roadmap.md` for phases 6-10 and the deferral list; README.md for the
phase table and the cross-validation plan.
