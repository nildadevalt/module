# VYNXVX MODULE — PERFORMANCE AUDIT & OPTIMIZATION SPEC (REVISED)

> **Scope:** `VynxVX / Module` — x64 C++20 Windows DLL, Luau VM 0.6xx, Roblox client hook.  
> **Method:** claims checked against source in this tree, with file/line where practical.  
> **Revision:** 2026-11-15 — updated to reflect in-tree changes (WebSocket async connect, Dev Console mirror, RakNet/websocket hardening, UNC rerun). Items below describe the **current** state; superseded claims are called out explicitly.  
> **Baselines (repo evidence):** sUNC 82/86 (`.maintenance/sunc-iteration-28.json` gate context in prior audit); Myriad 140/140 reported on an older DLL hash (not re-verified here). Latest automated UNC run in this workspace: **99% (81/82)** on Baseplate `95206881` using `Module\.maintenance\unc_test_user.lua` (log `0.747.0.7471481_20261115T101813Z_Player_1E2C2_last.log`) — remaining fail is an environment/path issue (`CoreGui.RobloxGui.Modules.Common.Constants` absent in Baseplate), not a `getscriptclosure` regression.

---

## 0. How to read this

| Column | Meaning |
|---|---|
| **Confidence** | `verified` = proven from source; `estimated` = derived from code shape, not measured |
| **Cost class** | `per-frame` / `per-call` / `per-yield` / `per-connection` / `one-shot` |
| **Status** | `open` / `partially addressed` / `addressed` / `n/a` / `needs verification` |
| **Risk** | what breaks if done wrong |

**No profiling data exists in this repo.** There is no `tests/sunc_runner.py`, no `tests/myriad_runner.py`, and no frame-timing capture. Any "ms" figure is **estimated** unless explicitly tied to a recorded measurement. See §5 (Instrumentation).

---

## 1. Current-state summary (what changed)

| Area | Current behavior (verified) | Performance / freeze impact |
|:--|:--|:--|
| **WebSocket connect** (`RBX/environment/libs/websocket/websocket.cpp`, `RBX/environment/env.cpp`) | `__ws_open` starts handshake on a **worker thread** and returns immediately; `__ws_poll` returns `(inbox, done, state, err)` with states `connecting/open/failed/closed`; Lua `WebSocket.connect` **yields via `task.wait`** until `open` or errors on `failed/closed`. WinHTTP timeouts set (resolve/connect 5s, send/recv 10s). Dead echo rewrite maps `echo.websocket.events` → `ws.postman-echo.com/raw` and **prefers `wss://`** for reliability. Reader loop (`pump`) runs on the same worker after handshake completes. | **Addresses the primary visual-freeze cause** from blocking handshake on the game step (connect no longer stalls render while waiting). Reliability improved for the echo case (5/5 connect+echo in a targeted check). `Send` remains synchronous WinHTTP send on caller thread when socket is open; handshake/IO still use a thread per connection. |
| **Dev Console mirror** (`Renderer/ui/ui.cpp`, `Renderer/ui/ui.h`, `RBX/environment/env.cpp`) | Console tab mirrors Roblox Dev Console output: `__devconsole_push` + `LogService:GetLogHistory()` bootstrap and `LogService.MessageOut` subscription enqueue into `g_devconsole_lines` (mutex, cap 5000, trim by 1000). `GetDevConsoleLines()` returns a **copy** of the vector under lock. UI renders by copying lines each draw (and copies again for "Copy all" on click). Captures client-visible log output (including server messages replicated to client LogService/Dev Console). | Correctness/UX improvement; introduces an additional **per-frame copy** pattern similar to P-02 when the Console tab is visible. |
| **RakNet** (`RBX/environment/libs/raknet/*`, `task_scheduler.cpp`) | Capture paused by default, safer vtable restore/copy, SEH-guarded access, watcher polling. | Reduces per-packet overhead/risk on network thread (helps avoid stalls/crashes under load). |
| **Reflection `setscriptable`** (`RBX/environment/libs/reflection/reflection.cpp`) | Per-instance overrides with remembered base descriptor state (no shared-descriptor mutation). | Fixes correctness ("persists between unique instances"); not a hot-path perf change. |
| **Hooks** (`RBX/environment/hooks/hooks.cpp`) | `Run` removed from blocked functions (cUNC/CET compatibility). | Functional fix; no intended perf regression. |
| **Automation** (`auto_sunc.py`, `send_script.py`) | Automated deploy/launch/inject/monitor/report flow. `auto_sunc.py` completion heuristic can misreport timeout (`checks_seen=0`) even when summary is present in log (parser/heuristic issue) — **tooling**, not runtime. | No runtime impact. |

