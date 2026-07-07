# XvT Single-Player Craft Spawn — Local (non-DirectPlay) Path

Follows from `docs/directplay.md`: the DirectPlay subsystem in XvT is
**multiplayer-only** (the `0xB4E5FC` connection byte indexes enumerated network
service providers). Single-player therefore instantiates craft through a
separate **local** path that reads a `.tie` mission and spawns flight-group
craft directly. This doc tracks locating that path — the model for how a Totally
Games title creates SP craft, and the hypothesis-under-test for XWA (whose SP
flight is currently black because the effort forced the *DirectPlay* create path
— possibly the wrong one for single-player).

All addresses from `Z_XVT__.EXE` (base `0x00400000`), verified with capstone.

## Mission file plumbing

- **`0x517108`** — mutable **current-mission-filename** buffer, initialized to
  the literal `"DEMO.TIE"`. Code overwrites it (e.g. `mov byte [reg+0x517108],'i'/'n'/'v'…`)
  to point at different `.tie` missions. `.tie` = the TIE mission format, the
  same family XWA uses.
- **`sub_0x455B80`** — mission file operation (open / exists-check), called with
  `0x517108` from many sites (`0x40612B`, `0x45477C`, `0x4548B8`, `0x454A96`, …).
- Other mission ops on the buffer: `sub_0x452E80` (`0x406162`), `sub_0x4093B0`
  (`0x40627F`).

## Mission-management regions

Two clusters reference the mission buffer heavily — the mission load/select code:

| Region | Notes |
|--------|-------|
| `~0x406000–0x406300` | early mission setup / filename build + file checks |
| `~0x454600–0x454B00` | larger mission-management function (`sub_0x455B80` file checks, state at `0xA3193C`, table `0x9C5D54`) |

## Goal / next steps

1. Trace `sub_0x455B80` (`.tie` open) → the `.tie` parser → the **flight-group /
   craft spawn** function that populates XvT's craft-object array with **no
   DirectPlay calls**. That function is the SP-spawn reference.
2. Read off the **per-craft field setup** it performs before instantiating each
   craft (type, position, orientation, AI, iff). This is the missing-data recipe
   that XWA's local builder `sub_0041EF60` needs — in the XWA effort, driving the
   local spawn (`sub_005064D0`→`sub_0041EF60`) for all flight groups crashed
   *only* because those per-craft fields weren't fully set (the XWA player craft
   built fine once its type `FG+2` was supplied).
3. Map the XvT SP-spawn onto XWA: if XWA has the analogous local spawn (very
   likely — `sub_0041EF60` is a local, non-DP builder), driving it with the
   complete per-craft field set is the candidate route to rendered XWA flight —
   an alternative to the DirectPlay loopback that the XWA effort proved does not
   converge under forcing.

## Status

XvT SP mission-load entry located (`0x517108` filename buffer, `sub_0x455B80`
file-op, mission code at `0x406xxx`/`0x454xxx`). The `.tie`→craft-spawn trace and
the XWA mapping are the next unit. XWA flight remains black; this is the most
promising redirect surfaced by the cross-validation, not a completed fix.
