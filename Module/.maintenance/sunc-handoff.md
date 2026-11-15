The current Module.dll is restored at `C:\Users\LASSE\AppData\Local\Roblox\Versions\version-4ea8e85481e34f46\Module.dll`. Release x64 build verification passed. Its SHA-256 is `fec655dc071ea622188029ef9730cdff40691e54e3ba613289d6a05b40e16b82`. Roblox is closed and the temporary disabled DLL name is absent.

The latest sUNC attempt in place 90441122676618 was rejected with Roblox error 268 before any of the 86 checks completed. It is an invalid run, with no percentage score. The raw Player log is `sunc-iteration-15.log`; its result is `sunc-iteration-15.json`.

Two controls used the same place: Module.dll disabled stayed connected for 45.2 seconds; Module.dll loaded and initialized with no test script submitted stayed connected for 45.56 seconds. Both ended with Roblox closed and the DLL hash intact. These observations narrow the rejected run to the sUNC execution path during the observed intervals. They do not identify a wrong offset or prove the underlying cause of error 268.

The best historical completed sUNC run remains 81/86 (94%), in `sunc-iteration-10.log`. Its failing checks were clonefunction, getconnections, hookfunction, newcclosure, and restorefunction. This score predates later DLL changes and is not a score for the current DLL. The requested 100% has not been achieved.

The Windows runner now saves explicit failed or invalid JSON reports, clears stale evidence before a live rerun, rejects disconnected logs even through --check-log, validates the reported percentage, and closes Roblox after launched runs. All 10 report-validation regression tests passed. No C++ source or offsets were changed during this handoff continuation.

Offsets remain sourced from `D:\NERO\Offsets\hyperion_win_live.txt`; its hash matches the source recorded in client_offsets.hpp. The older `deployed-artifact.json` describes an earlier DLL and its Myriad verification. `sunc-handoff.json` records the current artifact and test state.
