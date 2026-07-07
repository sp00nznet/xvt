# XvT DirectPlay Subsystem — Cross-Validation Notes

Purpose: locate XvT's DirectPlay session/create path and map it to the XWA
create-pipeline (XWA `memory/xwa-flight-entry.md` #48–75). In XWA, mission
craft are instantiated by a session-gated, DirectPlay-message-driven create
loopback that the single-player force-launch bypasses — leaving the 3D flight
view black. XvT runs the same Totally Games engine two years earlier with a
**DirectPlay-first** design, so the same mechanism should read more directly.

All addresses are from `Z_XVT__.EXE` (image base `0x00400000`, no relocation
applied). Verified with `pefile` + `capstone` against the extracted binary.

## DPLAYX.dll imports (by ordinal)

| Ordinal | API | IAT slot | Import thunk |
|---------|-----|----------|--------------|
| 1 | `DirectPlayCreate` | `0xB5041C` | `0x4F87AE` (`jmp [0xB5041C]`) |
| 2 | `DirectPlayEnumerate` | `0xB50420` | `0x4F87B4` (`jmp [0xB50420]`) |
| 4 | `DirectPlayLobbyCreateA` | `0xB50418` | `0x4F87BA` (`jmp [0xB50418]`) |

(For comparison, XWA imports only **1** DPLAYX function — DirectPlay is far
more central to XvT.)

## Call sites (the session subsystem)

| API | Call sites | Meaning |
|-----|-----------|---------|
| `DirectPlayCreate` | `0x4C23A8`, `0x4C2577`, `0x4C4E2B` | Create the IDirectPlay object (session host/join) |
| `DirectPlayEnumerate` | `0x4C32B4` | Enumerate available sessions |
| `DirectPlayLobbyCreateA` | `0x4C6728` | Lobby-launched session bootstrap |

**Observation:** every DirectPlay entry point lives in one contiguous region,
roughly **`0x4C2000`–`0x4C6800`** — XvT's networking/session module. This is
the region to map against XWA's `sub_004E7A10` (create-broadcast),
`sub_004F91C0`/`sub_004F8A40` (receive-dispatch), and the session-active gate
(`0x77330C` = `0xB0C7BC & 0xFF`).

## Reproducibility notes (toolchain validation)

- The **unmodified XWA `pefile`/`capstone` flow** parses and scans `Z_XVT__.EXE`
  with no target-specific changes → the pipeline generalizes.
- XvT functions use **FPO** (no `push ebp; mov ebp,esp` frame) — same as XWA;
  precise function boundaries require the full `disasm.py` function-discovery
  pass (not yet run here).
- No SafeDisc: `.text` is scanned directly from the on-disk image (no runtime
  decryption dump needed, unlike XWA).

## Next steps

1. Run `disasm.py` function discovery over `.text` to get exact function
   boundaries for the `0x4C2xxx`–`0x4C6xxx` region.
2. Disassemble the `DirectPlayCreate` callers (session host/join) and trace how
   game objects (flight groups / craft) are serialized into DirectPlay messages
   and re-instantiated on receive — the XvT analogue of the XWA `0x3E` create.
3. Map the message-type dispatch and the object/flight-group record layout back
   to XWA. If XvT's create path is legible, it becomes the reference for
   reimplementing the XWA single-player create loopback (the current XWA
   flight-render blocker).
