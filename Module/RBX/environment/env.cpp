#include <RBX/environment/env.hpp>
#include <RBX/environment/hooks/hooks.hpp>

#include <Utilitys/Utils.hpp>
#include <Utilitys/TPHandler.hpp>
#include <Utilitys/Globals.hpp>
#include <RBX/update.hpp>

#include <RBX/environment/libs/drawingimmediate/drawingimmediate.hpp>

#include <cstring>

#include <Renderer/ui/ui.h>

// â”€â”€ Global EnvironmentAPI instance â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
EnvironmentAPI _g_Environment;
EnvironmentAPI* Environment = &_g_Environment;



// â”€â”€ Closure Tracking â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
void EnvironmentAPI::TrackClosure(Closure* c)
{
	if (!c) return;
	Shared::ExecutorClosures.insert(c);

	for (Closure* existing : function_array)
	{
		if (existing == c)
			return;
	}
	function_array.push_back(c);
}

// â”€â”€ make_global â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
static void PushNativeFunction(lua_State* L, lua_CFunction func, const char* debugname)
{
    lua_pushcclosure(L, func, debugname, 0);
    // Native exports use thread globals through the API, not their own
    // environment. Keep that metadata separate from both script sandboxes
    // and the mutable client globals.
    lua_newtable(L);
    lua_setfenv(L, -2);
}

void EnvironmentAPI::make_global(lua_State* L, const char* name, lua_CFunction func, int id)
{

	PushNativeFunction(L, func, name);
	TrackClosure(clvalue(index2addr(L, -1)));
	lua_setglobal(L, name);
}

// ── make_table_function ───────────────────────────────────────
void EnvironmentAPI::make_table_function(lua_State* L, const char* name, lua_CFunction func, int id)
{
	PushNativeFunction(L, func, name);
	TrackClosure(clvalue(index2addr(L, -1)));
	lua_setfield(L, -2, name);
}

// ── make_aliases ──────────────────────────────────────────────
void EnvironmentAPI::make_aliases(lua_State* L, lua_CFunction func, int id, std::initializer_list<const char*> aliases)
{
	for (const char* alias : aliases)
	{
		PushNativeFunction(L, func, alias);
		TrackClosure(clvalue(index2addr(L, -1)));
		lua_setglobal(L, alias);
	}
}

// â”€â”€ Reset State â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
void EnvironmentAPI::ResetState()
{
	Shared::ExecutorClosures.clear();
	function_array.clear();
	{
		std::lock_guard<std::mutex> lock(Shared::YieldQueueMutex);
		std::queue<std::function<void()>> empty;
		Shared::YieldQueue.swap(empty);
	}
	{
		std::lock_guard<std::mutex> lock(Shared::ExecutionRequestsMutex);
		Shared::ExecutionRequests.clear();
	}
	DrawingImmediate::ClearAll();
}

// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
//  Library includes â€” each library's .hpp declares register_lib
// â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
#include <RBX/environment/libs/http/http.hpp>
#include <RBX/environment/libs/cache/cache.hpp>
#include <RBX/environment/libs/crypt/crypt.hpp>
#include <RBX/environment/libs/input/input.hpp>
#include <RBX/environment/libs/debug/debug.hpp>
#include <RBX/environment/libs/script/script.hpp>
#include <RBX/environment/libs/closures/closures.hpp>
#include <RBX/environment/libs/instance/instance.hpp>
#include <RBX/environment/libs/metatable/metatable.hpp>
#include <RBX/environment/libs/filesystem/filesystem.hpp>
#include <RBX/environment/libs/miscellaneous/miscellaneous.hpp>
#include <RBX/environment/libs/signals/signals.hpp>
#include <RBX/environment/libs/regex/regex.hpp>
#include <RBX/environment/libs/filedialog/filedialog.hpp>
#include <RBX/environment/libs/bit/bit.hpp>
#include <RBX/environment/libs/drawing/drawing.hpp>
#include <RBX/environment/libs/reflection/reflection.hpp>
#include <RBX/environment/libs/oth/oth.hpp>
#include <RBX/environment/libs/raknet/raknet.hpp>
#include <RBX/environment/libs/drawingimmediate/drawingimmediate.hpp>
#include <RBX/environment/libs/luastateproxy/luastateproxy.hpp>
#include <RBX/environment/libs/actor/actor.hpp>
#include <RBX/environment/libs/websocket/websocket.hpp>
#include <RBX/environment/libs/rconsole/rconsole.hpp>


#define LuaState L


namespace module::core::environment
{
  
