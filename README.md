# X-Wing vs TIE Fighter Static Recompilation

A static recompilation of **Star Wars: X-Wing vs TIE Fighter** (1997) and its **Balance of Power** expansion by Totally Games / LucasArts, targeting modern Windows with native x86 execution and a modern graphics pipeline.

XvT is the **direct predecessor of the [X-Wing Alliance](https://github.com/sp00nznet/xwa) engine** — XWA (1999) is built on the same Totally Games flight engine. This project exists primarily to **cross-validate the XWA recompilation toolchain and engine findings** against the sister title: reproducing the same methodology on the older, simpler codebase confirms the tooling is general (not XWA-specific), and the shared engine internals (object/flight-group model, the DirectPlay single-player session/create subsystem, the software rasterizer) are often clearer here.

## Project Status

| Phase | Status | Description |
|-------|--------|-------------|
| **Phase 0** | **In Progress** | Media acquisition & extraction — retail + multiplayer ISOs and Balance of Power on disk; installed tree not yet extracted |
| Phase 1 | Pending | Binary analysis — PE parsing, section mapping, import table (per-exe) |
| Phase 2 | Pending | Copy-protection handling & decrypted memory dump (if applicable) |
| Phase 3 | Pending | Function discovery (Capstone linear sweep + function finder) |
| Phase 4 | Pending | x86-to-C code generation (reuse XWA lifter/generator) |
| Phase 5 | Pending | Compilation and linking (MSVC 2022, x86) |
| Phase 6 | Pending | Runtime bringup — CRT init, import bridging, game startup |
| Phase 7 | Pending | Win32/DirectX HAL — COM mocks, main loop |
| Phase 8 | Pending | Rendering backend — 2D surface pipeline, 3D flight |
| Phase 9 | Pending | Cross-validation report vs XWA engine findings |

## Why This Repo — Cross-Validation with XWA

The [XWA recomp](https://github.com/sp00nznet/xwa) has "drilled down plenty": full boot, concourse rendering, and a complete instruction-level map of the flight-entry path. It reaches 3D flight but the flight view is black because the **single-player DirectPlay session / per-flight-group craft-create subsystem** is not yet reimplemented — mission craft are instantiated by a session-gated, message-driven loopback that the force-launch path bypasses.

XvT runs the **same engine family, two years earlier**, with a smaller executable and (typically) a cleaner separation of the same subsystems. Recompiling it lets us:

- **Confirm reproducibility** — the same toolchain (`pe_analyze` → `disasm` → `lifter` → `generate`) should recompile a second Totally Games title with only per-binary config changes. If it doesn't, our XWA success relied on target-specific hacks.
- **Validate engine findings** — cross-check the object/flight-group record layout, the FG→craft-type mapping (`FG+2`), and especially the **DP session-establish + create-message flow** against XvT's implementation, where the multiplayer-first design of 1997 may expose the session/create path more explicitly.
- **Illuminate the XWA blocker** — the exact `sub_004E7A10` (create broadcast) / `sub_004F91C0` (receive dispatch) / session-flag (`0x77330C`) mechanics that gate XWA craft instantiation almost certainly have simpler analogues here.

See the XWA repo's `memory/` notes (#48–75) for the full flight-entry create-pipeline map this project aims to corroborate.

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

## Target Media (Phase 0)

Present on disk under `G:\recomp\pc\xwingvstiefighter\` (not committed — see `.gitignore`):

| File | Size | Notes |
|------|------|-------|
| `Star Wars X-Wing vs Tie Fighter - Master Game Disc (LucasArts) (1997).iso` | ~154 MB | Retail single-disc image |
| `Star Wars X-Wing vs Tie Fighter - Multiplayer Disc (LucasArts) (1997).iso` | ~156 MB | Multiplayer disc |
| `Disc.rar` | ~437 MB | Archived disc set (incl. Balance of Power) |

**Next step:** extract/install to obtain the executable(s). XvT's primary executable and the Balance of Power binary, their compiler, image base, section layout, and any copy protection are recorded in *Binary Analysis* once extraction completes — this README intentionally does **not** guess those values.

## Planned Methodology (mirrors XWA)

Reuse the [XWA toolchain](https://github.com/sp00nznet/xwa/tree/main/tools) unchanged where possible:

```
xvt-recomp/
├── tools/                      # Python toolchain (shared with XWA where possible)
│   ├── pe_analyze.py           # PE header/section/import parser
│   ├── disasm.py               # Capstone x86 disassembler + function finder
│   ├── lifter.py               # x86 → C lifter
│   ├── generate.py             # Linear-sweep code generator
│   └── dump_memory.py          # Runtime decryption dumper (if protected)
├── src/
│   ├── game/
│   │   ├── main.c              # Entry, VEH handler, memory setup, overrides
│   │   ├── imports.c           # Win32/DirectX import bridges
│   │   ├── com_mocks.c/.h      # COM mocks (DirectDraw/Direct3D/DirectInput/DirectSound/DirectPlay)
│   │   └── recomp/gen/         # Auto-generated code (gitignored)
│   └── hal/
│       └── d3d11_renderer.c    # D3D11 backend (execute-buffer translation)
├── config/                     # PE metadata, function list (per-exe)
├── CMakeLists.txt              # MSVC 2022 x86 build
├── CLAUDE.md                   # AI assistant project context
└── README.md                   # This file
```

The recompilation approach (global register model `g_eax`…`g_edi`/`g_esp`; identity-mapped memory via `MEM32`/`ADDR`; three-tier `RECOMP_ICALL` dispatch; pattern-matched flag conditions) is identical to XWA — see the [XWA README](https://github.com/sp00nznet/xwa#recompilation-approach). Divergences unique to XvT (older CRT, different DirectX minor version, protection scheme) are documented as they are found.

## Build Requirements

- **CMake** 3.20+
- **Visual Studio 2022** (MSVC, x86/Win32 target)
- **Python 3.10+** with `capstone`, `pefile`

*(Build instructions land once Phase 3+ produces compilable code.)*

## Legal

This project is for game-preservation purposes. You must own a legal copy of Star Wars: X-Wing vs TIE Fighter (and Balance of Power) to use this tool. **No copyrighted game assets, disc images, or executables are included in this repository** — the ISOs/RAR shown above are user-supplied and git-ignored.

## Related Projects

Part of the [sp00nznet](https://github.com/sp00nznet) recompilation collection:

- [xwa](https://github.com/sp00nznet/xwa) — X-Wing Alliance recomp (sister engine; primary reference for this project)
- [burnout3](https://github.com/sp00nznet/burnout3) — Xbox x86 recomp (x86-to-C lifter reference)
- [bw](https://github.com/sp00nznet/bw) — Black & White Win32 recomp (Win32 game patterns)
- [civ](https://github.com/sp00nznet/civ) — Civilization DOS recomp (16-bit x86 lifting)
