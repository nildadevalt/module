# Myriad verification

Verified the unchanged `D:\NERO\MacOS\tests\myriad_full.lua` v1.8.2 in place **79035306837882**.

```text
MYR-DONE ran=140 200=140
```

All 140 distinct checks returned 200. Roblox remained running after completion. Roblox was closed and its exit verified before each build and new Player session. Tests used TCP script submission and local console logs.

The Release x64 build succeeded and deployed to `C:\Users\LASSE\AppData\Local\Roblox\Versions\version-4ea8e85481e34f46\Module.dll`. Existing compiler/linker warnings remain in the compilation log; they did not prevent the build.

## Changes verified

- Actor dispatch uses the native ScriptContext owner, anchored threads and Instances, and the active task library.
- New VM threads initialize the client finalizer field; client GC assists use the client collector so its additional roots and tagged pages remain valid.
- Touch events use the primitive overlap offset from the supplied Windows offsets. Cloned Instance references retain their metatable.
- Callback reads follow the captured object to the validated native thread and function reference.
- Metamethod hooks preserve the shared closure and its identity when its saved body is called from the hook. Direct calls to the saved clone retain their separate identity.
- `debug.getupvalues` rejects C closures.
- The matching local Tovek v2.7 backend is deployed with its MIT license and copied by the build when changed.

## Evidence

- Player log: `C:\Users\LASSE\AppData\Local\Roblox\logs\0.747.0.7471481_20261114T071000Z_Player_FEC09_last.log`
- Console evidence: `D:\NERO\ModuleV2\.maintenance\myriad-console-evidence.log`
- Full compilation log: `D:\NERO\ModuleV2\.maintenance\build-myriad-callback.log`
- Final build/deployment log: `D:\NERO\ModuleV2\.maintenance\build-myriad-final.log`
- Artifact metadata: `D:\NERO\ModuleV2\.maintenance\deployed-artifact.json`
- Offsets source: `D:\NERO\Offsets\hyperion_win_live.txt`

Module SHA256: `4a9ebd76424aba87fc99abfc7c5dbd298813ba035a32f8978a9391ad239da839`

Unchanged suite SHA256: `7bc5ea307c252d51c341b084635003d9067764c1317b3d27ed8bcab392762981`

Local decompiler SHA256: `5e8e090766aeee227cc9579a1d086d825955e23a606795eeb58bab15c1990990`
