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

## Session-object creation (disassembled)

The `DirectPlayCreate` call at `0x4C2577` (function starting `0x4C2558`) reads
cleanly:

```asm
0x4C2560  call 0x4C32A0        ; obtain provider/connection GUID (-> eax)
0x4C2568  test eax, eax
0x4C256A  je   0x4C2A6A        ; bail if no provider
0x4C2570  push ebx             ; pUnkOuter (0)
0x4C2571  push 0xA70605        ; &lpDP  <-- global for the IDirectPlay object
0x4C2576  push eax             ; lpGUID
0x4C2577  call DirectPlayCreate ; -> HRESULT
0x4C257E  jne  0x4C2A8E        ; bail on failure
0x4C2584  mov  eax, [0xA70605] ; the created IDirectPlay object
0x4C258A  mov  ebp, [eax]      ; vtable
0x4C258C  call [ebp + 8]       ; initialize the object
```

**`0xA70605` is XvT's IDirectPlay session-object global** — the direct analogue
of the XWA DirectPlay object/session that single-player force-launch could not
bootstrap (XWA `0xB0C7BC`/`0x77330C`). Here it is created in plain sight, no
SafeDisc obfuscation. This is the reference point for understanding how the
Totally Games engine stands up its session before the craft-create messages
flow — the exact step missing in XWA SP.

Open question to answer next: does XvT's **single-player / instant-action** mode
also create a (loopback) DirectPlay session through this path? If so, this code
*is* the blueprint for the XWA SP create loopback.

## Three session-setup paths

The IDirectPlay object global `0xA70605` is referenced at **12 sites in three
clusters**, one per `DirectPlayCreate` call — i.e. three distinct session-setup
functions:

| Cluster | `0xA70605` refs | `DirectPlayCreate` | Likely role |
|---------|-----------------|--------------------|-------------|
| A | `0x4C23A3, 0x4C23BB, 0x4C23CD, 0x4C23D9` | `0x4C23A8` | one setup mode (host?) |
| B | `0x4C2572, 0x4C2585, 0x4C25A1, 0x4C2A3E` | `0x4C2577` | another (join?) |
| C | `0x4C4E26, 0x4C4E7C, 0x4C4E8D, 0x4C4E99` | `0x4C4E2B` | third mode (lobby / instant-action?) |

Identifying which cluster is entered by **single-player / instant-action** is
the crux: that path is XvT's equivalent of the XWA SP create loopback. The
three-way split is itself a useful structural comparison — XWA's SP object
create funnels through one custom loopback (`sub_0052CEE0` send → dispatcher),
so seeing how XvT selects among host/join/local here should clarify what the
XWA SP path is emulating.

## Cluster C detail (`0x4C4E2B`) — versioned-interface acquisition

```asm
0x4C4E23  push 0             ; pUnkOuter
0x4C4E25  push 0xA70605      ; &lpDP
0x4C4E2A  push eax           ; lpGUID  <-- provider GUID (selects loopback vs network)
0x4C4E2B  call DirectPlayCreate
0x4C4E32  je   0x4C4E76      ; on SUCCESS ->
   ... (failure path: sub_0x4BE020(6,&buf) + error string 0x5258B0 + sub_0x4C86E0 dialog)
0x4C4E76  push 0xA70609      ; &lpDPvN  (output: upgraded interface)
0x4C4E7B  mov  eax,[0xA70605]
0x4C4E82  push 0x5152B0      ; IID_IDirectPlayN (interface-version GUID, in .rdata)
0x4C4E88  call [vtable+0]    ; QueryInterface -> IDirectPlay2/3/4 into 0xA70609
0x4C4E94  call [vtable+8]    ; initialize / AddRef
```

- `0xA70605` = raw `IDirectPlay` from `DirectPlayCreate`; **`0xA70609` = the
  versioned interface** (`IDirectPlay2A`/`3A`/`4A`) the game actually uses.
- The **provider GUID in `eax`** at the `DirectPlayCreate` call is the
  single-player-vs-network discriminant. DirectPlay's loopback/"single-player"
  service provider vs the TCP/IPX/modem providers is chosen by this GUID. The
  cluster whose provider resolves to the local/loopback GUID is XvT's SP path —
  and its craft-create-message flow is the XWA SP-loopback blueprint.

