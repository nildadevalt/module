#include "Metatable.hpp"

#include <Windows.h>
#include <lstate.h>
#include <lobject.h>
#include <lfunc.h>
#include <lstring.h>
#include <ltable.h>
#include <lapi.h>
#include <ldo.h>
#include <lgc.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <lualib.h>

#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>

#include <RBX/environment/libs/closures/closures.hpp>
#include <RBX/environment/env.hpp>


namespace Metatable {

    int getrawmetatable(lua_State* LS) {
        
        if (LS->userdata == nullptr)
            return 0;

        luaL_checkany(LS, 1);

        if (!lua_getmetatable(LS, 1))
            lua_pushnil(LS);

        return 1;
    }

    int setreadonly(lua_State* L) {
        
        luaL_checktype(L, 1, LUA_TTABLE);
        luaL_checktype(L, 2, LUA_TBOOLEAN);
        hvalue(luaA_toobject(L, 1))->readonly = lua_toboolean(L, 2);
        return 0;
    }

    int isreadonly(lua_State* L) {
        
        luaL_checktype(L, 1, LUA_TTABLE);
        lua_pushboolean(L, hvalue(luaA_toobject(L, 1))->readonly);
        return 1;
    }

    int setrawmetatable(lua_State* L)
    {
        
        luaL_checkany(L, 1);
        luaL_checktype(L, 2, LUA_TTABLE);

        if (lua_isuserdata(L, 1) && !lua_getreadonly(L, 2) && lua_getmetatable(L, 1))
        {
            static const char* const preserved[] = { "__namecall", "__newindex" };

            for (const char* field : preserved)
            {
                lua_rawgetfield(L, 2, field);
                const bool defined = !lua_isnil(L, -1);
                lua_pop(L, 1);

                if (defined)
                    continue;

                lua_rawgetfield(L, -1, field);

                if (lua_isnil(L, -1))
                {
                    lua_pop(L, 1);
                    continue;
                }

                lua_rawsetfield(L, 2, field);
            }

            lua_pop(L, 1);
        }

        lua_settop(L, 2);
        lua_setmetatable(L, 1);
        lua_pushvalue(L, 1);
        return 1;
    }

    int makereadonly(lua_State* L) {
        
        luaL_checktype(L, 1, LUA_TTABLE);

        LuaTable* table = hvalue(luaA_toobject(L, 1));

        table->readonly = true;

        return 0;
    }


    int makewriteable(lua_State* L) {
        
        luaL_checktype(L, 1, LUA_TTABLE);

        LuaTable* table = hvalue(luaA_toobject(L, 1));

        table->readonly = false;

        return 0;
    }

    int getnamecallmethod(lua_State* L)
    {
        Closures::RestoreNamecallAtom(L);
        
        const char* namecall = lua_namecallatom(L, nullptr);

        if (!namecall)
        {
            lua_pushnil(L);
            return 1;
        }

        lua_pushstring(L, namecall);
        return 1;
    }



    // resolves the metatable of the value at `index` without leaving
    // anything behind on the stack. returns nullptr when there is none.
    static LuaTable* metatable_of(lua_State* L, int index)
    {
        if (!lua_getmetatable(L, index))
            return nullptr;

        if (lua_type(L, -1) != LUA_TTABLE)
        {
            lua_pop(L, 1);
            return nullptr;
        }

        LuaTable* mt = hvalue(luaA_toobject(L, -1));
        lua_pop(L, 1);
        return mt;
    }

    // Swapping the __namecall slot directly avoids patching the shared native
    // closure in-place so sUNC's hookfunction/namecall detection and restores
    // see an untouched underlying closure while preserving L->namecall across
    // nested or yielding hooks.
    static int NamecallWrapperCall(lua_State* L)
    {
        Closure* current = curr_func(L);
        Closures::BeginNamecallFrame(L, current);
        lua_pushvalue(L, lua_upvalueindex(1));
        lua_insert(L, 1);
        luaD_callint(L, L->base, LUA_MULTRET, lua_isyieldable(L) != 0);
        const int result = Closures::finish_yieldable_call(L);
        if (result != C_CALL_YIELD)
            Closures::EndNamecallFrame(L);
        return result;
    }

