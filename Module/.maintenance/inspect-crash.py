"""Read a Player minidump and resolve local module frames without running it."""
import ctypes as c
import pathlib
import struct
import sys

data = pathlib.Path(sys.argv[1]).read_bytes()
count, directory = struct.unpack_from('<II', data, 8)
streams = {kind: (size, rva) for kind, size, rva in
           (struct.unpack_from('<III', data, directory + i * 12) for i in range(count))}
rva = streams[6][1]
thread_id = struct.unpack_from('<I', data, rva)[0]
code, flags, record, address, parameter_count, _ = struct.unpack_from('<IIQQII', data, rva + 8)
context = struct.unpack_from('<II', data, rva + 160)[1]
registers = {name: struct.unpack_from('<Q', data, context + offset)[0] for name, offset in
             [('rax',120),('rcx',128),('rdx',136),('rbx',144),('rsp',152),('rbp',160),
              ('rsi',168),('rdi',176),('r8',184),('r9',192),('r12',216),('r13',224),
              ('r14',232),('r15',240)]}
print('exception', hex(code), 'thread', thread_id, 'address', hex(address))
print('parameters', [hex(value) for value in struct.unpack_from('<' + str(parameter_count) + 'Q', data, rva + 40)])
print('registers', {name: hex(value) for name, value in registers.items()})

modules = []
rva = streams[4][1]
for i in range(struct.unpack_from('<I', data, rva)[0]):
    entry = rva + 4 + i * 108
    base, size, _, _, name_rva = struct.unpack_from('<QIIII', data, entry)
    length = struct.unpack_from('<I', data, name_rva)[0]
    name = data[name_rva + 4:name_rva + 4 + length].decode('utf-16le')
    modules.append((base, size, name))
    if base <= address < base + size:
        print('fault', pathlib.PureWindowsPath(name).name, hex(address - base))

class Symbol(c.Structure):
    _fields_ = [('SizeOfStruct',c.c_uint32),('TypeIndex',c.c_uint32),('Reserved',c.c_uint64*2),
                ('Index',c.c_uint32),('Size',c.c_uint32),('ModBase',c.c_uint64),('Flags',c.c_uint32),
                ('Value',c.c_uint64),('Address',c.c_uint64),('Register',c.c_uint32),('Scope',c.c_uint32),
                ('Tag',c.c_uint32),('NameLen',c.c_uint32),('MaxNameLen',c.c_uint32),('Name',c.c_char*1)]

class Line(c.Structure):
    _fields_ = [('SizeOfStruct',c.c_uint32),('Key',c.c_void_p),('LineNumber',c.c_uint32),
                ('FileName',c.c_char_p),('Address',c.c_uint64)]

kernel = c.WinDLL('kernel32')
kernel.GetCurrentProcess.restype = c.c_void_p
process = kernel.GetCurrentProcess()
symbols = c.WinDLL('dbghelp', use_last_error=True)
symbols.SymInitializeW.argtypes = [c.c_void_p,c.c_wchar_p,c.c_int]
symbols.SymLoadModuleExW.argtypes = [c.c_void_p,c.c_void_p,c.c_wchar_p,c.c_wchar_p,c.c_uint64,c.c_uint32,c.c_void_p,c.c_uint32]
symbols.SymLoadModuleExW.restype = c.c_uint64
symbols.SymFromAddr.argtypes = [c.c_void_p,c.c_uint64,c.POINTER(c.c_uint64),c.POINTER(Symbol)]
symbols.SymGetLineFromAddr64.argtypes = [c.c_void_p,c.c_uint64,c.POINTER(c.c_uint32),c.POINTER(Line)]
symbols.SymSetOptions(0x16)
symbols.SymInitializeW(process, r'D:\NERO\ModuleV2\build\obj\x64\Release', False)
for base, size, name in modules:
    if name.lower().endswith('module.dll'):
        symbols.SymLoadModuleExW(process, None, name, None, base, size, None, 0)

def resolve(address):
    storage = c.create_string_buffer(c.sizeof(Symbol) + 1024)
    info = c.cast(storage, c.POINTER(Symbol))
    info.contents.SizeOfStruct = c.sizeof(Symbol)
    info.contents.MaxNameLen = 1024
    displacement = c.c_uint64()
    if not symbols.SymFromAddr(process, address, c.byref(displacement), info):
        return ''
    name = c.string_at(c.addressof(storage) + Symbol.Name.offset, info.contents.NameLen).decode()
    line = Line()
    line.SizeOfStruct = c.sizeof(Line)
    delta = c.c_uint32()
    if symbols.SymGetLineFromAddr64(process, address, c.byref(delta), c.byref(line)):
        return f'{name}+{displacement.value:#x} {line.FileName.decode()}:{line.LineNumber}'
    return f'{name}+{displacement.value:#x}'

rva = streams[3][1]
for i in range(struct.unpack_from('<I', data, rva)[0]):
    entry = rva + 4 + i * 48
    if struct.unpack_from('<I', data, entry)[0] != thread_id:
        continue
    stack_base, stack_size, stack_rva = struct.unpack_from('<QII', data, entry + 24)
    start = registers['rsp'] - stack_base
    for offset in range(start, min(start + 1024, stack_size), 8):
        candidate = struct.unpack_from('<Q', data, stack_rva + offset)[0]
        for base, size, name in modules:
            if base <= candidate < base + size and ('module.dll' in name.lower() or 'RobloxPlayerBeta' in name):
                label = pathlib.PureWindowsPath(name).name
                print('stack', hex(offset - start), label, hex(candidate - base),
                      resolve(candidate) if label.lower() == 'module.dll' else '')
