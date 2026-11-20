#pragma once

// Small compatibility layer for the module's executor code.
//
// The 2c6fd5e0 Luau sources do not ship the Roblox integer-64 extension nor the
// frame-hidden filter hook, but the module's library surface is built on both.
// They are re-provided here: int64 values are carried as doubles (exact up to
// 2^53, which is what Luau numbers are anyway) and the frame filter is a stub.

#include <cstdint>
#include <cstdlib>

typedef int (*lua_FrameHiddenFn)(lua_State* L, int level);

inline void lua_setframehiddenfilter(lua_FrameHiddenFn fn) { (void)fn; }
inline int lua_frameishidden(lua_State* L, int level) { (void)L; (void)level; return 0; }

#ifndef LUA_INTLIBNAME
#define LUA_INTLIBNAME "integer"
#endif

#define lua_normalisestack(L, mxs) do { if (lua_gettop(L) > (mxs)) lua_settop(L, (mxs)); } while (0)

inline void lua_pushinteger64(lua_State* L, std::int64_t n) { lua_pushnumber(L, static_cast<double>(n)); }
inline std::int64_t lua_tointeger64(lua_State* L, int idx, int* isnum = nullptr)
{
    int ok = 0;
    double value = lua_tonumberx(L, idx, &ok);
    if (isnum)
        *isnum = ok;
    return static_cast<std::int64_t>(value);
}
inline std::int64_t luaL_checkinteger64(lua_State* L, int idx)
{
    int ok = 0;
    const double value = lua_tonumberx(L, idx, &ok);
    (void)ok;
    return static_cast<std::int64_t>(value);
}
inline std::int64_t luaL_optinteger64(lua_State* L, int idx, std::int64_t def)
{
    return lua_isnoneornil(L, idx) ? def : luaL_checkinteger64(L, idx);
}
