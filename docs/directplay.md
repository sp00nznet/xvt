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
