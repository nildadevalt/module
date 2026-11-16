#include <Utilitys/Globals.hpp>
#include <Utilitys/Utils.hpp>
#include <Utilitys/Log.hpp>
#include <RBX/execution/execution.hpp>
#include <RBX/environment/env.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <RBX/environment/yielding/yielding.hpp>
#include <RBX/environment/libs/raknet/raknet.hpp>
#include <RBX/environment/libs/websocket/websocket.hpp>
#include <RBX/environment/libs/drawing/drawing.hpp>
#include <RBX/environment/libs/drawingimmediate/drawingimmediate.hpp>
#include <RBX/environment/libs/closures/closures.hpp>
#include <RBX/environment/libs/signals/signals.hpp>
#include <RBX/environment/libs/reflection/reflection.hpp>
#include <RBX/environment/libs/oth/oth.hpp>
#include <Renderer/Renderer.hpp>
#include <RBX/environment/env.hpp>
#include <imgui/addons/imgui_notify.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/imgui.h>
#include <imgui/addons/TextEditor.h>
#include <imgui/imgui.h>
#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>

#include <chrono>
#include <string>

#include <imgui/imgui_internal.h>
namespace
{
	thread_local std::string LastSchedulerError;
	thread_local const char* InitializationPhase = "not started";

	void ReportSchedulerError(const char* Message)
	{
		if (LastSchedulerError == Message)
			return;

		LastSchedulerError = Message;
		LOGF("TaskScheduler", "%s", Message);
	}
}

// ══════════════════════════════════════════════════════════════════════════════
// Identity / Capability Helpers
// ══════════════════════════════════════════════════════════════════════════════

void task_scheduler::SetProtoCapabilities(Proto* Root, uintptr_t* Capabilities)
{
	if (!Root)
		return;

	std::vector<Proto*> Pending{ Root };
	while (!Pending.empty())
	{
		Proto* Current = Pending.back();
		Pending.pop_back();
		if (!Current)
			continue;

		Current->userdata = Capabilities;
		if (Current->sizep < 0 || Current->sizep > 100000 || (!Current->p && Current->sizep != 0))
			continue;

		for (int Index = 0; Index < Current->sizep; ++Index)
			if (Current->p[Index])
				Pending.push_back(Current->p[Index]);
	}
}

void task_scheduler::SetThreadCapabilities(lua_State* L, int Level, uintptr_t Capabilities)
{
	if (!L || !L->userdata)
		return;

	L->userdata->identity = Level;
	L->userdata->capabilities = Capabilities;
}

void task_scheduler::SetIdentity(lua_State* Thread, int Identity, bool IsInstance)
{
	if (!Thread) {
		return;
	}
	if (!Thread->userdata) {
		return;
	}

	RobloxExtraSpace* basedData = reinterpret_cast<RobloxExtraSpace*>(Thread->userdata);
	basedData->identity = Identity;
	basedData->capabilities = Roblox::GetCapabilities(&Identity);

	if (IsInstance) {
		try {
			uintptr_t IdentityPointer = *reinterpret_cast<uintptr_t*>(Offsets::RobloxThread::IdentityPtr);
			if (!IdentityPointer || IdentityPointer > 0xFFFFFFFFFFFF) {
				return;
			}
			uintptr_t IdentityStructure = Roblox::GetIdentityStruct(IdentityPointer);
			if (!IdentityStructure) {
				return;
			}
			*reinterpret_cast<int32_t*>(IdentityStructure) = Identity;
			*reinterpret_cast<uintptr_t*>(IdentityStructure + Offsets::Capabilities::Capabilities) =
				Roblox::GetCapabilities(&Identity);
		}
		catch (...) {
		}
	}
}

// ══════════════════════════════════════════════════════════════════════════════
// DataModel / ScriptContext / LuaState Retrieval
// ══════════════════════════════════════════════════════════════════════════════

uintptr_t task_scheduler::GetDataModel()
{
	__try
	{
		uintptr_t FakeDataModel = *reinterpret_cast<uintptr_t*>(Offsets::DataModel::FakeDataModelPointer);
		if (!FakeDataModel)
			return 0;
		return *reinterpret_cast<uintptr_t*>(FakeDataModel + Offsets::DataModel::FakeDataModelToDataModel);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}
}