**Trace next:** back-track `eax` (the provider GUID source) for clusters A/B/C
to identify which is entered by instant-action/training (single-player).

### `sub_0x4C4DD0` = generic `create_session(providerGUID)`

The cluster-C `DirectPlayCreate` lives in a clean, self-contained function:

```asm
0x4C4DD0  sub esp, 0x150 ; push ebx/esi/edi/ebp      ; prologue (FPO)
0x4C4DDC  mov  bl, [0xA49555]                         ; mode/state flag
0x4C4DE7  mov  eax, [0xA70609] ; test eax,eax ; jne   ; IDEMPOTENT: skip if
                                                       ; a session already exists
0x4C4DF4  mov  eax, [esp+0x17C]                        ; providerGUID (ARGUMENT)
0x4C4DFC  call sub_0x4C32A0                            ; resolve/validate provider
0x4C4E06  jne  <create>                                ; valid -> DirectPlayCreate
          ...                                          ; (invalid -> early return)
```

Takeaways for XWA:
- The **provider GUID is a parameter** — the host/join/SP distinction is made by
  the *caller*, not baked into the create function. `sub_0x4C32A0` is the shared
  provider resolver (also called at `0x4C2560` in cluster B).
- The `[0xA70609] != 0` idempotency guard means "don't re-create an existing
  session" — a clean structural marker of where the session lifetime begins.
- XvT's session-establish is a **single legible `create_session(provider)`**,
  versus XWA's session bootstrap (`sub_0050C640`→`sub_00594063`) that could not
  be stood up in SP force-launch. Mapping the XvT SP caller here shows exactly
  which provider + call sequence brings a Totally Games session up locally.

**Identifying SP:** find the caller of `sub_0x4C4DD0` (or its A/B siblings) that
passes the **loopback/local DirectPlay service-provider GUID** (single-player
hosts a local session with no remote clients). That caller + provider is the
XWA SP-loopback blueprint. Requires the full `disasm.py` function-discovery pass
to enumerate callers cleanly.

## Subsystem structure (function-discovery pass)

Call-target analysis over `.text` (the XWA toolchain's core discovery method,
run unmodified) finds **1,673 function candidates** — same order of magnitude as
XWA's 2,702, smaller binary. This is a second reproducibility datapoint.

DirectPlay session layer, bottom-up:

| Layer | Address | Role | Callers |
|-------|---------|------|---------|
| API thunks | `0x4F87AE/B4/BA` | `jmp [DPLAYX IAT]` | — |
| Session-create (host/join) | `0x4C21A0` | contains `DirectPlayCreate` @`0x4C23A8` **and** @`0x4C2577` (two modes in one fn) | `0x4B3E5E`, `0x4CC434`, `0x4CCC5C`, `0x4D45A5` |
| Session-create (provider) | `0x4C4DD0` | `create_session(providerGUID)` | `0x4CC673` |
| Provider resolver | `0x4C32A0` | resolve/validate provider GUID | (shared) |
| **Mode/management layer** | **`~0x4CC000–0x4CCC00`** | selects host / join / SP and drives the create fns | (entry from menu/game flow) |
| Session globals | `0xA70605`, `0xA70609` | raw + versioned `IDirectPlay` | — |

**The `~0x4CCxxx` cluster is where single-player vs multiplayer is decided** —
`0x4CC434`, `0x4CC673`, `0x4CCC5C` all call into the session-create functions.
Reading this layer identifies which call path instant-action/training takes and
with which provider GUID; that path is XvT's SP session-establish and the
reference for the XWA SP create loopback.

## Connection-type selection (the SP/MP switch)

The caller `0x4CC673` invokes `sub_0x4C4DD0` with **7 args**:

```asm
0x4CC626  mov eax,[0x525A50]; cmp eax,-1; je proceed   ; gate: state must be -1
   push [0xB4E5FC]          ; arg7 = connection-type selector (BYTE)
   push 0x20                ; arg6 = size (32)
   push &localbuf           ; arg5
   push [0x5151FC]          ; arg4 \
   push [0x5151F8]          ; arg3  } args1-4 = 16-byte APPLICATION GUID @0x5151F0
   push [0x5151F4]          ; arg2  |   {09438C20-E06A-11CE-8681-00AA006C5D57}
   push [0x5151F0]          ; arg1 /
   call sub_0x4C4DD0
```

