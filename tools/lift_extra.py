"""Lift a handful of individual functions into gen/recomp_extra.c.

Stopgap for functions the discovery pass missed: appends declarations to
recomp_funcs.h and entries to recomp_dispatch.c so they link and dispatch.
A full `python -m tools <exe> --all` regen supersedes this.

Usage: python -m tools.lift_extra <exe> 0x434670 0x471740 ...
"""
import os
import sys

from .pe_analyze import analyze_pe, build_iat_map
from .disasm import Disassembler
from .lifter import Lifter

GEN = 'src/game/recomp/gen'


def main():
    exe, addrs = sys.argv[1], [int(a, 0) for a in sys.argv[2:]]
    info = analyze_pe(exe)
    pe_data = open(exe, 'rb').read()
    iat_map = build_iat_map(info)
    disasm = Disassembler(pe_data, info.image_base, info.sections)
    lifter = Lifter(iat_map=iat_map)

    lifted = []
    body = []
    for addr in addrs:
        func = disasm.disassemble_function(addr, iat_map)
        if not func or not func.blocks:
            print(f'[!] 0x{addr:08X}: nothing disassembled, skipped')
            continue
        body.append(lifter.lift_function(func))
        lifted.append((addr, func.name))
        print(f'[*] lifted {func.name} ({func.num_instructions} insns)')

    with open(os.path.join(GEN, 'recomp_extra.c'), 'w') as f:
        f.write('/* Functions missed by discovery - lifted by tools/lift_extra.py */\n\n')
        f.write('#define RECOMP_GENERATED_CODE\n#include "recomp_types.h"\n')
        f.write('#include "recomp_funcs.h"\n#include <math.h>\n#include <string.h>\n\n')
        f.write('\n\n'.join(body) + '\n')

    header = os.path.join(GEN, 'recomp_funcs.h')
    existing = open(header).read()
    with open(header, 'a') as f:
        for addr, name in lifted:
            if f'void {name}(void);' not in existing:
                f.write(f'void {name}(void);  /* 0x{addr:08X} - lift_extra */\n')

    # Dispatch table stays address-sorted (main.c binary-searches it).
    dispatch = os.path.join(GEN, 'recomp_dispatch.c')
    lines = open(dispatch).read().split('\n')
    end = next(i for i, l in enumerate(lines) if l.startswith('};'))
    for addr, name in lifted:
        entry = f'    {{ 0x{addr:08X}u, {name} }},'
        if entry not in lines:
            lines.insert(end, entry)
    entries = sorted((l for l in lines[:] if l.strip().startswith('{ 0x')),
                     key=lambda l: int(l.split('0x')[1].split('u')[0], 16))
    head = lines[:next(i for i, l in enumerate(lines) if l.strip().startswith('{ 0x'))]
    tail = [l for l in lines if l.startswith('};') or l.startswith('const uint32_t') or l == '']
    count = len(entries)
    out = head + entries + ['};', '', f'const uint32_t recomp_dispatch_count = {count};', '']
    open(dispatch, 'w').write('\n'.join(out))
    print(f'[*] dispatch table now has {count} entries')


if __name__ == '__main__':
    main()