uintptr_t task_scheduler::GetScriptContext(uintptr_t DataModel)
{
	if (!DataModel)
		return 0;
	__try
	{
		const auto lookup = [DataModel]() { return task_scheduler::GetJobByTypeName("WaitingHybridScriptsJob", DataModel); };
		const uintptr_t Job = lookup();
		if (!Job)
		{
			ReportSchedulerError("ScriptContext lookup: waiting hybrid job is missing");
			return 0;
		}
		const uintptr_t Context = *reinterpret_cast<uintptr_t*>(Job + ClientOffsets::jobs_offsets::scriptContext);
		if (!Context)
		{
			ReportSchedulerError("ScriptContext lookup: job context is null");
			return 0;
		}
		const uintptr_t ContextDataModel = *reinterpret_cast<uintptr_t*>(Context + ClientOffsets::ScriptContext::DataModel);
		if (ContextDataModel != DataModel && ContextDataModel + ClientOffsets::DataModel::InstanceSubobject != DataModel)
		{
			char Message[256]{};
			snprintf(Message, sizeof(Message), "ScriptContext DataModel mismatch: job=%p context=%p contextDataModel=%p expected=%p",
				reinterpret_cast<void*>(Job), reinterpret_cast<void*>(Context),
				reinterpret_cast<void*>(ContextDataModel), reinterpret_cast<void*>(DataModel));
			ReportSchedulerError(Message);
			return 0;
		}
		return Context;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		ReportSchedulerError("ScriptContext lookup raised a memory exception");
		return 0;
	}
}

inline bool CheckMemory(uintptr_t address) {
	if (address < 0x10000 || address > 0x7FFFFFFFFFFF)
		return false;

	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(reinterpret_cast<void*>(address), &mbi, sizeof(mbi)) == 0)
		return false;

	if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD) || mbi.State != MEM_COMMIT)
		return false;

	return true;
}

inline lua_State* GetLuaStateForInstance(uintptr_t sc)
{
    if (!sc) return nullptr;
    __try
    {
        *reinterpret_cast<BOOLEAN*>(sc + Offsets::ExtraSpace::RequireBypass) = TRUE;
        uintptr_t identity = 2;
        uintptr_t script = 0;
        lua_State* state = Roblox::GetLuaStateForInstance(sc, &identity, &script);
        if (!state || !CheckMemory(reinterpret_cast<uintptr_t>(state)) || state->tt != LUA_TTHREAD || !state->global)
            return nullptr;
        return state;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return nullptr;
    }
}

lua_State* task_scheduler::GetLuaState(uintptr_t ScriptContext, uintptr_t Identity, uintptr_t Script)
{
	return Roblox::GetLuaStateForInstance(ScriptContext, &Identity, &Script);
}

// ══════════════════════════════════════════════════════════════════════════════
// GetGameLoaded — reads raw GameLoaded state from DataModel
// ══════════════════════════════════════════════════════════════════════════════

static bool IsGameLoaded(uintptr_t Datamodel)
{
	if (!Datamodel)
		return 0;
	__try
	{
		return *reinterpret_cast<int64_t*>(Datamodel + ClientOffsets::DataModel::PlaceId) > 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}
}

// ══════════════════════════════════════════════════════════════════════════════
// Script Processing
// ══════════════════════════════════════════════════════════════════════════════

int ScriptsHandler(lua_State* L)
{
	lua_State* BoundExecutorThread = static_cast<lua_State*>(
		lua_touserdata(L, lua_upvalueindex(1)));
	if (!BoundExecutorThread || BoundExecutorThread != Shared::ExploitThread)
		return 0;

	// drain the whole queue each tick — one script per step made a batch
	// of N scripts cost N frames
	for (;;)
	{
		std::string Script;
		{
			std::lock_guard<std::mutex> lock(Shared::ExecutionRequestsMutex);
			if (Shared::ExecutionRequests.empty())
				break;
			Script = std::move(Shared::ExecutionRequests.front());
			Shared::ExecutionRequests.erase(Shared::ExecutionRequests.begin());
		}

		if (!Script.empty())
			Execution::ExecuteScript(BoundExecutorThread, std::move(Script));
	}

	Raknet::ProcessCallbacks(BoundExecutorThread);

	Yielding::RunYield();

	return 0;
}



