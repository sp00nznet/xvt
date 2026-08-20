# X-Wing vs TIE Fighter Static Recompilation

A static recompilation of **Star Wars: X-Wing vs TIE Fighter** (1997) and its **Balance of Power** expansion by Totally Games / LucasArts, targeting modern Windows with native x86 execution and a modern graphics pipeline.

XvT is the **direct predecessor of the [X-Wing Alliance](https://github.com/sp00nznet/xwa) engine** — XWA (1999) is built on the same Totally Games flight engine. This project exists primarily to **cross-validate the XWA recompilation toolchain and engine findings** against the sister title: reproducing the same methodology on the older, simpler codebase confirms the tooling is general (not XWA-specific), and the shared engine internals (object/flight-group model, the DirectPlay single-player session/create subsystem, the software rasterizer) are often clearer here.

## Project Status

| Phase | Status | Description |
|-------|--------|-------------|
| **Phase 0** | **Complete** | Media acquisition & extraction — `Z_XVT__.EXE` (1.34 MB) + `XWINGTIE.EXE` (launcher) extracted from the Master Game Disc ISO |
| **Phase 1** | **Complete** | Binary analysis — PE parsed with the XWA toolchain: x86, base 0x400000, **no SafeDisc**, DirectPlay-heavy imports |
| **Phase 2** | **Complete (N/A)** | No copy protection — `.text` is directly analyzable, no runtime decryption dump needed |
| **Phase 3** | **Complete** | Function discovery — 1,787 with the unmodified XWA finder; **1,790** after fixing the finder (call-target closure, plus one data-referenced WndProc lifted by hand) |
| **Phase 4** | **Complete** | x86-to-C code generation — **1,790 functions, ~364K lines of C**. The XWA lifter needed no *target-specific* changes, but running the result exposed real lifter bugs (see below) |
| **Phase 5** | **Complete** | Compilation and linking — CMake (MSVC 2022, x86) + `main.c` + generated `imports.c`; **`xvt_recomp.exe` builds, links, and executes recompiled guest code**. Required four fixes to the shared lifter/disassembler (see below) |
| **Phase 6** | **Complete** | Runtime bringup — **the guest VC4.x CRT startup runs unmodified into `WinMain`**; a window is created and real Windows messages dispatch into recompiled code (2,924 calls / 1,114 icalls before the DirectDraw wall). Needed a native WndProc bridge and `GetProcAddress` native registration; needed **none** of XWA's CRT-global patching |
| Phase 7 | **Next** | DirectX HAL — COM mocks. Interface version measured rather than assumed: XvT drives **`IDirectDraw` v1** (`SetDisplayMode` takes 3 args), 640×480×8bpp fullscreen-exclusive. XWA's mocks are `IDirectDraw4` — reuse their method bodies, not their slot indices |
| Phase 8 | Pending | 2D presentation — DirectDraw surface → window, 8-bit palette, input |
| Phase 9 | Pending | Flight — `.tie` load → local craft spawn (`sub_00409390`) → software raster |
| Phase 10 | Pending | Cross-validation report vs XWA engine findings |

See **[`docs/roadmap.md`](docs/roadmap.md)** for each phase's exit criterion and the deferral list.

### Where the build currently gets to

```
$ ./build/Release/xvt_recomp.exe game_data/Z_XVT__.EXE
=== X-Wing vs TIE Fighter Static Recompilation ===
[*] Dispatch table: 1790 functions
[*] Mapping guest image range 0x00400000-0x00B60000 (7552 KB)
[*] Guest stack at 03FC0000-047C0000
[*] Loaded .text  VA 0x00401000  1127424/1127424 bytes
[*] Loaded .rdata VA 0x00515000  7680/7680 bytes
[*] Loaded .data  VA 0x00517000  75776/75776 bytes
[*] Loaded .idata VA 0x00B50000  5120/5120 bytes
[*] Registered 188 import bridges
[*] Entering guest at 0x004FD970 ...
[*] WndProc slot 0: guest 0x004C81E0 -> native 10001000
STUB: DDRAW.dll:DirectDrawCreate
```

Guest CRT startup → `WinMain` → `RegisterClassA` → `CreateWindowExA` → a real
window receiving real Windows messages, all dispatched back into recompiled
code. **2,924 recompiled calls and 1,114 indirect calls** before it stops at
`DirectDrawCreate`, which is still a stub (Phase 7).

## Why This Repo — Cross-Validation with XWA

The [XWA recomp](https://github.com/sp00nznet/xwa) has drilled down plenty: full boot, concourse rendering, screen-to-screen menu navigation, and a complete instruction-level map of the flight-entry path. It now **enters 3D flight and presents frames** — the remaining problem there is *ship geometry*: the craft in the scene are not coherently populated, so the vertex transform reads placeholder positions and the ships do not render as ships.

XvT runs the **same engine family, two years earlier**, with a smaller executable and a cleaner separation of the same subsystems. Recompiling it lets us:

- **Confirm reproducibility** — the same toolchain (`pe_analyze` → `disasm` → `lifter` → `generate`) should recompile a second Totally Games title with only per-binary config changes. If it doesn't, the XWA success relied on target-specific hacks. *(Confirmed: it does — and running the result surfaced eight genuine lifter bugs neither project would have found by compiling alone.)*
- **Validate engine findings** — cross-check the object/flight-group record layout, the FG→craft-type mapping (`FG+2`), and the craft-create flow against XvT's implementation.
- **Illuminate the XWA blocker** — XvT's single-player craft spawn is **local**, with no DirectPlay involved (`docs/directplay.md`, `docs/singleplayer-spawn.md`), and its per-craft field recipe — flight-group stride `0x23`, object record at `fg+0x1F` with stride `0xEC`, active tag `fg+0x1A = 2`, render status `obj+0x2CA ∈ {2,4}` — is the reference for what XWA's own local builder needs to write. That reframing came out of this repo, and it is the currently-live hypothesis for XWA's geometry problem.

## Engine Lineage

| | X-Wing vs TIE Fighter | X-Wing Alliance |
|---|---|---|
| **Release** | 1997 (+ Balance of Power) | 1999 |
| **Developer** | Totally Games / LucasArts | Totally Games / LucasArts |
| **Engine** | Totally Games flight engine (1st gen multiplayer) | Same engine, extended (single-player campaign) |
| **Graphics** | DirectDraw + Direct3D IM (execute buffers), software raster | Same |
| **Multiplayer** | DirectPlay (design core) | DirectPlay (also drives SP object create) |
| **Compiler** | Visual C++ (era: VC4.2/VC5 — confirm on extraction) | Visual C++ 6.0 |
| **Dev path** | `K:\...` (Totally Games convention) | `K:\XWA\dev\` |

## Target Media (Phase 0 — done)

You supply your own copy of the game. The retail **Master Game Disc** carries the
executables this project targets; extract them to a `game_data/` directory at the
repository root (git-ignored, along with any disc images):

| Executable | Size | Date | Role |
|------------|------|------|------|
| `Z_XVT__.EXE` | 1,369,088 B (1.34 MB) | 1997-04-15 | **Main game binary** (the recomp target) |
| `XWINGTIE.EXE` | 322,560 B | 1997-04-16 | Front-end launcher / loader |

Both sit in the disc root and copy out directly; no installer or unpacking step
is needed. The Multiplayer disc and the *Balance of Power* expansion are not
required for anything currently implemented.

## Binary Analysis — `Z_XVT__.EXE`

Parsed with the **unmodified XWA toolchain** (`pefile`), confirming the pipeline generalizes to a second Totally Games title:

| Property | Value | vs XWA |
|----------|-------|--------|
| **Architecture** | x86-32, PE32 | same |
| **Image base** | `0x00400000` | same |
| **Entry point** | `0x004FD970` | — |
| **Timestamp** | 1997-04-16 | 2 yrs older |
| **Linker version** | 4.20 (Visual C++ 4.x era) | XWA = VC6 |
| **Copy protection** | **None** — no SafeDisc, no `.bind`, `.text` directly analyzable | XWA = SafeDisc v1 (runtime decryption) |
| **Relocations** | `.reloc` present (relocatable) | XWA = none (fixed base) |
| **Code (.text)** | `0x401000`, vsize `0x113204` (~1.1 MB) | XWA = ~1.7 MB |
| **Data (.data)** | `0x517000`, vsize `0x638BE0` (~6.2 MB) | XWA = ~5.4 MB |

### Import Summary

| DLL | Functions | Purpose |
|-----|-----------|---------|
| KERNEL32.dll | 109 | Core Win32 APIs |
| USER32.dll | 33 | Window management, input |
| GDI32.dll | 17 | Font/text rendering |
| WINMM.dll | 14 | Joystick, timers, CD audio |
| MSACM32.dll | 5 | Audio codec (ACM) — FMV/voice |
| ADVAPI32.dll | 4 | Registry (settings) |
| **DPLAYX.dll** | **3** | **DirectPlay (multiplayer) — 3× XWA's usage** |
| DDRAW.dll | 1 | DirectDraw |
| DINPUT.dll | 1 | DirectInput |
| DSOUND.dll | 1 | DirectSound |

### First cross-validation observations

1. **The toolchain reproduces — end to end, confirmed.** The full unmodified XWA pipeline (`pe_analyze` → `disasm`/function-finder → `lifter` → `generate`) ran on `Z_XVT__.EXE` with **zero target-specific changes** and produced **1,787 recompiled functions / ~364K lines of C**. XWA's success was not the result of XWA-only hacks — the same tools recompile a second Totally Games title as-is.
2. **XvT is *simpler* to recompile than XWA** — no SafeDisc means the `.text` needs no runtime decryption dump (`tools/dump_memory.py` is unnecessary), and `.reloc` gives us ground-truth for pointer sites.
3. **DirectPlay is more prominent** (3 imports vs XWA's 1), matching the multiplayer-first 1997 design. This is the payoff for the effort: the session/create subsystem that gates XWA craft instantiation (XWA memory #48–75) should surface here more directly, since in XvT the DirectPlay object-create path is the *primary* flow, not a single-player special case.

   *(Subsequent static analysis reframed this — see `docs/directplay.md`: XvT's DirectPlay is **multiplayer-only**, and single-player spawns craft through a local path, `sub_00409390`. That reframing is itself the cross-validation result, and it points at the XWA blocker from the other side.)*

4. **The toolchain had four general bugs, found by making XvT *run* rather than just compile.** Static output looked fine; execution did not. The most consequential: `shr`/`shl`/`sar` were not registered as flag setters, so any `jcc` following a shift silently matched the *previous* flag-setting instruction. In XvT's `memset` this inverted a branch and turned a 64 KB fill into a 4-billion-byte one on the first frame of CRT startup. The other three: `loop` targets were not basic-block leaders; `int3`-only blocks emitted a label with no statement; and discovered call targets were never fed back into function discovery. **All four are in XWA's generated code too** — regenerating XWA with the fixed toolchain is the first concrete finding flowing back, and it flows XvT → XWA exactly as this repo intended.

5. **XvT's CRT is materially easier than XWA's.** XWA had to bypass its VC6 CRT startup entirely (it crashed in locale/env init) and hand-initialise the small-block-heap threshold, the CRT lock table, the heap handle and `_pctype` before calling `WinMain` directly. XvT's VC4.x CRT startup runs *unmodified* all the way into `WinMain` — no bypass, no CRT-global patching, and `g_manual_overrides[]` is still empty. Two years earlier really is two years simpler, which is the cross-validation thesis holding up.

6. **And the traffic runs both ways — XWA has since found four FPU bugs XvT still has.** While chasing garbage vertices in its flight view, the XWA effort traced them to the shared lifter: the x87 stack was modelled per-function (so any guest function returning a value in `st(0)` handed back a zeroed local — which made the CRT `__ftol` return 0 for *every* float→int conversion); `fstp st(i)` was off by one; `fxch` lifted to a no-op; and, most insidiously, **the `ST(i)` register IDs were hardcoded to Capstone 4's numbering** while both projects build against Capstone 5, so every FPU register operand indexed *negatively* into the stack array. XvT's output has all four. None has bitten yet — nothing before the DirectDraw wall touches the FPU — but flight math touches all of them, so they are queued as one batched fix ahead of Phase 9. See `docs/roadmap.md` → "Known issues, queued".

   Which is the cross-validation working as designed, in both directions: XvT found four lifter bugs by *running* where XWA had only compiled, and XWA found four more by getting far enough into 3D to see them.

## Methodology (mirrors XWA)

The [XWA toolchain](https://github.com/sp00nznet/xwa/tree/main/tools) is vendored into
`tools/` — copied verbatim, then fixed where running XvT exposed general bugs
(the fixes apply to any target, XWA included):

```
recomp/
├── tools/                      # Python toolchain (vendored from XWA, + fixes)
│   ├── pe_analyze.py           # PE header/section/import parser
│   ├── disasm.py               # Capstone disassembler + function finder (recursive to closure)
│   ├── lifter.py               # x86 → C lifter
│   ├── translator.py           # Pipeline driver (`python -m tools`)
│   ├── gen_bridges.py          # Import-bridge generator (bootstrap only)
│   └── lift_extra.py           # Lift individual missed functions
├── src/game/
│   ├── main.c                  # Entry, memory setup, dispatch, VEH handler
│   ├── imports.c               # 188 Win32/DirectX import bridges (generated, then hand-maintained)
│   ├── com_mocks.c/.h          # COM mocks — Phase 7, not yet written
│   └── recomp/
│       ├── recomp_types.h      # Register/memory/dispatch model (from XWA, unchanged)
│       └── gen/                # Auto-generated lifted C (gitignored, ~364K lines)
├── config/                     # pe_analysis.json, functions.json
├── docs/                       # roadmap + engine analysis notes
├── CMakeLists.txt              # MSVC 2022 x86 build
├── LICENSE                     # MIT (this project's own source only)
└── README.md                   # This file
```

`dump_memory.py` is *not* vendored — XvT has no copy protection, so there is
nothing to dump. `d3d11_renderer.c` is deferred until the simpler present path
proves too slow.

The recompilation approach (global register model `g_eax`…`g_edi`/`g_esp`; identity-mapped memory via `MEM32`/`ADDR`; three-tier `RECOMP_ICALL` dispatch; pattern-matched flag conditions) is identical to XWA — see the [XWA README](https://github.com/sp00nznet/xwa#recompilation-approach). Divergences unique to XvT (older CRT, different DirectX minor version, protection scheme) are documented as they are found.

## Building

Requires **CMake** 3.20+, **Visual Studio 2022** (MSVC, x86/Win32 target), and
**Python 3.10+** with `capstone` and `pefile`.

The generated code under `src/game/recomp/gen/` is git-ignored, so a fresh clone
regenerates it first (~35 min; needs `game_data/Z_XVT__.EXE` from your own disc):

```sh
python -m tools game_data/Z_XVT__.EXE --all -o src/game/recomp/gen --split 1000 \
    --functions-json config/functions.json

cmake -B build -G "Visual Studio 17 2022" -A Win32     # 32-bit is required
cmake --build build --config Release

./build/Release/xvt_recomp.exe game_data/Z_XVT__.EXE
```

The executable links at `0x10000000` with ASLR disabled, so the guest's original
address range (`0x00400000`–`0x00B60000`) stays free to be mapped identically —
guest VAs are host addresses, which is what makes the lifted `MEM32(0x517108)`
style accesses work unmodified.

Set `XVT_ENTRY=0x4XXXXX` to enter the guest somewhere other than the PE entry
point (`0x004FD970`, the guest CRT startup).

## Legal

This project is for game-preservation purposes. You must own a legal copy of
Star Wars: X-Wing vs TIE Fighter to use it. **No copyrighted game assets, disc
images, or executables are included in this repository** — you supply your own,
and `game_data/` plus all disc-image formats are git-ignored.

The project's own source — the toolchain, runtime, and documentation — is
released under the [MIT License](LICENSE). That covers this repository's code
only; it grants no rights in the game itself, which remains the property of its
copyright holders. Star Wars and X-Wing vs TIE Fighter are trademarks of their
respective owners; this project is unaffiliated with and unendorsed by them.

## Related Projects

Part of the [sp00nznet](https://github.com/sp00nznet) recompilation collection:

- [xwa](https://github.com/sp00nznet/xwa) — X-Wing Alliance recomp (sister engine; primary reference for this project)
- [burnout3](https://github.com/sp00nznet/burnout3) — Xbox x86 recomp (x86-to-C lifter reference)
- [bw](https://github.com/sp00nznet/bw) — Black & White Win32 recomp (Win32 game patterns)
- [civ](https://github.com/sp00nznet/civ) — Civilization DOS recomp (16-bit x86 lifting)