    namespace Bit              = ::Bit;
    namespace Cache            = ::Cache;
    namespace Closures         = ::Closures;
    namespace Crypt            = ::Crypt;
    namespace Debug            = ::Debug;
    namespace Drawing          = ::Drawing;
    namespace DrawingImmediate = ::DrawingImmediate;
    namespace FileDialog       = ::FileDialog;
    namespace Filesystem       = ::Filesystem;
    namespace Input            = ::Input;
    namespace Instance         = ::Instance;
    namespace LuaStateProxy    = ::LuaStateProxy;
    namespace Metatable        = ::Metatable;
    namespace Miscellaneous    = ::Miscellaneous;
    namespace Oth              = ::Oth;
    namespace Raknet           = ::Raknet;
    namespace RConsole         = ::RConsole;
    namespace Reflection       = ::Reflection;
    namespace Regex            = ::Regex;
    namespace Script           = ::Script;
    namespace Signals          = ::Signals;
    namespace Websocket        = ::Websocket;
    namespace Actor            = ::Actor;
}


LUAU_FASTFLAG(LuauCIProto)
void ApplyLuauFlags()
{
    FFlag::LuauCIProto.value = true;
}

static void ClearInstanceFastPaths(lua_State* L)
{
    lua_getglobal(L, "game");
    const int tag = lua_userdatatag(L, -1);
    lua_pop(L, 1);
    if (tag < 0 || tag >= UTAG_INTERNAL_LIMIT)
        return;
    __try
    {
        // Keep the VM's rooted metamethod values and native property paths.
        // Instance method calls need metatable dispatch for explicit hooks.
        auto& direct = L->global->udatadirect[tag];
        direct.index = nullptr;
        direct.newindex = nullptr;
        direct.namecall = nullptr;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        OutputDebugStringA("[ENV] Instance fast-path setup failed\n");
    }
}


void EnvironmentAPI::initalizeEnvironment(lua_State* L)
{
    ClearInstanceFastPaths(L);

    Closures::ResetLibraryState();
    Signals::ResetLibraryState();
    Reflection::ResetLibraryState();

    Oth::ResetLibrary();
    Websocket::teardown();
    Shared::RobloxPcallRef = LUA_NOREF;

    lua_getglobal(L, "pcall");
    if (lua_isfunction(L, -1))
        Shared::RobloxPcallRef = lua_ref(L, -1);
    lua_pop(L, 1);
    ResetState();

    module::core::environment::c_http::register_lib(LuaState);
    module::core::environment::Crypt::register_lib(LuaState);
    module::core::environment::Input::register_lib(LuaState);
    module::core::environment::Debug::register_lib(LuaState);
    module::core::environment::Cache::register_lib(LuaState);
    module::core::environment::Signals::register_lib(LuaState);
    module::core::environment::Script::register_lib(LuaState);
    module::core::environment::Drawing::register_lib(LuaState);
    module::core::environment::Closures::register_lib(LuaState);
    module::core::environment::Instance::register_lib(LuaState);
    module::core::environment::Metatable::register_lib(LuaState);
    module::core::environment::Filesystem::register_lib(LuaState);
    module::core::environment::Miscellaneous::register_lib(LuaState);
    module::core::environment::Regex::register_lib(LuaState);
    module::core::environment::FileDialog::register_lib(LuaState);
    module::core::environment::Bit::register_lib(LuaState);
    module::core::environment::Reflection::register_lib(LuaState);
    module::core::environment::Oth::register_lib(LuaState);
    module::core::environment::Raknet::register_lib(LuaState);
    module::core::environment::DrawingImmediate::register_lib(LuaState);
    module::core::environment::LuaStateProxy::register_lib(LuaState);
    module::core::environment::Actor::register_lib(LuaState);

    module::core::environment::RConsole::register_lib(LuaState);
    module::core::environment::Websocket::register_lib(LuaState);
    initialize_hooks(LuaState);




    lua_pushvalue(L, LUA_GLOBALSINDEX);
    lua_setglobal(L, ("_G"));

    lua_newtable(L);
    lua_setglobal(L, ("shared"));

    // Signal library (Lua bootstrap)
    std::string signal = R"LUA(local Signal = {}
Signal.__index = Signal

function Signal.new()
    local self = setmetatable({}, Signal)
    self._connections = {}
    return self
end

function Signal:Connect(callback)
    local conn = {
        _callback = callback,
        _signal = self,
        _connected = true,
    }
    conn.Disconnect = function(c)
        c._connected = false
        for i, v in ipairs(self._connections) do
            if v == c then
                table.remove(self._connections, i)
                break
            end
        end
    end
    table.insert(self._connections, conn)
    return conn
end

function Signal:Once(callback)
    local conn
    conn = self:Connect(function(...)
        conn:Disconnect()
        callback(...)
    end)
    return conn
end

function Signal:Fire(...)
    for _, conn in ipairs(table.clone(self._connections)) do
        if conn._connected then
            task.spawn(conn._callback, ...)
        end
    end
end

function Signal:Wait()
    local thread = coroutine.running()
    local conn
    conn = self:Connect(function(...)
        conn:Disconnect()
        task.spawn(thread, ...)
    end)
    return coroutine.yield()
end

getgenv().Signal = Signal)LUA";
    


   

                    std::string websocketScript = R"LUA(
