# X-Wing vs TIE Fighter Static Recompilation

## Project Overview
Static recompilation of Star Wars: X-Wing vs TIE Fighter (1997) and its Balance of Power expansion from the original Win32 PE32 x86 binaries into clean, compilable C that runs natively on modern Windows with a modern graphics backend.

**Primary purpose: cross-validate the [X-Wing Alliance recomp](https://github.com/sp00nznet/xwa).** XvT is the predecessor of the same Totally Games flight engine. Reproducing the XWA methodology here confirms the toolchain is general and lets us corroborate/clarify shared engine internals — especially the DirectPlay single-player session + per-flight-group craft-create subsystem that currently blocks visible 3D flight in XWA.

## Source Media
- Retail + multiplayer ISOs and a `Disc.rar` (Balance of Power) are on disk under the parent directory `G:\recomp\pc\xwingvstiefighter\`. They are git-ignored (user-supplied, copyrighted).
- **Not yet extracted.** Executable name(s), compiler, image base, section layout, and copy protection are TBD until Phase 0 extraction completes. Do NOT assume these values — record them in README "Binary Analysis" once known.

## Engine Lineage (shared with XWA)
- DirectDraw (2D surfaces), Direct3D Immediate Mode with execute buffers (DX5/6 era), DirectSound, DirectInput, DirectPlay.
- Flight rendering uses SOFTWARE rasterization writing the DirectDraw framebuffer (same as XWA).
- Object/flight-group model and the DirectPlay-driven craft instantiation are the key structures to compare against XWA memory notes #48–75.

## Toolchain (reuse XWA where possible)
```
python -m tools <decrypted.exe> --all -o src/game/recomp/gen
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

## Key Patterns (identical to XWA)
- Global register model: g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp (ebp local per function).
- Memory access: MEM8/16/32/MEMF macros over an identity-mapped region via ADDR(va).
- Indirect calls: RECOMP_ICALL 3-tier dispatch (manual overrides, auto table, import bridges).
- Callee-saved ebx/esi/edi auto-preserved by RECOMP_CALL/RECOMP_ICALL.
- Conditions pattern-matched from flag-setter to consumer (CMP_EQ, TEST_Z, etc.).
- COM mocks at 0xBBxxxxxx vtable markers.

## Build Requirements
- CMake 3.20+, Visual Studio 2022 (MSVC x86), Python 3.10+ with capstone, pefile.

## Status
Phase 0 (media extraction). See README.md for the phase table and the cross-validation plan.