---

## 2. Verified hot paths and issues (current)

| ID | Cost class | Location | Current behaviour (verified) | Status | Suggested fix | Risk |
|:--|:--|:--|:--|:--|:--|:--|
| **P-01** | per-yield | `RBX/environment/yielding/yielding.cpp` | Previously spawned/detached **1 OS thread per yield** worker. | `partially addressed` | Replaced per-yield detached thread with a bounded **4-worker** pool for yield workers (blocking jobs queued), preserving resume/state-generation checks. Limits concurrent blocking operations but reduces thread churn. | med |
| **P-02** | per-frame | `Renderer/ui/ui.cpp`, `managers/logging/logging.h`, `Renderer/ui/ui.h` | **Output tab**: `log_mgr.get_log_info()` returns by value under lock. **Console tab**: `ui::GetDevConsoleLines()` copies dev-console buffer under lock (capped at 5000). | `partially addressed` | Added generation counters (`LogManager::get_generation`, `GetDevConsoleGeneration`) and UI-side cached snapshots refreshed only when generation changes, avoiding per-frame copies when buffers are unchanged (render paths). Copy-all still fetches on demand. Further improvement possible via `for_each`/no-copy iteration. | low |
| **P-03** | per-log | `Utilitys/Log.hpp` | `LogFile::Write` previously called `FlushFileBuffers` on every write inside a critical section. | `addressed` | Removed per-line `FlushFileBuffers`; writes still buffered by OS/file API. Verbosity gating for noisy `LOGF` sites remains a possible follow-up. | low |
| **P-04** | per-frame | `RBX/environment/yielding/yielding.cpp` `RunYield()` | Previously resumed 1 pending task per scheduler step. | `partially addressed` | Batch-drains up to **16** queued resumes per step (preserves order, SEH-wrapped per callback) to improve yield throughput while capping VM re-entry spikes. | low/med |
| **P-05** | per-call | `RBX/environment/hooks/hooks.cpp` `__namecall` | Hook-resolution path can incur expensive lookup/pcall work per method call when hooks exist (closure-keyed records). Past `namecall-regression-*` incidents. | `open` (conservative gate present), `high risk` | Conservative fast path already returns original when `HookRecords` empty; further prefilter/caching must preserve closure-keyed semantics and avoid hook regressions. Not modified further. | **high** |
| **P-06** | per-call | `RBX/environment/hooks/hooks.cpp` | Sanitized method/service keys were heap-allocated and compared via string-keyed sets. | `partially addressed` | `sanitize_key` now returns `std::string_view` (thread-local scratch only when sanitization needed), blocked sets use transparent hash/equality, and hook paths avoid building `std::string` for clean keys (temporary string only for debug/error strings). | low |
| **P-08** | per-yield | `RBX/environment/yielding/yielding.cpp` | `YieldExecution` resolved ScriptContext on every yield. | `partially addressed` | Cache ScriptContext keyed by `(LastDataModel, StateGeneration)`; recompute on change to avoid stale context across state transitions. | med |
| **P-09** | per-iteration | `RBX/environment/libs/instance/instance.cpp` | `getinstances`/`getnilinstances` historically used per-object VM checks and expensive dedup patterns. | `needs verification` | Tag-based fast path only if tag uniformity is verified at runtime; fix stack discipline carefully. | med |
| **P-10** | per-vertex | `RBX/environment/libs/drawingimmediate/drawingimmediate.cpp` | `ReadVector2` performed field lookups for X/Y on each read. | `partially addressed` | Added `lua_isvector`/`lua_tovector` fast path for `ReadVector2` with fallback to X/Y field reads; `ReadColor3` unchanged. | low |
| **P-12** | per-script | `RBX/execution/execution.cpp` | `task.defer` was resolved via global lookups on each script execution. | `partially addressed` | Execution now reuses cached `registry["module.task.defer"]` when available, falling back to resolve+cache (or inline execution if unavailable). Cache lifetime tied to state; no explicit invalidation added. | low |
| **P-13** | per-tick | `RBX/task_scheduler/task_scheduler.cpp` | `ScriptsHandler` drained `Shared::ExecutionRequests` via repeated `erase(begin())` under lock. | `partially addressed` | `ScriptsHandler` now swaps pending requests out of lock and executes after (similar to existing `ExecuteGameImpl` pattern). Other queue/drain sites should be rechecked separately. | low |
| **P-14** | per-poll | `RBX/task_scheduler/task_scheduler.cpp` | `GetJobByTypeName` performed redundant `CheckMemory`/`VirtualQuery` inside `__try` and took `std::string`. | `partially addressed` | Changed signature to `std::string_view`, removed redundant `CheckMemory` reads inside SEH-covered scan path while keeping bounds/type checks; callers unchanged. Other `VirtualQuery` sites outside this function remain. | low |
| **WS-THREAD** | per-connection | `RBX/environment/libs/websocket/websocket.cpp` | **1 worker thread per websocket** performs handshake and then runs receive `pump` loop; `release` joins/detaches appropriately. (Distinct from P-01 yield thread-per-yield.) | `open` (by design tradeoff) | Acceptable for typical counts; for many concurrent sockets consider pooling/IO strategy — only with careful lifetime/error semantics. | med |
| **WS-SEND** | per-send | `RBX/environment/libs/websocket/websocket.cpp` `send` | `WinHttpWebSocketSend` called on caller thread (must be ready/open). Large/frequent sends from game step can stall. | `open` / `needs workload data` | Offload sends if proven problematic; preserve ordering/errors. | med |

