"""Read only the supplied scheduler layout from the current local Player."""
import ctypes as c
import json
import struct
import sys

pid, base = int(sys.argv[1]), int(sys.argv[2], 0)
kernel = c.WinDLL('kernel32', use_last_error=True)
kernel.OpenProcess.argtypes = [c.c_uint32, c.c_int, c.c_uint32]
kernel.OpenProcess.restype = c.c_void_p
kernel.ReadProcessMemory.argtypes = [c.c_void_p, c.c_void_p, c.c_void_p, c.c_size_t, c.POINTER(c.c_size_t)]
kernel.CloseHandle.argtypes = [c.c_void_p]
handle = kernel.OpenProcess(0x1010, False, pid)
if not handle:
    raise c.WinError(c.get_last_error())

def read(address, size):
    buffer = c.create_string_buffer(size)
    received = c.c_size_t()
    if not kernel.ReadProcessMemory(handle, address, buffer, size, c.byref(received)) or received.value != size:
        raise OSError(f'read failed at {address:#x}: {c.get_last_error()}')
    return buffer.raw

def ptr(address):
    return struct.unpack('<Q', read(address, 8))[0]

try:
    scheduler = ptr(base + 0x8F1E408)
    start, end = ptr(scheduler + 0xC8), ptr(scheduler + 0xD0)
    if not start or end < start or (end - start) % 16 or (end - start) // 16 > 65536:
        raise ValueError('invalid scheduler span')
    jobs = []
    for entry in range(start, end, 16):
        try:
            job = ptr(entry)
            raw = read(job + 0x18, 32)
            size, capacity = struct.unpack_from('<QQ', raw, 16)
            if not 0 < size <= 256 or capacity < size:
                continue
            name = (raw[:size] if capacity < 16 else read(struct.unpack_from('<Q', raw)[0], size)).decode('utf-8', 'replace')
            if 'Script' not in name and 'Render' not in name:
                continue
            data = {'job': hex(job), 'name': name, 'step8': hex(ptr(ptr(job) + 8 * 8))}
            if 'Script' in name:
                context = ptr(job + 0x1C0)
                data['context'] = hex(context)
                if context > 0x10000:
                    try:
                        data['contextDataModel'] = hex(ptr(context + 0x100))
                    except OSError:
                        pass
            jobs.append(data)
        except OSError:
            continue
    fake = ptr(base + 0x8F8E968)
    print(json.dumps({'scheduler': hex(scheduler), 'jobCount': (end-start)//16, 'dataModel': hex(ptr(fake+0x1F8)), 'jobs': jobs}, indent=2))
finally:
    kernel.CloseHandle(handle)
