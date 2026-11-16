#include <Utilitys/Globals.hpp>
#include <RBX/environment/yielding/yielding.hpp>
#include <Utilitys/Log.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <lualib.h>

static void SehInvoke(const std::function<void()>& fn)
{
	__try
	{
		fn();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOGF("Yielding", "Resume callback raised exception 0x%08lX", GetExceptionCode());
	}
}

void Yielding::RunYield()
{
	std::function<void()> YieldingRequest;
	{
		std::lock_guard<std::mutex> lock(Shared::YieldQueueMutex);
		if (Shared::YieldQueue.empty())
			return;

		YieldingRequest = Shared::YieldQueue.front();
		Shared::YieldQueue.pop();
	}

	SehInvoke(YieldingRequest);
}

int Yielding::YieldExecution(lua_State* L, const std::function<Yielded()>& YieldingClosure)
{
	const uint64_t Generation = Shared::StateGeneration.load();
	// shared->scriptContext points to an interface subobject, while Resume
	// expects the native ScriptContext owner plus its Face offset.
	const uintptr_t ScriptContext = task_scheduler::GetScriptContext(Shared::LastDataModel);
	if (!ScriptContext)
		luaL_error(L, "cannot yield without the active ScriptContext");
	lua_pushthread(L);
	int YieldedThreadRef = lua_ref(L, -1);
	lua_pop(L, 1);
	LOGF("Yielding", "Suspending thread=%p ref=%d", L, YieldedThreadRef);

	std::thread([=]
	{
		Yielded ResumeFunction = YieldingClosure();
		LOGF("Yielding", "Worker completed for thread=%p", L);
		if (Generation != Shared::StateGeneration.load())
			return;

		{
			std::lock_guard<std::mutex> lock(Shared::YieldQueueMutex);
			Shared::YieldQueue.emplace([=]() -> void
			{
				if (Generation != Shared::StateGeneration.load())
					return;
				RBX::LiveThreadRef YieldingLuaThreadObject{};
				YieldingLuaThreadObject.thread = L;
				YieldingLuaThreadObject.thread_ref = YieldedThreadRef;
				RBX::LiveThreadRef* YieldingLuaThread = &YieldingLuaThreadObject;

				RBX::debugger_result_t YieldState{};
				LOGF("Yielding", "Resuming thread=%p context=%p", L, reinterpret_cast<void*>(ScriptContext));

				// The resume callback must never escape: ScriptContextResume has to be
				// reached or the yielding thread stays suspended forever and whatever
				// it was running never finishes (this stranded whole UNC runs). A
				// throwing resume is converted into a normal Lua error value so the
				// thread still unwinds and its caller sees a failure instead of a hang.
				int Results = 0;
				__try
				{
					Results = ResumeFunction(L);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					LOGF("Yielding", "Resume body raised 0x%08lX; resuming with error", GetExceptionCode());
					lua_pushstring(L, "yielding: resume callback raised a native exception");
					Results = 1;
				}

				const int Status = Roblox::ScriptContextResume(ScriptContext + Offsets::ExtraSpace::ScriptContextToResume, &YieldState, &YieldingLuaThread, Results, false, nullptr);
				LOGF("Yielding", "Resume returned %d; result=%d", Status, YieldState.result);

				lua_unref(L, YieldedThreadRef);
			});
		}
	}).detach();

	return lua_yield(L, NULL);
}