> **Note:** P-03/P-04/P-06/P-08–P-14 retain detailed analysis in git/history and prior revisions; re-verify each against the **current** source line numbers before applying, as the codebase has evolved.

---

## 3. Rejected / unsafe proposals (do not reintroduce)

Carried forward from prior audit; still valid unless re-proven against current code:

| Proposal | Why rejected (summary) |
|:--|:--|
| Drain `std::queue` yield queue by `swap` like a vector / uncapped mass resume | `YieldQueue` is a `std::queue`; wrong form won’t compile, and uncapped batch can spike VM re-entries. |
| `getinstances` stack discipline / `luaL_typename=="Instance"` / unproven tag assumptions | Prior sketch had result-table pop bug and dead fallback; tag uniformity must be runtime-verified. |
| Name-based `__namecall` hook prefilter keyed incorrectly / placed incorrectly | `HookRecords` is closure-keyed; naive gating breaks hooks (see `namecall-regression-*`). |
| `/fp:fast`, `/arch:AVX2` “easy wins” | Build is already optimized (`/O2 /GL /LTCG` observed in prior logs); 87+ `__try` sites rely on `/EHa`; portability/precision concerns. |
| Raw `lua_touserdata` cast for vector floats | Luau userdata has headers/tags; correct primitive is `lua_tovector`. |
| Claims of non-existent test harnesses or invented metrics | `tests/sunc_runner.py` / `tests/myriad_runner.py` absent; no timing harness — do not state unmeasured numbers as fact. |

---

## 4. Suggested order of work (current)

