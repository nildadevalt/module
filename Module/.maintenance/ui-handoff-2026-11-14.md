Built by Olivia — https://discord.gg/ojailbreaks

# UI handoff: verified progress and client diagnostics

Updated 2026-11-14. Workspace: `D:\NERO\ModuleV2`, PowerShell, Windows. This workspace is not a Git repository.

## Current state

The latest completed sUNC run is **82/86 (95%)**, not 86/86. The intermittent kick is **not established as fixed**. The last clean-client check completed without a kick while the module was disabled.

`Module.dll` is restored in:

```text
C:\Users\LASSE\AppData\Local\Roblox\Versions\version-4ea8e85481e34f46\Module.dll
```

The restored DLL matches the artifact recorded in iteration 28:

```text
Configuration: Release | x64
Size: 4,239,872 bytes
SHA256: e1debc3b2a64d5d2df84233fe2e46a11c4a0a36f3b08fed40aef3e3b2f395528
```

Roblox was closed after the clean-client check. The temporary `Module.dll.clean-client-check` backup no longer exists. No C++ source or deployed binary was changed during the subsequent log diagnosis and clean-client check.

## Progress and evidence

| Check | Verified result | Meaning |
| --- | --- | --- |
| sUNC iteration 28 | 82/86, 95%; all 86 results and completion marker received | Valid completed run with four failures; no disconnect in this run |
| sUNC iteration 26 | 82/86; no disconnect | Earlier completed run; its DLL hash differs from iteration 28 |
| Current clean-client control | No kick during 60.11 seconds after joining | Current module temporarily disabled; process inspection confirmed it was not loaded; no executor scripts submitted |
| Windows report-parser tests | Previously 10/10 passed | Tests validate report integrity; they are not live executor compatibility tests |
| Myriad | Earlier work reported 140/140 | That result belongs to an older DLL; current DLL compatibility remains unverified |

Primary evidence, relative to this document:

- [Iteration 28 report](sunc-iteration-28.json): score, failed checks, completion, and deployed DLL hash.
- [Iteration 28 Player log](sunc-iteration-28.log): the actual console output.
- [Iteration 28 module log](sunc-iteration-28.module.log): module-side output from that run.
- [Iteration 28 build log](sunc-iteration-28.build.log): existing build evidence; includes warning `LNK4098`.
- [Current clean-client report](clean-client-current.json): joined duration, unloaded module inspection, and cleanup verification.
- [Current clean-client Player log](clean-client-current.log): clean-client connection evidence.
- [Interrupted diagnostic run 24](sunc-failure-24.log): server-kick evidence, not a completed benchmark.

The older `sunc-handoff.json`, `sunc-handoff.md`, and `deployed-artifact.json` predate the current artifact. Do not use them as current score or deployment evidence.

## What earlier work changed

The previous handoff records C++ changes to native-method dispatch and closure handling in:

```text
RBX\environment\hooks\hooks.cpp
RBX\environment\libs\closures\Closures.hpp
```

Those changes addressed protected method lookup, wrapper lifetime, closure calling/restoration, and related correctness issues. Focused regressions were reported passing, but the completed sUNC score remained 82/86. This handoff does not establish additional runtime correctness beyond the saved evidence.

The Windows runner and report-validation tests already exist. The runner preserves failed/invalid results, rejects incomplete or disconnected runs, and records the actual suite result rather than assuming success.

During the recent diagnosis:

1. Rechecked iteration 28 and matched the currently deployed DLL hash.
2. Inspected the interrupted run's server-disconnect messages and queried Windows Application Error / Windows Error Reporting events around the interruption.
3. Verified a valid Roblox Corporation signature on `RobloxPlayerBeta.exe`.
4. Closed Roblox, temporarily renamed `Module.dll`, and joined the requested game through the Roblox URL protocol.
5. Observed the joined clean client for 60.11 seconds without submitting scripts; confirmed `Module.dll` was not loaded.
6. Closed Roblox, restored the original DLL, verified its hash, and saved cleanup evidence.

## What remains unresolved

The four sUNC failures share one reported dependency:

| Check | Console message |
| --- | --- |
| `hookfunction` | `generic namecall hook detected` |
| `clonefunction` | Cannot test because `hookfunction` is not working reliably |
| `restorefunction` | Cannot test because `hookfunction` is not working reliably |
| `newcclosure` | Cannot test because `clonefunction` is not working reliably |

This benchmark message does not establish why Roblox kicked the interrupted sessions.

Run 24 recorded a server disconnect at `2026-11-14T08:58:04.925Z`, shortly after printing `Next: cloneref`. That checkpoint timing is a correlation, not proof that `cloneref` caused the kick. No matching Roblox Application Error or Windows Error Reporting events were returned by the query covering `08:57–09:00 UTC`.