do
    local WebSocket = {}

    function WebSocket.connect(url)
        local id = __ws_open(url)

        local function makeEvent()
            local ok, inst = pcall(function()
                return Instance.new("BindableEvent")
            end)
            if ok and inst and inst.Event then
                return {
                    Event = inst.Event,
                    Fire = function(self, ...)
                        inst:Fire(...)
                    end
                }
            end
            local sig = Signal.new()
            return {
                Event = sig,
                Fire = function(self, ...)
                    sig:Fire(...)
                end
            }
        end

        local messages = makeEvent()
        local closed = makeEvent()

        local socket = {}
        socket.OnMessage = messages.Event
        socket.OnClose = closed.Event
        socket._id = id

        function socket.Send(self, message)
            if self ~= socket then
                message = self
            end
            if type(message) == "string" then
                __ws_send(id, message)
            else
                __ws_send(id, tostring(message))
            end
        end

        function socket.Close(self)
            __ws_close(id)
        end

        socket.send = socket.Send
        socket.close = socket.Close

        -- Wait for async handshake (yields so game step/render is not blocked).
        while true do
            local inbox, done, state, err = __ws_poll(id)
            if state == "open" then
                break
            elseif state == "failed" then
                __ws_close(id)
                error(err or "the websocket handshake failed", 0)
            elseif state == "closed" or done then
                __ws_close(id)
                error(err or "the websocket handshake failed", 0)
            end
            task.wait(0.03)
        end

        task.spawn(function()
            while true do
                local inbox, done, state, err = __ws_poll(id)

                if inbox then
                    for index = 1, #inbox do
                        messages:Fire(inbox[index])
                    end
                end

                if done or state == "closed" or state == "failed" then
                    closed:Fire()
                    break
                end

                task.wait(0.03)
            end
        end)

        return socket
    end

    WebSocket.Connect = WebSocket.connect
    WebSocket.New = WebSocket.connect
    WebSocket.new = WebSocket.connect

    local genv = getgenv and getgenv() or _G
    genv.WebSocket = WebSocket
    genv.websocket = WebSocket

    if type(genv.syn) == "table" then
        genv.syn.websocket = WebSocket
    else
        genv.syn = { websocket = WebSocket }
    end

    _G.WebSocket = WebSocket
    _G.websocket = WebSocket
    shared.WebSocket = WebSocket
    shared.websocket = WebSocket
end
)LUA";


 
    task_scheduler::RequestExecution(signal);
    task_scheduler::RequestExecution(websocketScript);

    // Dev Console mirror: capture Roblox LogService.MessageOut into ImGui Console tab
    Environment->make_global(LuaState, "__devconsole_push", [](lua_State* L) -> int {
        size_t len = 0;
        const char* msg = luaL_optlstring(L, 1, "", &len);
        const int msgType = (int)luaL_optinteger(L, 2, 0);
        module::core::graphics::ui::PushDevConsoleLine(std::string(msg ? msg : "", len), msgType);
        return 0;
    }, 0);

    std::string devConsoleScript = R"LUA(
do
    if getgenv().__devconsole_hooked then return end
    getgenv().__devconsole_hooked = true
    task.spawn(function()
        local ok, LogService = pcall(game.GetService, game, "LogService")
        if not ok or not LogService then return end
        local function push(msg, mt)
            local t = 0
            pcall(function()
                if typeof(mt) == "EnumItem" then t = mt.Value else t = tonumber(mt) or 0 end
            end)
            pcall(__devconsole_push, tostring(msg or ""), t)
        end
        pcall(function()
            local hist = LogService:GetLogHistory()
            if type(hist) == "table" then
                for _, e in ipairs(hist) do
                    local m = e.message or e.Message or ""
                    local mt = e.messageType or e.MessageType or 0
                    push(m, mt)
                end
            end
        end)
        pcall(function()
            LogService.MessageOut:Connect(function(msg, mt)
                push(msg, mt)
            end)
        end)
    end)
end
)LUA";
    task_scheduler::RequestExecution(devConsoleScript);

    ApplyLuauFlags();
}
