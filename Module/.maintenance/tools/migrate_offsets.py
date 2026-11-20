"""Migrate Module/RBX/client_offsets.hpp to version-2c6fd5e09aaf4cfa.

The old client is still Hyperion-packed, so byte signatures cannot be lifted
from it. Both community dumps exist for the old and the new version instead, so
each constant is translated by name first and by its exact numeric value second.
"""

import io
import re

PAIRS = [
    ('.tmp_off/offsets_version-4ea8e85481e34f46.hpp', '.tmp_off/offsets_version-2c6fd5e09aaf4cfa.hpp'),
    ('.tmp_off/imtheo_old.hpp', '.tmp_off/imtheo_new.hpp'),
]
HEADER = 'Module/RBX/client_offsets.hpp'

NS = re.compile(r'namespace\s+([A-Za-z_][A-Za-z0-9_]*)')
DECL = re.compile(r'(?:const|constexpr)\s+uintptr_t\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(REBASE\()?\s*0x([0-9A-Fa-f]+)')


def parse(path):
    out = {}
    stack = []
    depths = []
    depth = 0
    for raw in io.open(path, encoding='utf-8', errors='surrogateescape'):
        line = raw.strip()
        ns = NS.search(line)
        decl = DECL.search(line)
        if decl:
            out['.'.join(stack + [decl.group(1)])] = int(decl.group(3), 16)
        opens = line.count('{')
        closes = line.count('}')
        if ns and opens:
            stack.append(ns.group(1))
            depths.append(depth)
        depth += opens - closes
        while depths and depth <= depths[-1]:
            depths.pop()
            if stack:
                stack.pop()
    return out


sources = []
for old_path, new_path in PAIRS:
    old = parse(old_path)
    new = parse(new_path)
    by_value = {}
    for name, value in old.items():
        by_value.setdefault(value, []).append(name)
    sources.append((old, new, by_value))
    print('%s: %d entries' % (old_path.split("/")[-1], len(old)))

HEADER_NS = re.compile(r'namespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{')

text = io.open(HEADER, encoding='utf-8', errors='surrogateescape').read().split('\n')
stack = []
converted = []
unresolved = []
out = []

for line in text:
    stripped = line.strip()
    ns = HEADER_NS.match(stripped)
    decl = DECL.search(line)

    if decl:
        name = decl.group(1)
        value = int(decl.group(3), 16)
        qualified = '.'.join(stack + [name])
        new_value = None
        found_via = None

        for old, new, by_value in sources:
            # 1) same qualified name path
            if qualified in old and qualified in new:
                new_value = new[qualified]
                found_via = qualified
                break
            # 2) same leaf name, unique in that dump
            leaves = [k for k in old if k.split('.')[-1].lower() == name.lower()]
            if len(leaves) == 1 and leaves[0] in new:
                new_value = new[leaves[0]]
                found_via = leaves[0]
                break
            # 3) exact numeric value, unambiguous
            cands = by_value.get(value, [])
            if len(cands) == 1 and cands[0] in new:
                new_value = new[cands[0]]
                found_via = cands[0]
                break

        if new_value is None:
            unresolved.append((qualified, decl.group(3)))
            out.append(line)
        else:
            converted.append((qualified, decl.group(3), new_value, found_via))
            out.append(re.sub(r'0x' + decl.group(3), '0x%X' % new_value, line, count=1))
    else:
        out.append(line)

    if ns:
        stack.append(ns.group(1))
    if stripped.startswith('}') and stack:
        stack.pop()

print('converted %d, unresolved %d' % (len(converted), len(unresolved)))
io.open('.tmp_off/migrated.hpp', 'w', encoding='utf-8', newline='\n').write('\n'.join(out))
with io.open('.tmp_off/unresolved.txt', 'w', encoding='utf-8', newline='\n') as handle:
    for qualified, hex_value in unresolved:
        handle.write('%-46s 0x%s\n' % (qualified, hex_value))
