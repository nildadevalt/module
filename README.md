# Module

**Level 8, 99% UNC, 100% sUNC and 89% debUNC**
working on version-2c6fd5e09aaf4cfa (0.748.0.7481526)

**⚠️ Only for educational purpose**

## Build
```
MSBuild Module/VynxVX.vcxproj -t:Build -p:Configuration=Release -p:Platform=x64 -m:1
```
Output: `Module/x64/Release/Module.dll`

## Notes
- Vendored Luau2 is synced to the 0.748 client ABI (offsets from the client dump).
- `Module/RBX/client_offsets.hpp`, `fflags.hpp`, `update.hpp` are retargeted for this build.
- Test harness: `python auto_sunc.py --script <script.lua> --place-id 95206881 --timeout <sec>`