| Step | Item | Rationale |
|:--|:--|:--|
| 1 | ~~**P-03** flush-per-log~~ (addressed) | Removed per-line `FlushFileBuffers`. |
| 2 | **P-02** snapshot/iterate for Output + Console dev-mirror buffers | Partially addressed via generation-cached snapshots; consider `for_each`/no-copy. |
| 3 | **P-13/P-14** (partially addressed) | `ScriptsHandler` drain improved; `GetJobByTypeName` now uses `string_view` and drops redundant SEH-protected checks. Other sites still to recheck. |
| 4 | ~~**P-06/P-12**~~ (partially addressed) | Hook key sanitization/lookup allocs reduced; `task.defer` registry-cached in execution path. |
| 5 | **P-08** (partially addressed), **P-04** (partially addressed/capped) | Yield ScriptContext cached by (DataModel,generation); yield drain batched/capped (16). Measure if needed. |
| 6 | **P-01** yield thread pool | Bounds kernel/thread churn under load; validate concurrency expectations. |
| 7 | **WS-SEND/WS-THREAD** only if profiling shows need | Correctness/compat first. |
| 8 | **P-09/P-10** with runtime probes | Requires correctness validation. |
| 9 | **P-05** last | Highest regression risk; gate on evidence and tests. |

---

## 5. Instrumentation (required before quoting numbers)

No in-repo timing harness. To convert estimates into facts:
- Add lightweight timing (e.g. `QueryPerformanceCounter`) around candidates, log at a gated verbosity level to an in-memory ring/buffer and dump on demand/exit (avoid measuring logging overhead with synchronous log flushes).
- Record results under `.maintenance/perf-<date>.json` including deployed DLL hash, build config, scenario, and counts.
- For script-level microbenches, submit over TCP `127.0.0.1:6969`, e.g.:

```lua
local clock = os.clock
local function bench(name, fn, n)
    n = n or 50
    local t0 = clock()
    for _ = 1, n do fn() end
    print(string.format("%-24s %8.3f ms/call", name, (clock() - t0) * 1000 / n))
end

bench("getinstances", function() getinstances() end, 20)
bench("loadstring_small", function() loadstring("local a=1 return a")() end, 200)
```

---

## 6. Verification gate for any change

```text
Pass condition (runtime): Release|x64 builds clean; Module.dll deployed hash recorded;
  loads and runs without new disconnect/crash; sUNC >= 82/86 baseline with no disconnect,
  Myriad >= 140/140 where applicable/evidence exists.
```

- Treat incomplete/disconnected runs as non-results (consistent with `.maintenance/` evidence practice).
- Latest known UNC (this workspace, Baseplate, custom script): **99% (81/82)**, `WebSocket.connect` pass, sole fail due to missing `CoreGui.RobloxGui.Modules.Common.Constants` in that place — use as a regression signal, not a substitute for official sUNC/Myriad evidence.


---

## 8. Completion status (2026-11-15, this session)

- **Addressed/implemented (build + self-test + UNC where applicable):** P-01 bounded yield worker pool (4), P-02 generation-cached UI snapshots (Output/Console), P-03 removed per-line log flush, P-04 batched yield drain (cap 16), P-06 hook key sanitization/transparent lookups, P-07/P-03 follow-up demoted verbose per-yield trace logs (`LOGT`, default off), P-08 ScriptContext cache for yields, P-10 `ReadVector2` vector fast path, P-12 cached `task.defer`, P-13 `ScriptsHandler` swap-drain, P-14 `GetJobByTypeName` `string_view` + reduced redundant SEH-protected checks.
- **Regression checks:** targeted smoke/self-tests per changed area (execution/task/yield/request/websocket/DrawingImmediate/etc.) and repeated UNC runs on Baseplate `95206881` (`Module\.maintenance\unc_test_user.lua`) remained **99% (81/82)** with the same environment-only fail (`CoreGui.RobloxGui.Modules.Common.Constants` missing); `WebSocket.connect` passed.
- **Intentionally not forced (risk):** P-05 deeper `__namecall` optimization and major P-09 `getinstances` rewrite carry correctness/regression risk per audit/history; left unchanged pending stricter proof/measurement. P-15 build config already optimal.