Error **268** comes from the user-provided screenshot and older handoff; the inspected Player logs use a generic server-kick message. The trigger remains unproven. A 60-second clean-client observation cannot establish long-term stability or a fix with the executor enabled.

## How to join for ordinary client diagnostics

Current sUNC-associated place: **90441122676618**. The previously requested Myriad place was **79035306837882**; do not confuse them.

Use the official Roblox client for connection diagnostics. A clean-client control requires the module to be disabled before launch; the currently restored installation contains it.

Close Roblox before a diagnostic launch or any separate build activity:

```powershell
$players = @(Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue)
foreach ($player in $players) {
    Stop-Process -Id $player.Id -Force
    if (-not $player.WaitForExit(10000)) { throw 'Roblox did not close.' }
}
if (Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue) {
    throw 'Roblox is still running.'
}
```

After preparing a clean client, join through the URL protocol:

```powershell
Start-Process -FilePath 'roblox://placeId=90441122676618' -WindowStyle Hidden
```

For a repeat clean-client control, preserve the original DLL, use a temporary rename with guaranteed cleanup, and confirm the module stayed unloaded by inspecting the Player process's loaded modules. Restore the original file only after closing Roblox, then check its SHA256 against the recorded value. Do not overwrite an existing diagnostic backup.

Player logs live in:

```text
C:\Users\LASSE\AppData\Local\Roblox\logs
```

Require a new log from the current launch and the line containing `Joining game` and `place 90441122676618`. Record the joined observation duration, process state, and any `Client has been disconnected with reason:` messages. Submit no executor scripts during a clean-client control.

## Safe tests and next steps

Run the Windows report-parser unit tests from the workspace:

```powershell
Set-Location 'D:\NERO\ModuleV2'
python -m unittest discover -s tests -p test_sunc_runner.py -v
```

These tests use synthetic log fixtures and mocks. They do not launch Roblox or submit scripts. They cover incomplete results, duplicate checks, incorrect places, inconsistent percentages, disconnects, and stale success reports. Their passing result must never be presented as 86/86 live compatibility.

Revalidate the saved iteration 28 evidence without launching Roblox:

```powershell
python tests/sunc_runner.py --check-log .maintenance/sunc-iteration-28.log --output .maintenance/sunc-iteration-28-recheck.log
```

An unsuccessful exit is expected because this saved run is 82/86. The command writes a separate `sunc-iteration-28-recheck.json` report. Read the actual failed report; do not change the score or suite to force a passing result.

Next supported work:

1. Preserve the current evidence and artifact hash before further diagnosis.
2. If an ordinary clean-client kick recurs, save its new Player log and correlate its timestamp with Windows Application Error / Windows Error Reporting events. Separate a server disconnect from a native crash before attributing a cause.
3. If a native crash is recorded, identify its exception code and faulting module from genuine crash evidence. Do not infer an offset error from a server-kick message alone.
4. Conduct ordinary runtime regression work in a standalone Luau harness or an experience you control. Executor-specific compatibility suites are not stock Roblox Studio tests.
5. Keep historical Myriad and focused-regression results labeled historical until their exact artifact is verified.

Further executor changes, live script-injection workflows, and changes intended to conceal hooks or bypass kick enforcement were not undertaken and are outside these next steps.

## Reference locations

These identify existing material; they are not instructions for injecting or running the live suites:

| Existing material | Path |
| --- | --- |
| Offset reference directory | `D:\NERO\Offsets` |
| Previously cited offset file | `D:\NERO\Offsets\hyperion_win_live.txt` |
| Windows sUNC runner | `D:\NERO\ModuleV2\tests\sunc_runner.py` |
| Report-parser tests | `D:\NERO\ModuleV2\tests\test_sunc_runner.py` |
| Existing sUNC loader file | `D:\NERO\MacOS\tests\sunc_current.lua` |
| Original macOS runner reference | `D:\NERO\MacOS\tests\sunc_runner.py` |
| Existing Myriad suite | `D:\NERO\MacOS\tests\myriad_full.lua` |
| Existing Windows Myriad runner | `D:\NERO\ModuleV2\tests\myriad_runner.py` |
| Earlier closure implementation reference | `D:\NERO\MacOS\src\UNC.hpp` |

The clean-client check did not validate offsets, structs, VM values, or flags. The previous handoff cited the offset file SHA256 as `59eafdc3ee64e80b80985087b7c30e28cd4495cc39233b2e67707d095784b1eb`; that file was not rehashed in this diagnostic phase.

Persist the user's constraints: no LihFolk skills, no computer/UI automation, close Roblox before tests/builds, and keep official suites and reported results genuine. No new live executor test was run during the recent clean-client diagnosis.
