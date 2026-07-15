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

## Object model (partial)

- Craft/object **record stride = `0xEC` (236 bytes)** — the only dominant
  `imul r,r,imm` stride in `.text` (11 sites, `~0x40Fxxx`). Compact vs XWA's
  `0xBCF` (3023), consistent with the older 1997 engine.
- A game-state base pointer **`0x9D39B8`** is dereferenced alongside these
  accesses (`mov ecx,[0x9D39B8]; ... [ecx+idx+field]`), with per-entry fields at
  small offsets (`+0x19`, `+0x1A`, `+0x1F`, `+0x7F`). This is the region to
  resolve into the craft array + the spawn that fills it.
- Cross-map target: XWA's craft-record array is `0x8B94E0` (stride `0xBCF`), with
  active byte `+0x8B94F1`, craft type `+0x8B94F0` (from `FG+2`). Establishing the
  XvT field correspondences (active/type/pos) at stride `0xEC` gives the
  per-craft recipe XWA's `sub_0041EF60` needs.

## Status

XvT SP mission-load entry located (`0x517108` filename buffer, `sub_0x455B80`
file-op, mission code at `0x406xxx`/`0x454xxx`). The `.tie`→craft-spawn trace and
the XWA mapping are the next unit. XWA flight remains black; this is the most
promising redirect surfaced by the cross-validation, not a completed fix.

## Spawn/activation function FOUND — `sub_00409390` (2026-07-08)

Located via the stride-`0xEC` craft indexing + state-base `0x9D39B8`. `sub_00409390`
(recomp gen line ~11705) is the **per-craft activation loop** — the local, non-DP
routine that tags flight-group craft active + render-visible. Caller: `0x004DC947`
(mission driver). Inner loop at `~0x40F79C`:

- `eax = [0x9D39B8]` — deref game-state base → object table.
- `edx = [eax + esi + 0x1F]` — per-entry **object pointer** at record offset `+0x1F`
  (esi = entry index * stride).
- `[base + esi + 0x1A] = 2` — **ACTIVE tag** ⇔ XWA craft record `+0x11 = 2`.
- `[obj + 0x2CA] = 4` then `2` — **render-status byte in {2,4}** ⇔ XWA render-visibility
  status `{1,2,4}` (XWA `sub_004652F0` builds the per-frame visible list `0x68BD68`
  from craft whose status byte ∈ {1,2}, transparent `0x68BCE0` from {4}).
- `[obj + 0x90] = 1`, `[obj + 0x89] = [obj+0x90]`, `[edx + 0x7F] = idx*0xEC` (self record xref).

### Cross-validation payoff (direct XWA relevance)
This confirms XvT single-player activates + render-tags craft through a **local path
with NO DirectPlay** — the same conclusion the XWA effort arrived at from the other
side (XWA #165: render path works when fed geometry; the DP loopback caused early-exit;
force-launch reached flight-init but the DP path never sustained). The `+0x1A=2` active
tag and `+0x2CA ∈ {2,4}` render-status map onto the exact XWA fields the render reads.
=> XWA route candidate: drive the local builder `sub_0041EF60` and set the analogous
active + render-status fields, rather than the DP loopback.

NEXT: read the FULL per-craft field set `sub_00409390` writes (type, position,
orientation, iff/ai) — that's the complete recipe to port to XWA's local spawn. Then
trace its caller `0x004DC947` back to the `.tie` parse (`sub_00455B80` chain).

## Per-craft field recipe extracted (sub_00409390 spawn loop ~0x40F6C0–0x40F82B)

Two structures per flight group (mirrors XWA's FG-descriptor + object split):
- **FG-descriptor table**: `[0x9D39B8]` deref → base; per-entry stride **0x23 (35)**
  (idx: `esi=idx*8-idx=idx*7`, then `esi+=esi*4` → `idx*0x23`). *(XWA analogue: FG
  table `0x7B33C4`, stride 0x27.)*
- **Object record**: pointer stored at `fg+0x1F`; stride **0xEC**. *(XWA analogue:
  `ro=FG+0x23`, craft-object stride 0xBCF.)*

Per-craft writes in the spawn loop (position scatter uses RNG `sub_00477EC0`):
| Write | Field | Meaning |
|-------|-------|---------|
| `[fg+0x10] += bx` | position component A (+0x8000 wrap) | spawn X w/ random scatter |
| `[fg+0x12] += di` | position component B (neg if ≥0x8000) | spawn Y w/ random scatter |
| `[obj+0x77] = bp` | orientation/heading word | facing (RNG-derived) |
| `[fg+0x1A] = 2` | **ACTIVE tag** | ⇔ XWA craft `+0x11 = 2` |
| `[obj+0x2CA] = 4` then `2` | **render-status** | ⇔ XWA render status {1,2,4} (sub_004652F0 visible-list filter) |
| `[obj+0x90] = 1`, `[obj+0x89] = [obj+0x90]` | object state flags | |
| `[obj+0x7F]... ` / `[edx+0x7F]=idx*0xEC` | self record xref | |

`sub_00477EC0` = the RNG (called ~5×/craft) — spawn positions/orientation are
scattered around a mission-defined point. So a craft is "spawned" by: allocate its
object (ptr into `fg+0x1F`), set position (`fg+0x10/0x12`) + orientation (`obj+0x77`),
tag active (`fg+0x1A=2`), tag render-visible (`obj+0x2CA∈{2,4}`), set state flags.

### Port to XWA (the black-frame fix candidate)
XWA's per-frame render (`sub_004652F0`) and camera (`sub_004EE820`) crash on craft whose
object/render-object fields are unset (XWA #164/#166). The XvT recipe says the MISSING
writes are: object position (XWA craft `+0xB48` region / ro world pos), orientation
(ro matrix), active `+0x11=2`, and the render-status byte on the ro-scene-manager object
array (`ro+0xDD` → `+0x2E1` entries, status ∈{1,2,4}). Populate these on every FG's craft
BEFORE flight-init links them (`sub_0041EDF0`) and the camera/render stop faulting →
flight-init completes → 3D-frame cb `0x0049E600` ticks → render draws (proven to build a
real visible list). Field-offset mapping (0x23↔0x27 FG stride, 0xEC↔0xBCF object) must be
done per-field, not assumed identical.