    inline int hookmetamethod(lua_State* L)
    {
        luaL_checkany(L, 1);
        luaL_checktype(L, 2, LUA_TSTRING);
        luaL_checktype(L, 3, LUA_TFUNCTION);

        const char* metamethod = lua_tostring(L, 2);

        if (!lua_getmetatable(L, 1)) {
            luaL_argerror(L, 1, ("object has no metatable"));
        }
        int mt_idx = lua_gettop(L);

        lua_rawgetfield(L, mt_idx, metamethod);
        if (!lua_isfunction(L, -1))
            luaL_argerror(L, 2, "metamethod is not a function");
        int old_method = lua_gettop(L);
        Closure* target = clvalue(luaA_toobject(L, old_method));

        if (strcmp(metamethod, "__namecall") == 0)
        {
            LuaTable* mt = hvalue(luaA_toobject(L, mt_idx));
            const bool wasReadOnly = mt->readonly;
            mt->readonly = false;

            Closure* hookClosure = clvalue(luaA_toobject(L, 3));
            const bool isRestore = (target == hookClosure) ||
                (hookClosure->isC && hookClosure->c.f == NamecallWrapperCall);

            if (isRestore)
            {
                lua_pushvalue(L, 3);
                lua_setfield(L, mt_idx, "__namecall");
            }
            else
            {
                lua_pushvalue(L, 3);
                lua_pushcclosurek(L, NamecallWrapperCall, nullptr, 1, Closures::DispatchContinuation);
                lua_setfield(L, mt_idx, "__namecall");
            }

            mt->readonly = wasReadOnly;
            lua_pushvalue(L, old_method);
            return 1;
        }

        // Preserve the metatable slot and all existing references to the
        // closure. hookfunction returns an anchored copy of its old body.
        Closures::ensure_debug_info_hook(L);
        const int results = Closures::hookfunction(L, old_method, 3);
        auto record = Closures::HookRecords.find(target);
        if (record != Closures::HookRecords.end())
            record->second.PreserveFrameIdentity = true;
        return results;
    }

    int iswritable(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TTABLE);
        lua_pushboolean(L, !hvalue(luaA_toobject(L, 1))->readonly);
        return 1;
    }

    int setnamecallmethod(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TSTRING);
        size_t len = 0;
        const char* str = lua_tolstring(L, 1, &len);
        Closures::UpdateNamecallAtom(L, luaS_newlstr(L, str, len));
        return 0;
    }

    int getmetafield(lua_State* L)
    {
        luaL_checkany(L, 1);
        luaL_checkstring(L, 2);
        if (!lua_getmetatable(L, 1))
        {
            lua_pushnil(L);
            return 1;
        }
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        return 1;
    }

    int hasmetafield(lua_State* L)
    {
        luaL_checkany(L, 1);
        luaL_checkstring(L, 2);
        bool present = false;
        if (lua_getmetatable(L, 1))
        {
            lua_pushvalue(L, 2);
            lua_rawget(L, -2);
            present = !lua_isnil(L, -1);
        }
        lua_pushboolean(L, present);
        return 1;
    }

    int hasmetatable(lua_State* L)
    {
        luaL_checkany(L, 1);
        lua_pushboolean(L, lua_getmetatable(L, 1) != 0);
        return 1;
    }

    void register_lib(lua_State* L)
    {
        Environment->make_global(L, ("hookmetamethod"), hookmetamethod, Metatable::funcnameinc++);
        Environment->make_global(L, ("getnamecallmethod"), getnamecallmethod, Metatable::funcnameinc++);
        Environment->make_global(L, ("setnamecallmethod"), setnamecallmethod, Metatable::funcnameinc++);
        Environment->make_global(L, ("setreadonly"), setreadonly, Metatable::funcnameinc++);
        Environment->make_global(L, ("isreadonly"), isreadonly, Metatable::funcnameinc++);
        Environment->make_global(L, ("iswritable"), iswritable, Metatable::funcnameinc++);
        Environment->make_global(L, ("iswriteable"), iswritable, Metatable::funcnameinc++);
        Environment->make_global(L, ("getrawmetatable"), getrawmetatable, Metatable::funcnameinc++);
        Environment->make_global(L, ("setrawmetatable"), setrawmetatable, Metatable::funcnameinc++);
        Environment->make_global(L, ("makereadonly"), makereadonly, Metatable::funcnameinc++);
        Environment->make_global(L, ("makewriteable"), makewriteable, Metatable::funcnameinc++);
        Environment->make_global(L, ("makewritable"), makewriteable, Metatable::funcnameinc++);
        Environment->make_global(L, ("getmetafield"), getmetafield, Metatable::funcnameinc++);
        Environment->make_global(L, ("hasmetafield"), hasmetafield, Metatable::funcnameinc++);
        Environment->make_global(L, ("hasmetatable"), hasmetatable, Metatable::funcnameinc++);
    }
}
