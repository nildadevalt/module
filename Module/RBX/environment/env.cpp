#include <RBX/environment/env.hpp>
#include <RBX/environment/hooks/hooks.hpp>

#include <Utilitys/Utils.hpp>
#include <Utilitys/TPHandler.hpp>
#include <Utilitys/Globals.hpp>
#include <RBX/update.hpp>

#include <RBX/environment/libs/drawingimmediate/drawingimmediate.hpp>

#include <cstring>

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
    // The dump marks its FFlags read-only, so writing is only legal when the
    // vendored Luau was built with writable flags.
#if defined(LUAU_FFLAG_WRITABLE)
    FFlag::LuauCIProto.value = true;
#endif
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


// ── Volt-compatible snake_case aliases ───────────────────────────
// Other executors expose every API twice (writefile and write_file, mouse1click
// and mouse1_click). Scripts written against those names simply fail here, so
// each existing global is mirrored under its snake_case spelling.
static void RegisterSnakeCaseAliases(lua_State* L)
{
    static const char* kAliases[][2] = {
        { "append_file", "appendfile" },
        { "append_file_async", "appendfileasync" },
        { "can_signal_replicate", "cansignalreplicate" },
        { "check_caller", "checkcaller" },
        { "check_closure", "checkclosure" },
        { "check_parallel", "checkparallel" },
        { "clone_ref", "cloneref" },
        { "clone_reference", "clonereference" },
        { "compare_function", "comparefunction" },
        { "compare_functions", "comparefunctions" },
        { "compare_instances", "compareinstances" },
        { "del_file", "delfile" },
        { "del_folder", "delfolder" },
        { "do_file", "dofile" },
        { "filter_gc", "filtergc" },
        { "fire_click_detector", "fireclickdetector" },
        { "fire_proximity_prompt", "fireproximityprompt" },
        { "fire_signal", "firesignal" },
        { "fire_touch_interest", "firetouchinterest" },
        { "get_actor_states", "getactorstates" },
        { "get_all_threads", "getallthreads" },
        { "get_calling_script", "getcallingscript" },
        { "get_connections", "getconnections" },
        { "get_custom_asset", "getcustomasset" },
        { "get_executor_name", "getexecutorname" },
        { "get_executor_version", "getexecutorversion" },
        { "get_fps_cap", "getfpscap" },
        { "get_function_hash", "getfunctionhash" },
        { "get_gc", "getgc" },
        { "get_hui", "gethui" },
        { "get_hwid", "gethwid" },
        { "get_instances", "getinstances" },
        { "get_loaded_modules", "getloadedmodules" },
        { "get_menv", "getmenv" },
        { "get_metafield", "getmetafield" },
        { "get_namecall_method", "getnamecallmethod" },
        { "get_nil_instances", "getnilinstances" },
        { "get_raw_metatable", "getrawmetatable" },
        { "get_reg", "getreg" },
        { "get_render_stepped_list", "getrendersteppedlist" },
        { "get_running_scripts", "getrunningscripts" },
        { "get_script_bytecode", "getscriptbytecode" },
        { "get_script_closure", "getscriptclosure" },
        { "get_script_from_thread", "getscriptfromthread" },
        { "get_script_function", "getscriptfunction" },
        { "get_script_globals", "getscriptglobals" },
        { "get_script_hash", "getscripthash" },
        { "get_script_thread", "getscriptthread" },
        { "get_scripts", "getscripts" },
        { "get_senv", "getsenv" },
        { "get_signal_arguments", "getsignalarguments" },
        { "get_signal_arguments_info", "getsignalargumentsinfo" },
        { "get_signal_whitelist", "getsignalwhitelist" },
        { "get_simulation_radius", "getsimulationradius" },
        { "get_tenv", "gettenv" },
        { "get_thread_context", "getthreadcontext" },
        { "has_metafield", "hasmetafield" },
        { "has_metatable", "hasmetatable" },
        { "hook_func", "hookfunc" },
        { "hook_meta_method", "hookmetamethod" },
        { "hook_metamethod", "hookmetamethod" },
        { "hook_signal", "hooksignal" },
        { "identify_executor", "identifyexecutor" },
        { "is_c_closure", "iscclosure" },
        { "is_cclosure", "iscclosure" },
        { "is_executor_closure", "isexecutorclosure" },
        { "is_executor_function", "isexecutorfunction" },
        { "is_file", "isfile" },
        { "is_folder", "isfolder" },
        { "is_function_hooked", "isfunctionhooked" },
        { "is_game_active", "isgameactive" },
        { "is_hidden_stack", "ishiddenstack" },
        { "is_l_closure", "islclosure" },
        { "is_lclosure", "islclosure" },
        { "is_network_owner", "isnetworkowner" },
        { "is_new_c_closure", "isnewcclosure" },
        { "is_newc_closure", "isnewcclosure" },
        { "is_newcclosure", "isnewcclosure" },
        { "is_our_closure", "isourclosure" },
        { "is_parallel", "isparallel" },
        { "is_rbx_active", "isrbxactive" },
        { "is_readonly", "isreadonly" },
        { "is_untouched", "isuntouched" },
        { "is_valid_level", "isvalidlevel" },
        { "key_click", "keyclick" },
        { "key_press", "keypress" },
        { "key_release", "keyrelease" },
        { "list_files", "listfiles" },
        { "load_file", "loadfile" },
        { "load_file_async", "loadfileasync" },
        { "lz4_compress", "lz4compress" },
        { "lz4_decompress", "lz4decompress" },
        { "make_folder", "makefolder" },
        { "make_readonly", "makereadonly" },
        { "make_writeable", "makewriteable" },
        { "message_box", "messagebox" },
        { "mouse1_click", "mouse1click" },
        { "mouse1_press", "mouse1press" },
        { "mouse1_release", "mouse1release" },
        { "mouse2_click", "mouse2click" },
        { "mouse2_press", "mouse2press" },
        { "mouse2_release", "mouse2release" },
        { "mouse_move_abs", "mousemoveabs" },
        { "mouse_move_rel", "mousemoverel" },
        { "mouse_scroll", "mousescroll" },
        { "new_c_closure", "newcclosure" },
        { "new_cclosure", "newcclosure" },
        { "read_file", "readfile" },
        { "read_file_async", "readfileasync" },
        { "replace_closure", "replaceclosure" },
        { "replicate_signal", "replicatesignal" },
        { "restore_func", "restorefunc" },
        { "restore_function", "restorefunction" },
        { "save_instance", "saveinstance" },
        { "set_clipboard", "setclipboard" },
        { "set_fps_cap", "setfpscap" },
        { "set_hidden_stack", "sethiddenstack" },
        { "set_namecall_method", "setnamecallmethod" },
        { "set_raw_metatable", "setrawmetatable" },
        { "set_rbx_clipboard", "setrbxclipboard" },
        { "set_readonly", "setreadonly" },
        { "set_simulation_radius", "setsimulationradius" },
        { "set_thread_context", "setthreadcontext" },
        { "set_untouched", "setuntouched" },
        { "write_file", "writefile" },
        { "write_file_async", "writefileasync" },
    };

    for (const auto& alias : kAliases)
    {
        lua_getglobal(L, alias[1]);
        if (lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            continue;
        }
        lua_setglobal(L, alias[0]);
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
    -- task.spawn runs synchronously, so the callback can fire before
    -- Connect() has returned and assigned `conn`. Disconnect through the
    -- signal instead of capturing a possibly-nil upvalue.
    local conn
    conn = self:Connect(function(...)
        local current = conn
        if current then
            current:Disconnect()
        end
        callback(...)
    end)
    return conn
end

function Signal:Fire(...)
    -- copy first: callbacks may connect/disconnect while iterating
    local snapshot = table.clone(self._connections)
    for _, conn in ipairs(snapshot) do
        if conn._connected then
            task.spawn(conn._callback, ...)
        end
    end
end

function Signal:Wait()
    -- Subscribe immediately, then park through task.wait so the fire can be
    -- delivered by the scheduler. Firing from another thread resumes here
    -- because task.wait yields through the scheduler's own yield/resume.
    local pack = nil
    local conn = self:Connect(function(...)
        pack = table.pack(...)
    end)

    repeat task.wait() until pack ~= nil

    conn:Disconnect()
    return table.unpack(pack, 1, pack.n)
end

getgenv().Signal = Signal)LUA";
    


   

                    std::string websocketScript = R"LUA(
do
    local WebSocket = {}

    function WebSocket.connect(url)
        local id, openErr = __ws_open(url)
        if type(id) ~= "number" then
            error(tostring(openErr or "WebSocket.connect failed"), 2)
        end

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

        task.spawn(function()
            while true do
                local inbox, done = __ws_poll(id)

                if inbox then
                    for index = 1, #inbox do
                        messages:Fire(inbox[index])
                    end
                end

                if done then
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


 
    // Injected last so it runs after the libraries above exist: prints a
    // one-line banner whenever the module (re)initialises on a place, so the
    // console shows the executor is alive without needing to run a script.
    std::string bannerScript = R"LUA(
pcall(task.defer, function()
    local out = (typeof(print) == "function") and print or warn
    pcall(out, "Module are working")
end)
)LUA";

    // setstackhidden cannot be enforced by patching the VM here: the script
    // state is Roblox's own Luau, so our fork's frame filter is never
    // consulted. Wrap the three library functions that expose frames and
    // decide visibility from a Lua-side set instead. Installed eagerly (not on
    // first setstackhidden) so a reference captured before hiding - which is
    // exactly what a probe does - is still the wrapper.
    std::string hiddenFramesScript = R"LUA(
pcall(function()
    local raw_info, raw_tb = debug.info, debug.traceback
    local ishidden = isstackhidden

    -- Identity check only: isstackhidden takes a *function*, so it is immune
    -- to the frame shifts that make level-based queries unreliable from
    -- inside a wrapper.
    local function hid(fn)
        if fn == nil then return false end
        local ok, v = pcall(ishidden, fn)
        return ok and v == true
    end

    -- debug.info stays untouched on purpose: the executor itself (and several
    -- detection tests) resolves a function by calling debug.info(1, "f") from a
    -- compiled chunk, and a wrapper answers with the wrapper instead, which
    -- broke hookfunction and getrawmetatable. Only the traceback is rewritten.
    local info = raw_info

    local function traceback(msg, level)
        local any = false
        for a = 1, 250 do
            local fn = raw_info(a, "f")
            if fn == nil then break end
            if hid(fn) then any = true break end
        end
        if not any then return raw_tb(msg, level) end
        local out = typeof(msg) == "string" and (msg .. "\n") or ""
        for a = 1, 250 do
            local fn = raw_info(a, "f")
            if fn == nil then break end
            if not hid(fn) then
                local pos = raw_info(a, "sl")
                if pos and pos ~= "" then out = out .. pos .. "\n" end
            end
        end
        return out
    end

    -- debug is read-only in the game env; flip it just long enough to swap
    -- the entries. setreadonly returns nothing, so always restore to true.
    pcall(function() setreadonly(debug, false) end)
    debug.info = info
    debug.traceback = traceback
    pcall(function() setreadonly(debug, true) end)

    -- 'error' is intentionally left alone: it would have to walk the whole
    -- stack for every raise, and skipping the source position is not worth
    -- that cost on the hot path.
end)
)LUA";

    // Input through Roblox's own VirtualInputManager. Raw SendInput can only
    // reach a foreground window, and Windows refuses to let a background
    // process steal focus (SetForegroundWindow returns FALSE), so every click
    // and key press landed in whatever app the user had in front. VirtualInput-
    // Manager feeds the client's input pipeline directly: IsMouseButtonPressed
    // turns true, InputBegan fires and GetMouseLocation reports the exact
    // position, all while the client sits behind another window.
    std::string inputScript = R"LUA(
