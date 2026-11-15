#include <Utilitys/Globals.hpp>
#include <RBX/execution/execution.hpp>
#include <Utilitys/Log.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>

class BytecodeEncoder : public Luau::BytecodeEncoder
{
	inline void encode(uint32_t* data, size_t count) override
	{
		for (auto i = 0; i < count;)
		{
			uint8_t Opcode = LUAU_INSN_OP(data[i]);
			const auto LookupTable = reinterpret_cast<BYTE*>(Offsets::OpcodeLookupTable);
			uint8_t FinalOpcode = static_cast<uint8_t>(Opcode * ClientOffsets::Bytecode::OpcodeWireMultiplier);
			FinalOpcode = LookupTable[FinalOpcode];

			data[i] = (FinalOpcode) | (data[i] & ~0xFF);
			i += Luau::getOpLength(static_cast<LuauOpcode>(Opcode));
		}
	}
};

std::string Execution::CompileScript(std::string Source)
{
	auto BytecodeEncoding = BytecodeEncoder();
	static const char* CommonGlobals[] = { "Game", "Workspace", "game", "plugin", "script", "shared", "workspace", "_G", "_ENV", nullptr };

	Luau::CompileOptions Options;
	Options.debugLevel = (Source.size() >= 65536) ? 1 : 2;
	Options.optimizationLevel = 1;
	Options.mutableGlobals = CommonGlobals;
	Options.vectorLib = "Vector3";
	Options.vectorCtor = "new";
	Options.vectorType = "Vector3";

	return Luau::compile(Source, Options, {}, &BytecodeEncoding);
}

void Execution::ExecuteScript(lua_State* L, std::string Script)
{
	if (!L || Script.empty())
		return;

	int OriginalTop = lua_gettop(L);
	LOGF("Execution", "Compiling %zu bytes", Script.size());
	lua_State* ExecutionThread = lua_newthread(L);
	if (!ExecutionThread)
	{
		LOGF("Execution", "lua_newthread returned null");
		lua_settop(L, OriginalTop);
		return;
	}

	// Each execution gets writable locals inheriting the persistent executor
	// environment, so script assignments do not pollute getgenv().
	luaL_sandboxthread(ExecutionThread);
	task_scheduler::SetThreadCapabilities(ExecutionThread, 8, MaxCapabilities);

	std::string Bytecode = Execution::CompileScript(Script);
	int LoadStatus = luau_load(ExecutionThread, "@Script", Bytecode.c_str(), Bytecode.length(), NULL);
	if (LoadStatus != LUA_OK)
	{
		const char* Error = lua_tostring(ExecutionThread, -1);
		LOGF("Execution", "Bytecode load failed (%d): %s", LoadStatus, Error ? Error : "non-string error");
		lua_pop(ExecutionThread, 1);
		lua_settop(L, OriginalTop);
		return;
	}

	Closure* Closure = clvalue(luaA_toobject(ExecutionThread, -1));
	LOGF("Execution", "Loaded %zu byte bytecode", Bytecode.size());
	task_scheduler::SetProtoCapabilities(Closure->l.p, &MaxCapabilities);

	// defer through the task scheduler so yields/waits inside the script
	// keep working; fall back to a direct call when task.defer is missing
	lua_getfield(ExecutionThread, LUA_REGISTRYINDEX, "module.task.defer");
	if (!lua_isfunction(ExecutionThread, -1))
	{
		lua_pop(ExecutionThread, 1);
		lua_getglobal(ExecutionThread, "task");
		lua_getfield(ExecutionThread, -1, "defer");
		if (lua_isfunction(ExecutionThread, -1))
		{
			lua_pushvalue(ExecutionThread, -1);
			lua_setfield(ExecutionThread, LUA_REGISTRYINDEX, "module.task.defer");
			lua_remove(ExecutionThread, -2); // remove task table, keep defer
		}
		else
		{
			lua_pop(ExecutionThread, 2); // pop non-function defer, task
		}
	}

	if (lua_isfunction(ExecutionThread, -1))
	{
		lua_insert(ExecutionThread, -2);

		int Status = lua_pcall(ExecutionThread, 1, 0, 0);
		if (Status != LUA_OK)
		{
			const char* Error = lua_tostring(ExecutionThread, -1);
			LOGF("Execution", "task.defer failed (%d): %s", Status, Error ? Error : "non-string error");
			lua_pop(ExecutionThread, 1);
		}
		else
			LOGF("Execution", "Dispatched through task.defer");
	}
	else
	{
		// no task library in this state — run the chunk inline so the
		// script still executes instead of silently doing nothing
		int Status = lua_pcall(ExecutionThread, 0, 0, 0);
		if (Status != LUA_OK)
		{
			const char* Error = lua_tostring(ExecutionThread, -1);
			LOGF("Execution", "Direct execution failed (%d): %s", Status, Error ? Error : "non-string error");
			lua_pop(ExecutionThread, 1);
		}
		else
			LOGF("Execution", "Direct execution completed");
	}

	lua_settop(L, OriginalTop);
}