Inside `sub_0x4C4DD0`, arg7 (`[esp+0x17C]`) is passed to `sub_0x4C32A0`, which
**resolves the connection-type byte to a DirectPlay service-provider GUID**
(none/serial/modem/IPX/TCPIP), then hands it to `DirectPlayCreate`.

Key globals identified:

| Global | Meaning |
|--------|---------|
| `0xB4E5FC` | **connection-type selector** (set by the connection menu; the SP/MP switch) |
| `0x5151F0` | DirectPlay **application GUID** `{09438C20-E06A-11CE-8681-00AA006C5D57}` |
| `0x5152B0` | interface IID `{9D460580-A822-11CF-960C-0080C7534E82}` (IDirectPlayN QI) |
| `0x525A50` | session state gate (must be `-1` to (re)create) |
| `sub_0x4C32A0` | connection-type → service-provider-GUID resolver |

**The XWA parallel:** XWA's SP object-create is gated on the session flag
`0x77330C` (= `0xB0C7BC & 0xFF`) and never bootstraps because force-launch skips
connection setup. XvT exposes the equivalent switch as a single readable byte
`0xB4E5FC` feeding a provider resolver. Determining the value `0xB4E5FC` takes
for **instant-action/training** (single-player) — and whether that provider is a
real network SP or a null/local one — tells us exactly what a Totally Games
single-player session looks like, i.e. what the XWA SP loopback must synthesize.

**Trace next:** find writers of `0xB4E5FC` in the connection/menu code to read
off the single-player value, then follow that session into the craft-create
message flow.

## KEY REFRAME — the DirectPlay path is multiplayer-only

Two facts change the picture:

1. **`0xB4E5FC` is written only with `0,1,2,3`** at four consecutive menu
   handlers (`0x4B12B9`/`0x4B1434`/`0x4B164E`/`0x4B186A`) — a classic 4-item
   *connection* menu.
2. **`sub_0x4C32A0` is a `DirectPlayEnumerate` wrapper**, not a static GUID
   table: it enumerates the installed DirectPlay **service providers** and
   returns the `0xB4E5FC`-th one's GUID:
   ```asm
   0x4C32A0  mov [0x639FB0],0 ; lea eax,[esp+4] ; push eax ; push cb=0x4C32D0
   0x4C32B4  call DirectPlayEnumerate            ; -> callback picks index-th SP
   0x4C32C0  mov eax,[0x639FB0] ; ret            ; return that provider's GUID
   ```
   `0xB4E5FC` is therefore an **index into the enumerated network providers**
   (serial / modem / IPX / TCP-IP) — every option is a real network transport.

**Implication:** the whole `0x4C2xxx`/`0x4C4xxx`/`~0x4CCxxx` DirectPlay session
machinery is the **multiplayer** path. There is no null/loopback provider in it,
so **XvT single-player (instant-action / training / historical combat) does not
create its craft through DirectPlay** — it must use a separate, *local*
object-spawn that reads the mission and instantiates craft directly.

### Why this matters for XWA

The XWA investigation (memory #48–75) concluded XWA single-player craft are
gated behind the DirectPlay session/create loopback (`0x77330C`), which
force-launch can't bootstrap → black flight view. XvT — the same engine — shows
a **clean split**: DirectPlay for MP, a local spawn for SP. This raises a
concrete, testable hypothesis for XWA:

> XWA very likely also has a **local single-player craft-spawn** (invoked at
> mission-load), and the DP-message create path the XWA effort kept hitting is
> the *multiplayer-inherited* branch — not the SP one. The reason forcing the DP
> path never converged may be that **it is the wrong path for single-player.**

**Next (highest value):** locate XvT's single-player object/craft spawn — the
function that reads a mission's flight groups and instantiates craft with **no
DirectPlay involvement** — and map it back to XWA. If XWA has the analogous
local spawn, driving *that* (instead of the DP loopback) is the likely route to
craft on the XWA flight screen.

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
