#include <Utilitys/Globals.hpp>
#include <RBX/environment/yielding/yielding.hpp>
#include <Utilitys/Log.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <lualib.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace
{
	struct YieldPool
	{
		std::vector<std::thread> workers;
		std::queue<std::function<void()>> jobs;
		std::mutex m;
		std::condition_variable cv;
		bool stop = false;

		explicit YieldPool(unsigned n = 4)
		{
			workers.reserve(n);
			for (unsigned i = 0; i < n; ++i)
			{
				workers.emplace_back([this]
				{
					for (;;)
					{
						std::function<void()> job;
						{
							std::unique_lock<std::mutex> lk(m);
							cv.wait(lk, [this] { return stop || !jobs.empty(); });
							if (stop && jobs.empty())
								return;
							job = std::move(jobs.front());
							jobs.pop();
						}
						job();
					}
				});
			}
		}

		~YieldPool()
		{
			{
				std::lock_guard<std::mutex> lk(m);
				stop = true;
			}
			cv.notify_all();
			for (auto& w : workers)
			{
				if (w.joinable())
					w.join();
			}
		}

		void submit(std::function<void()> job)
		{
			{
				std::lock_guard<std::mutex> lk(m);
				jobs.push(std::move(job));
			}
			cv.notify_one();
		}
	};

	YieldPool& yield_pool()
	{
		static YieldPool p(4);
		return p;
	}
}

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
	static thread_local std::vector<std::function<void()>> batch;
	batch.clear();

	{
		std::lock_guard<std::mutex> lock(Shared::YieldQueueMutex);
		constexpr size_t kMaxPerStep = 16;
		while (!Shared::YieldQueue.empty() && batch.size() < kMaxPerStep)
		{
			batch.emplace_back(std::move(Shared::YieldQueue.front()));
			Shared::YieldQueue.pop();
		}
	}

	for (const auto& fn : batch)
		SehInvoke(fn);
}

int Yielding::YieldExecution(lua_State* L, const std::function<Yielded()>& YieldingClosure)
{
	static uintptr_t s_cachedContext = 0;
	static uintptr_t s_cachedForModel = 0;
	static uint64_t s_cachedGeneration = ~0ull;

	const uint64_t Generation = Shared::StateGeneration.load();
	const uintptr_t model = Shared::LastDataModel;

	uintptr_t ScriptContext = 0;
	if (s_cachedContext && Generation == s_cachedGeneration && model == s_cachedForModel)
	{
		ScriptContext = s_cachedContext;
	}
	else
	{
		// shared->scriptContext points to an interface subobject, while Resume
		// expects the native ScriptContext owner plus its Face offset.
		ScriptContext = task_scheduler::GetScriptContext(model);
		if (ScriptContext)
		{
			s_cachedContext = ScriptContext;
			s_cachedForModel = model;
			s_cachedGeneration = Generation;
		}
	}

	if (!ScriptContext)
		luaL_error(L, "cannot yield without the active ScriptContext");

	lua_pushthread(L);
	int YieldedThreadRef = lua_ref(L, -1);
	lua_pop(L, 1);
	LOGT("Yielding", "Suspending thread=%p ref=%d", L, YieldedThreadRef);

	yield_pool().submit([=]
	{
		Yielded ResumeFunction = YieldingClosure();
		LOGT("Yielding", "Worker completed for thread=%p", L);
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
				LOGT("Yielding", "Resuming thread=%p context=%p", L, reinterpret_cast<void*>(ScriptContext));
				const int Results = ResumeFunction(L);
				const int Status = Roblox::ScriptContextResume(ScriptContext + Offsets::ExtraSpace::ScriptContextToResume, &YieldState, &YieldingLuaThread, Results, false, nullptr);
				LOGT("Yielding", "Resume returned %d; result=%d", Status, YieldState.result);

				lua_unref(L, YieldedThreadRef);
			});
		}
	});

	return lua_yield(L, NULL);
}