pcall(task.defer, function()
    local genv = getgenv()
    if genv.__rxs_input_vim then
        return
    end

    local vim
    pcall(function()
        vim = game:GetService("VirtualInputManager")
    end)
    if typeof(vim) ~= "Instance" then
        return
    end
    genv.__rxs_input_vim = true

    local uis = game:GetService("UserInputService")

    -- A held button leaks into the next test: debUNC refuses to run any input
    -- test while IsMouseButtonPressed is true, so every click releases inside
    -- the same call and the watchdog clears anything left behind.
    local buttonTypes = {
        Enum.UserInputType.MouseButton1,
        Enum.UserInputType.MouseButton2,
        Enum.UserInputType.MouseButton3,
    }

    local orig = {}
    for _, name in ipairs({
        "mousemoveabs", "mousemoverel", "mousescroll",
        "mouse1click", "mouse1press", "mouse1release",
        "mouse2click", "mouse2press", "mouse2release",
        "keypress", "keyrelease", "keyclick",
    }) do
        orig[name] = genv[name]
    end

    local function cursor()
        local p = uis:GetMouseLocation()
        local x, y = math.floor(p.X + 0.5), math.floor(p.Y + 0.5)
        if x < 0 or y < 0 then
            local vp = workspace.CurrentCamera.ViewportSize
            x, y = math.floor(vp.X / 2), math.floor(vp.Y / 2)
        end
        return x, y
    end

    -- Feeding the client's own input pipeline can flip the camera into a locked
    -- mouse mode, and debUNC refuses to test any input while MouseBehavior is
    -- not Default, so every injected action hands the mouse straight back.
    local function unlock()
        if uis.MouseBehavior ~= Enum.MouseBehavior.Default then
            pcall(function()
                uis.MouseBehavior = Enum.MouseBehavior.Default
            end)
        end
    end

    -- Games lock the camera the moment the cursor sits in the viewport, and a
    -- locked mouse makes every executor input test refuse to run. Only a lock
    -- that survives without any input at all is treated as stale: while the
    -- player is actually moving the mouse nothing is touched, so a real
    -- first-person camera keeps working.
    local function move(x, y)
        if not pcall(vim.SendMouseMoveEvent, vim, x, y, game) then
            pcall(vim.SendMouseMove, vim, x, y)
        end
        unlock()
    end

    local function button(index, down)
        local x, y = cursor()
        pcall(vim.SendMouseButtonEvent, vim, x, y, index, down, game, 1)
        unlock()
    end

    -- KeyCode values in this environment do not follow the Windows virtual-key
    -- numbers Roblox uses (E is 101 here, 69 there), so scripts that pass a raw
    -- VK code - and every executor does - matched nothing. Try the value table
    -- first (what Enum.KeyCode.X.Value gives here) and fall back to a real VK
    -- name map.
    local vkNames = {
        [0x08] = "Backspace", [0x09] = "Tab", [0x0D] = "Return", [0x10] = "LeftShift",
        [0x11] = "LeftControl", [0x12] = "LeftAlt", [0x13] = "Pause", [0x14] = "CapsLock",
        [0x1B] = "Escape", [0x20] = "Space", [0x21] = "PageUp", [0x22] = "PageDown",
        [0x23] = "End", [0x24] = "Home", [0x25] = "Left", [0x26] = "Up",
        [0x27] = "Right", [0x28] = "Down", [0x2C] = "Print", [0x2D] = "Insert",
        [0x2E] = "Delete", [0x5B] = "LeftSuper", [0x5C] = "RightSuper", [0x5D] = "Menu",
        [0x90] = "NumLock", [0x91] = "ScrollLock", [0xA0] = "LeftShift", [0xA1] = "RightShift",
        [0xA2] = "LeftControl", [0xA3] = "RightControl", [0xA4] = "LeftAlt", [0xA5] = "RightAlt",
    }
    for i = 0, 9 do
        vkNames[0x30 + i] = ({ "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" })[i + 1]
    end
    for i = 0, 25 do
        vkNames[0x41 + i] = string.char(65 + i)
    end
    for i = 0, 23 do
        vkNames[0x70 + i] = "F" .. (i + 1)   -- F1..F24 (F13 = 0x7C, used by tests)
    end

    local function keyEnum(code)
        if typeof(code) == "EnumItem" then
            return code
        end
        if typeof(code) == "string" then
            local ok, item = pcall(function()
                return Enum.KeyCode[code]
            end)
            if ok and typeof(item) == "EnumItem" then
                return item
            end
            return nil
        end

        -- The Windows virtual-key map has to win over the value scan: this
        -- environment numbers KeyCode items differently (Pipe happens to be
        -- 0x7C = VK_F13), so scanning values first turned every F13 keystroke
        -- into a Pipe press and the input tests rejected it as foreign input.
        local name = vkNames[code]
        if name then
            local ok, item = pcall(function()
                return Enum.KeyCode[name]
            end)
            if ok and typeof(item) == "EnumItem" then
                return item
            end
        end

        for _, item in ipairs(Enum.KeyCode:GetEnumItems()) do
            if item.Value == code then
                return item
            end
        end
    end

    local function keyEvent(code, down)
        local enum = keyEnum(code)
        if not enum then
            return false
        end
        local ok = pcall(vim.SendKeyEvent, vim, down, enum, false, game)
        unlock()
        return ok
    end

    local releaseStale = function() end

    genv.mousemoveabs = function(x, y)
        releaseStale()
        move(math.floor(x + 0.5), math.floor(y + 0.5))
    end

    -- kept for diagnostics: the raw OS wheel path (SendInput + window message)
    genv.__mousescroll_os = orig.mousescroll

    -- Mouse buttons were still going through the raw OS path, which needs the
    -- game to own the foreground and produced no InputBegan at all when it did
    -- not. The VirtualInputManager route works unfocused and, with
    -- gameProcessedEvent cleared, also updates IsMouseButtonPressed.
    -- VirtualInputManager numbers the buttons from zero: 0 is the left button,
    -- 1 the right and 2 the middle. Passing the usual 1/2/3 made mouse1press fire
    -- a MouseButton2 event, which every detection harness flags as foreign input
    -- and then skips the test with "the mouse was being used".
    local function pressButton(index, down)
        local x, y = cursor()
        local ok = pcall(vim.SendMouseButtonEvent, vim, x, y, index - 1, down, game, 0)
        if not ok then
            ok = pcall(vim.SendMouseButtonEvent, vim, x, y, index - 1, down, game, 1)
        end
        unlock()
        return ok
    end

    genv.mouse1press = function() pressButton(1, true) end
    genv.mouse1release = function() pressButton(1, false) end
    genv.mouse1click = function()
        pressButton(1, true)
        pressButton(1, false)
    end
    genv.mouse2press = function() pressButton(2, true) end
    genv.mouse2release = function() pressButton(2, false) end
    genv.mouse2click = function()
        pressButton(2, true)
        pressButton(2, false)
    end
    genv.mouse3press = function() pressButton(3, true) end
    genv.mouse3release = function() pressButton(3, false) end
    genv.mouse3click = function()
        pressButton(3, true)
        pressButton(3, false)
    end

    -- Scripts commonly reach for services as bare globals (UserInputService,
    -- Players, RunService...). Without them the first `UserInputService.InputBegan`
    -- line dies with "attempt to index nil", which is exactly what hubs report.
    for _, serviceName in ipairs({
        "Players", "UserInputService", "RunService", "TweenService", "HttpService",
        "Lighting", "ReplicatedStorage", "ReplicatedFirst", "StarterGui", "StarterPack",
        "SoundService", "ContentProvider", "ContextActionService", "GuiService",
        "TextService", "TeleportService", "CollectionService", "PathfindingService",
        "Teams", "Chat", "MarketplaceService", "LocalizationService", "VirtualUser",
        "HapticService", "VRService", "Stats", "GroupService", "BadgeService",
    }) do
        if genv[serviceName] == nil then
            local ok, service = pcall(function()
                return game:GetService(serviceName)
            end)
            if ok and service ~= nil then
                genv[serviceName] = service
            end
        end
    end

    genv.mousemoverel = function(dx, dy)
        local x, y = cursor()
        move(x + math.floor(dx + 0.5), y + math.floor(dy + 0.5))
    end

    genv.mousescroll = function(delta)
        -- A wheel event only reaches UserInputService while the window owns the
        -- foreground, and VirtualInputManager clamps every notch to +1 so a
        -- scroll down zoomed in exactly like a scroll up. Use the real Win32
        -- wheel directly when the client is already in front, otherwise borrow
        -- the foreground for a single frame through the native cycle helper and
        -- give it straight back.
        if __client_focused and __client_focused() then
            return orig.mousescroll(delta)
        end
        if __wheel_os_cycle and __wheel_os_cycle(delta) then
            return
        end
        -- Last resort: no foreground could be taken, so send the VirtualInputManager
        -- event (magnitude is lost, but the direction at least reaches the game).
        local x, y = cursor()
        if not pcall(vim.SendMouseWheelEvent, vim, x, y, delta, game) then
            return orig.mousescroll(delta)
        end
        unlock()
    end

    genv.keypress = function(code)
        if not keyEvent(code, true) then
            return orig.keypress(code)
        end
    end

    genv.keyrelease = function(code)
        if not keyEvent(code, false) then
            return orig.keyrelease(code)
        end
    end

    genv.keyclick = function(code)
        if not keyEvent(code, true) then
            return orig.keyclick(code)
        end
        task.wait()
        keyEvent(code, false)
    end
    -- Idle watchdog. Only active when a test harness asks for it, because
    -- forcing MouseBehavior or the zoom ceiling during normal play would fight
    -- the game's own camera.
    local lastInput = os.clock()
    for _, signal in ipairs({ uis.InputBegan, uis.InputChanged, uis.InputEnded }) do
        pcall(function()
            signal:Connect(function()
                lastInput = os.clock()
            end)
        end)
    end

    task.spawn(function()
        while true do
            task.wait(0.1)

            if not genv.__rxs_unlock_mouse then
                continue
            end

            local idle = os.clock() - lastInput
            if idle <= 0.4 then
                continue
            end

            -- The wheel notch VirtualInputManager emulates only zooms in, so a
            -- couple of scroll tests used to park the camera in first person,
            -- where the camera module locks the mouse and every following input
            -- test is skipped. Leave room to scroll by raising the ceiling only.
            pcall(function()
                local player = game:GetService("Players").LocalPlayer
                if player and player.CameraMaxZoomDistance < 1024 then
                    player.CameraMaxZoomDistance = 1024
                end
            end)

            if uis.MouseBehavior ~= Enum.MouseBehavior.Default then
                pcall(function()
                    uis.MouseBehavior = Enum.MouseBehavior.Default
                end)
            end
        end
    end)
end)
)LUA";

    RegisterSnakeCaseAliases(L);
    task_scheduler::RequestExecution(inputScript);
    task_scheduler::RequestExecution(signal);
    task_scheduler::RequestExecution(websocketScript);
    task_scheduler::RequestExecution(bannerScript);
    task_scheduler::RequestExecution(hiddenFramesScript);
    ApplyLuauFlags();
}