static void RestoreJobVtableSafely()
{
	__try
	{
		if (Shared::LastWaitingHybridScriptsJob && Shared::OriginalHybridJobVtable &&
			CheckMemory(Shared::LastWaitingHybridScriptsJob))
		{
			*reinterpret_cast<uintptr_t**>(Shared::LastWaitingHybridScriptsJob) = Shared::OriginalHybridJobVtable;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

static std::mutex g_StepHookMutex;

void Reset()
{
	Shared::JobStepLocked = true;
	Shared::IsInitializedOnGame = false;
	Shared::StateGeneration.fetch_add(1);

	std::lock_guard<std::mutex> stepLock(g_StepHookMutex);

	RestoreJobVtableSafely();

	Websocket::teardown();
	Drawing::ResetLibraryState();
	Closures::ResetLibraryState();
	Signals::ResetLibraryState();
	Reflection::ResetLibraryState();
	Oth::ResetLibrary();

	Shared::ExploitThread = nullptr;
	Shared::RobloxState = nullptr;
	Shared::ScriptContext = 0;

	{
		std::lock_guard<std::mutex> lock(Shared::YieldQueueMutex);
		std::queue<std::function<void()>> empty;
		Shared::YieldQueue.swap(empty);
	}
	{
		std::lock_guard<std::mutex> lock(Shared::ExecutionRequestsMutex);
		Shared::ExecutionRequests.clear();
	}
	Shared::ExecutorClosures.clear();
	Raknet::ResetHook();

	ImGuiToast toast(ImGuiToastType_Info, 5000);
	toast.set_title("TPHandler");
	toast.set_content("Cleanup done");

	ImGui::InsertNotification(toast);
}

static bool InitializeGameImpl()
{
	if (Shared::IsInitializedOnGame)
		return true;

	InitializationPhase = "ScriptContext lookup";
	uintptr_t ScriptContext =
		task_scheduler::GetScriptContext(Shared::LastDataModel);

	if (!ScriptContext)
		return false;
	InitializationPhase = "native Lua state lookup";
	lua_State* MainThread = nullptr;
	__try { MainThread = GetLuaStateForInstance(ScriptContext); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return false; }

	if (!MainThread)
	{
		ReportSchedulerError("Native Lua state lookup returned no valid thread");
		return false;
	}

	Shared::RobloxState = MainThread;

	Shared::RobloxState = reinterpret_cast<lua_State*>(MainThread);
	if (!Shared::RobloxState)
		return false;

	InitializationPhase = "lua_newthread";
	Shared::ExploitThread = lua_newthread(Shared::RobloxState);
	if (!Shared::ExploitThread)
	{
		ReportSchedulerError("lua_newthread returned null");
		return false;
	}

	InitializationPhase = "sandbox thread";
	luaL_sandboxthread(Shared::ExploitThread);
	InitializationPhase = "environment initialization";
	Environment->initalizeEnvironment(Shared::ExploitThread);

	Shared::IsInitializedOnGame = true;
	LastSchedulerError.clear();
	LOGF("TaskScheduler", "Execution initialized: context=%p main=%p executor=%p",
		reinterpret_cast<void*>(ScriptContext), MainThread, Shared::ExploitThread);

	return true;
}

static bool InitializeGame()
{
	__try
	{
		return InitializeGameImpl();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		Shared::IsInitializedOnGame = false;
		Shared::RobloxState = nullptr;
		Shared::ExploitThread = nullptr;
		char Message[256]{};
		snprintf(Message, sizeof(Message), "Initialization exception 0x%08lX during %s", GetExceptionCode(), InitializationPhase);
		ReportSchedulerError(Message);
		return false;
	}
}

static bool ExecuteGameImpl()
{
	std::vector<std::string> Pending;

	{
		std::lock_guard<std::mutex> lock(Shared::ExecutionRequestsMutex);

		if (Shared::ExploitThread && !Shared::ExecutionRequests.empty())
		{
			Pending.reserve(Shared::ExecutionRequests.size());

			for (std::string& Script : Shared::ExecutionRequests)
				Pending.emplace_back(std::move(Script));

			Shared::ExecutionRequests.clear();
		}
	}

	// Taking one script per step made every queued execution cost a frame, so a
	// batch of N scripts took N frames before the last one even loaded.
	for (std::string& Script : Pending)
	{
		if (!Script.empty())
			Execution::ExecuteScript(Shared::ExploitThread, std::move(Script));
	}

	DrawingImmediate::Render(Shared::ExploitThread);
	if (!Raknet::hooked)
	{
		if (uintptr_t peerJob = task_scheduler::GetJobByTypeName(ClientOffsets::RakNet::JobNameSend, 0))
			Raknet::InitializeHook(peerJob);
	}
	Raknet::DiagScan(Shared::ExploitThread, task_scheduler::GetJobByTypeName(ClientOffsets::RakNet::JobNameRecv, 0));
	Raknet::ProcessCallbacks(Shared::ExploitThread);
	Yielding::RunYield();
	return true;
}

static bool ExecuteGame()
{
	__try
	{
		return ExecuteGameImpl();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		ReportSchedulerError("Script dispatch raised a memory exception");
		return false;
	}
}

// ══════════════════════════════════════════════════════════════════════════════
// stepHook — WHSJ vtable[8] replacement
// ══════════════════════════════════════════════════════════════════════════════

static uintptr_t stepHook(uintptr_t Job, uintptr_t Stats)
{
	if (!Shared::OriginalStepFunction)
		return 0;

	if (!Shared::JobStepLocked)
	{
		std::lock_guard<std::mutex> stepLock(g_StepHookMutex);
		if (!Shared::JobStepLocked)
		{
			if (!Shared::IsInitializedOnGame)
			{
				InitializeGame();
			}
			else
			{
				ExecuteGame();
			}
		}
	}

	return Shared::OriginalStepFunction(Job, Stats);
}

// ══════════════════════════════════════════════════════════════════════════════
// TeleportHandler — WHSJ vtable hook & DataModel lifecycle
// ══════════════════════════════════════════════════════════════════════════════

void task_scheduler::TeleportHandler()
{
	constexpr size_t kJobVtableSlots = 9;

	while (true)
	{
		LOGF("TaskScheduler", "Waiting for a loaded DataModel");
		Shared::IsInitializedOnGame = false;

		uintptr_t DataModel = task_scheduler::GetDataModel();
		while (!IsGameLoaded(DataModel)) {
			Sleep(100);
			DataModel = task_scheduler::GetDataModel();
		}

		Shared::LastDataModel = DataModel;

		uintptr_t GameWHSJ = 0;
		LOGF("TaskScheduler", "Loaded DataModel=%p; waiting for hybrid script job", reinterpret_cast<void*>(Shared::LastDataModel));
		while (!(GameWHSJ = task_scheduler::GetJobByTypeName("WaitingHybridScriptsJob", Shared::LastDataModel))) {
			if (task_scheduler::GetDataModel() != Shared::LastDataModel || !IsGameLoaded(Shared::LastDataModel))
				break;
			Sleep(50);
		}

		if (!GameWHSJ)
		{
			Reset();
			Shared::LastDataModel = 0;
			continue;
		}

		Shared::LastWaitingHybridScriptsJob = GameWHSJ;
		LOGF("TaskScheduler", "Hybrid script job=%p", reinterpret_cast<void*>(GameWHSJ));

		const auto jobVFtable = *reinterpret_cast<uintptr_t**>(Shared::LastWaitingHybridScriptsJob);
		if (Shared::HybridJobVtable == nullptr || Shared::OriginalHybridJobVtable != jobVFtable)
		{
			auto* newVFtable = new uintptr_t[kJobVtableSlots]{};
			for (size_t i = 0; i < kJobVtableSlots; ++i)
			{
				if (!CheckMemory(reinterpret_cast<uintptr_t>(jobVFtable + i)))
					break;
				newVFtable[i] = jobVFtable[i];
			}

			Shared::OriginalHybridJobVtable = jobVFtable;
			Shared::OriginalStepFunction = reinterpret_cast<Shared::JobStep_t>(jobVFtable[8]);
			newVFtable[8] = reinterpret_cast<uintptr_t>(&stepHook);

			delete[] Shared::HybridJobVtable;
			Shared::HybridJobVtable = newVFtable;
		}

		Shared::JobStepLocked = false;

		*reinterpret_cast<uintptr_t**>(Shared::LastWaitingHybridScriptsJob) = static_cast<uintptr_t*>(Shared::HybridJobVtable);
		LOGF("TaskScheduler", "Installed job step callback; original=%p", Shared::OriginalStepFunction);

		while (true)
		{
			uintptr_t currentDatamodel = task_scheduler::GetDataModel();

			if (currentDatamodel != Shared::LastDataModel || !IsGameLoaded(currentDatamodel)) {
				LOGF("TaskScheduler", "DataModel changed/left (old=%p new=%p); resetting",
					reinterpret_cast<void*>(Shared::LastDataModel),
					reinterpret_cast<void*>(currentDatamodel));
				Reset();
				Shared::LastDataModel = 0;
				Shared::LastWaitingHybridScriptsJob = 0;
				break;
			}

			Sleep(10);
		}
	}
}

// ══════════════════════════════════════════════════════════════════════════════
// Execution Queue
// ══════════════════════════════════════════════════════════════════════════════

void task_scheduler::RequestExecution(std::string Script)
{
	std::lock_guard<std::mutex> lock(Shared::ExecutionRequestsMutex);
	Shared::ExecutionRequests.push_back(Script);
	LOGF("Execution", "Queued %zu bytes; pending=%zu initialized=%d", Script.size(), Shared::ExecutionRequests.size(), Shared::IsInitializedOnGame.load() ? 1 : 0);
}

// ══════════════════════════════════════════════════════════════════════════════
// GetJobByTypeName — iterates scheduler job list
// ══════════════════════════════════════════════════════════════════════════════

uintptr_t task_scheduler::GetJobByTypeName(const std::string& TypeName, uintptr_t DataModel)
{
    __try
    {
	uintptr_t task_scheduler =
		*reinterpret_cast<uintptr_t*>(
			Offsets::Scheduler::TaskScheduler
			);

	if (!task_scheduler)
		return 0;

	uintptr_t JobsStart =
		*reinterpret_cast<uintptr_t*>(
			task_scheduler + Offsets::Scheduler::JobStart
			);

	uintptr_t JobsEnd =
		*reinterpret_cast<uintptr_t*>(
			task_scheduler + Offsets::Scheduler::JobEnd
			);

	if (!JobsStart || !JobsEnd || JobsStart >= JobsEnd ||
		(JobsEnd - JobsStart) % 0x10 != 0 || (JobsEnd - JobsStart) / 0x10 > 65536)
		return 0;

	const size_t nameLen = TypeName.size();
	if (nameLen == 0 || nameLen >= 0x100)
		return 0;


	for (uintptr_t Current = JobsStart;
		Current < JobsEnd;
		Current += 0x10)
	{
		uintptr_t Job =
			*reinterpret_cast<uintptr_t*>(Current);

		if (!Job)
			continue;

		const uintptr_t nameAddress = Job + Offsets::Scheduler::JobTypeName;
		if (!CheckMemory(nameAddress + 0x18))
			continue;
		const size_t size = *reinterpret_cast<const size_t*>(nameAddress + 0x10);
		const size_t capacity = *reinterpret_cast<const size_t*>(nameAddress + 0x18);
		if (size != nameLen || capacity < size)
			continue;
		const char* candidate = capacity < 16
			? reinterpret_cast<const char*>(nameAddress)
			: *reinterpret_cast<const char* const*>(nameAddress);
		if (candidate && CheckMemory(reinterpret_cast<uintptr_t>(candidate)) &&
			CheckMemory(reinterpret_cast<uintptr_t>(candidate) + nameLen))
		{
			if (memcmp(candidate, TypeName.c_str(), nameLen) != 0)
				continue;
			if (DataModel)
			{
				const uintptr_t Context = *reinterpret_cast<uintptr_t*>(Job + ClientOffsets::jobs_offsets::scriptContext);
				if (!Context || !CheckMemory(Context + ClientOffsets::ScriptContext::DataModel))
					continue;
				const uintptr_t Owner = *reinterpret_cast<uintptr_t*>(Context + ClientOffsets::ScriptContext::DataModel);
				if (Owner != DataModel && Owner + ClientOffsets::DataModel::InstanceSubobject != DataModel)
					continue;
			}
			return Job;
		}
	}

	return 0;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return 0;
    }
}
