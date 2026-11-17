# Madium.dll reverse-engineering notes

## Unpacking Themida (works, repeatable)
`Madium.dll` is Themida-packed (`.themida` + `.boot`, EP in `.boot`, file entropy 7.98).
The packer unpacks inside `DllMain` and does not refuse a plain host, so a minimal
loader is enough - no Roblox and no debugger needed:

```c
HMODULE m = LoadLibraryA("Madium.dll");   // Themida stub decrypts, then real DllMain
Sleep(2000);
// write SizeOfImage bytes from (BYTE*)m to disk
```

Helper lives in `.tmp_madium/host.c` (+ `build.bat`). Result: a 46 MB flat image with
RVA == file offset. Verification: `.text` entropy drops 7.98 -> 6.69, `.rdata` 3.45,
real strings (`getsignalarguments`, `hookmetamethod`, ...) appear.

## API table layout
Registration tuples in `.rdata` hold `Name`, `snake_case` alias and the handler pointer;
the registration site in `.text` looks like:

```
lea rax, [rip+Name] ; mov [rbp-0x40], rax
lea rax, [rip+Alias]; mov [rbp-0x38], rax
lea rax, [rbp-0x40]; mov [rbp-0x50], rax
lea rax, [rbp-0x30]; mov [rbp-0x48], rax
lea r8,  [rip+Handler]        <-- Lua C function
call 0x1c7420                 <-- registrar
```

`table2.py` recovers 433 name->handler pairs; `find_handlers.py` resolves the rest by
searching the first `lea r8` after each name xref. `disas.py <name|0xaddr>` disassembles
with string and handler annotations.

## APIs Madium has that we do not (34 confirmed names)
cfiresignal, getactorfromthread, getactorthreads, getcurrentactor, getdeletedactors,
getcallbackfunction, getcallbackthread, setcallbackfunction, getconnectionfunction,
getconnectionthread, isconnectionenabled, setconnectionenabled, isluaconnection,
isgamescriptconnection, iswaitingconnection, geteventmember, getfilter,
getfunctionreferences, getobjectreferences, getoriginalenv, hookproto, isprotohooked,
restoreproto, isprotected, runonthread, writecustomasset, distpoint, ...

These are the same functions debUNC reports as missing for us, so they are learnable.

## Proto hooks - what Madium actually does
Recovered handlers: `hookproto` 0x22e510, `isprotohooked` 0x22f150, `restoreproto` 0x2326f0,
shared helper `0x230780`, registry helper `0x2308c0`.

`0x2308c0` builds/looks up a **Lua registry table keyed by a static light-userdata key**
(`lea rdx, [rip+0xad7c5c]`, `lua_rawgetp`/`lua_createtable`/`lua_rawsetp`, type id 6 = table).
So hook state lives in a Lua-side table keyed by the proto pointer, not in a modified Proto.
`isprotohooked` then reads a record from the argument value (`[value+0x18]` = hook function
pointer) and accepts it only when `byte[record+6] == 0` and `qword[record+0x18] == hook`.
`restoreproto` re-checks that record, allocates an 8-byte box holding the original pointer and
invokes a closure (0x22a2e0) with a 4-field context - that is the "call original" trampoline.

Conclusion: porting does **not** require generating Luau bytecode. A registry-keyed record
table + a trampoline closure is enough, which is far cheaper than what we deferred earlier.
