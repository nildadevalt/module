#include "Closures.hpp"

#include <RBX/environment/env.hpp>

namespace Closures
{
    std::unordered_set<Closure*> HiddenClosures;
    std::vector<HiddenLevel> HiddenLevels;



    void register_lib(lua_State* L)
    {
        funcnameinc = 0;
        // NOTE: no ResetLibraryState() here. register_lib also runs for actor
        // / secondary lua states and the records are process-wide — resetting
        // here used to wipe every live newcclosure/hookfunction record of the
        // main state mid-session ("newcclosure: missing wrapped function").
        // Fresh-state init resets explicitly in initalizeEnvironment instead.


        lua_pushcclosurek(L, Closures::HookLuaDispatcher, LuaHookDispatcherName, 0, DispatchContinuation);
        lua_setglobal(L, LuaHookDispatcherName);

        Environment->make_global(L, "loadstring", Closures::loadstring, funcnameinc++);
       Environment->make_global(L, "hookfunction", Closures::hookfunction, funcnameinc++);
       Environment->make_global(L, "hookfunc", Closures::hookfunction, funcnameinc++);
       Environment->make_global(L, "replaceclosure", Closures::hookfunction, funcnameinc++);
      Environment->make_global(L, "newcclosure", Closures::newcclosure, funcnameinc++);
        Environment->make_global(L, "newlclosure", Closures::newlclosure, funcnameinc++);
        Environment->make_global(L, "isnewcclosure", Closures::isnewcclosure, funcnameinc++);
      Environment->make_global(L, "restorefunction", Closures::restorefunction, funcnameinc++);
        Environment->make_global(L, "restorefunc", Closures::restorefunction, funcnameinc++);
        Environment->make_global(L, "isfunctionhooked", Closures::isfunctionhooked, funcnameinc++);
        Environment->make_global(L, "clonefunction", Closures::clonefunction, funcnameinc++);
       Environment->make_global(L, "iscclosure", Closures::iscclosure, funcnameinc++);
       Environment->make_global(L, "islclosure", Closures::islclosure, funcnameinc++);
       Environment->make_global(L, "isourclosure", Closures::is_executor_closure, funcnameinc++);
       Environment->make_global(L, "checkclosure", Closures::is_executor_closure, funcnameinc++);
       Environment->make_global(L, "isexecutorclosure", Closures::is_executor_closure, funcnameinc++);
       Environment->make_global(L, "isexecutorfunction", Closures::is_executor_closure, funcnameinc++);
     Environment->make_global(L, "gettenv", Closures::gettenv, funcnameinc++);
        Environment->make_global(L, "compareclosures", Closures::compareclosures, funcnameinc++);
        Environment->make_global(L, "comparefunctions", Closures::compareclosures, funcnameinc++);
        Environment->make_global(L, "comparefunction", Closures::compareclosures, funcnameinc++);
      Environment->make_global(L, "hooksignal", Closures::hooksignal, funcnameinc++);
        Environment->make_global(L, "restoresignal", Closures::restoresignal, funcnameinc++);
        Environment->make_global(L, "issignalhooked", Closures::issignalhooked, funcnameinc++);
        Environment->make_global(L, "is_signal_hooked", Closures::issignalhooked, funcnameinc++);
       Environment->make_global(L, "setstackhidden", Closures::setstackhidden, funcnameinc++);
        Environment->make_global(L, "sethiddenstack", Closures::setstackhidden, funcnameinc++);
        Environment->make_global(L, "set_stack_hidden", Closures::setstackhidden, funcnameinc++);
        Environment->make_global(L, "ishiddenstack", Closures::ishiddenstack, funcnameinc++);
        Environment->make_global(L, "isstackhidden", Closures::ishiddenstack, funcnameinc++);
        Environment->make_global(L, "is_stack_hidden", Closures::ishiddenstack, funcnameinc++);
        Environment->make_global(L, "setname", Closures::setname, funcnameinc++);

        // detection fix: Function Environment[3] Metamethods - DIRECT reads
        // getfenv of the frame at debug.info(2, "f") in the xpcall message
        // handler. Depending on where the error unwound to, that frame can
        // be the protected call itself (pcall/xpcall continuation) or the
        // error/assert builtin that raised — all of which share the script's
        // env table. Pin them to bare keyless tables: they never resolve
        // globals through their own env (C functions read LUA_GLOBALSINDEX,
        // the thread's gt), so behavior is unchanged while getfenv of them
        // can no longer echo the probe's marker back.
        // cleared per registration so pointer reuse from a torn-down state
        // can't shadow a fresh state's closures.
        HardenedErrorFrames.clear();
        // HardenErrorFrameLibs(L);

        // setstackhidden -> VM. Process-global pointer, same filter for
        // every state, so re-registering is idempotent. Everything that
        // builds a trace (lua_debugtrace, luaL_traceback) now drops frames
        // that were marked hidden.
        lua_setframehiddenfilter(&FrameHiddenFilter);

        EnsureInvokeSecurityHook(L);

        // Installed on-demand by hookmetamethod / setstackhidden so native debug.info
        // stays untouched for scripts that do not hook metamethods or hide frames.
        // ensure_debug_info_hook(L);
        // ensure_debug_traceback_hook(L);
        // ensure_fenv_globals(L);

    }
}
