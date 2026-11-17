import io

def patch(path, pairs):
    s = io.open(path, encoding='utf-8', errors='surrogateescape').read()
    for old, new in pairs:
        assert old in s, (path, old[:60])
        s = s.replace(old, new, 1)
    io.open(path, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(s)
    print('patched', path)

# A. raknet.send accepts a buffer as well as a string
patch('Module/RBX/environment/libs/raknet/raknet.cpp', [(
"""    int raknet_send_raw(lua_State* L)
    {
        size_t dataLen = 0;
        const char* data = luaL_checklstring(L, 1, &dataLen);
""",
"""    int raknet_send_raw(lua_State* L)
    {
        size_t dataLen = 0;
        const char* data = nullptr;
        std::string bufferCopy;

        // debUNC sends both a string and a buffer made with buffer.fromstring,
        // so convert a buffer through buffer.tostring before asking for a string.
        if (lua_type(L, 1) != LUA_TSTRING)
        {
            lua_getglobal(L, ("buffer"));
            if (lua_istable(L, -1))
            {
                lua_getfield(L, -1, ("tostring"));
                if (lua_isfunction(L, -1))
                {
                    lua_pushvalue(L, 1);
                    if (lua_pcall(L, 1, 1, 0) == LUA_OK && lua_isstring(L, -1))
                    {
                        bufferCopy = lua_tostring(L, -1);
                        data = bufferCopy.data();
                        dataLen = bufferCopy.size();
                    }
                    lua_pop(L, 1);
                }
                else
                {
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);
            }
            else
            {
                lua_pop(L, 1);
            }
        }

        if (!data)
            data = luaL_checklstring(L, 1, &dataLen);
""")])

# B. gettenv defaults to the calling thread's environment
patch('Module/RBX/environment/libs/closures/closures.hpp', [(
"""        if (!lua_isthread(L, 1)) {
            lua_pushnil(L);
            return 1;
        }

        lua_State* thread = lua_tothread(L, 1);
        if (!thread) {
            lua_pushnil(L);
            return 1;
        }

        lua_pushvalue(thread, LUA_GLOBALSINDEX);""",
"""        // With no argument the calling thread's environment is returned, which is
        // what getfenv(0) gives. A finished thread still reports its environment.
        lua_State* thread = L;
        if (lua_isthread(L, 1))
        {
            thread = lua_tothread(L, 1);
        }
        else if (!lua_isnoneornil(L, 1))
        {
            lua_pushnil(L);
            return 1;
        }

        if (!thread) {
            lua_pushnil(L);
            return 1;
        }

        if (thread == L) {
            lua_pushvalue(L, LUA_GLOBALSINDEX);
            return 1;
        }

        lua_pushvalue(thread, LUA_GLOBALSINDEX);""")])

# C. a failed HTTP GET raises instead of returning a fake error string
patch('Module/RBX/environment/libs/http/http.cpp', [(
"""                    if (!result.success)
                    {
                        lua_pushstring(L, (("http_get failed: ") + result.error).c_str());
                        return 1;
                    }

                    if (result.status_code != 200)
                    {
                        std::stringstream err;
                        err << ("http_get returned status: ") << result.status_code;
                        lua_pushstring(L, err.str().c_str());
                        return 1;
                    }
""",
"""                    // A failed request must not look like a successful body: Roblox
                    // raises an HttpError, so raise that instead of a message string.
                    if (!result.success)
                        luaL_error(L, ("HttpError: %s"), result.error.empty() ? ("request failed") : result.error.c_str());

                    if (result.status_code != 200)
                        luaL_error(L, ("HttpError: %d"), result.status_code);
""")])

# D. block the purchase/profile methods debUNC can still call
patch('Module/RBX/environment/hooks/hooks.cpp', [(
"""    "PromptNativePurchaseWithLocalPlayer", "PromptCollectiblesPurchase",""",
"""    "PromptNativePurchaseWithLocalPlayer", "PromptNativePurchaseWithLocalPlayerWithPaymentSessionId",
    "PromptNativePurchaseWithPaymentSessionId", "PromptCollectiblesPurchase","""), (
"""    "GetDeviceIntegrityTokenYield", "NoPromptCreateOutfit",""",
"""    "GetDeviceIntegrityTokenYield", "NoPromptApplyProfileConfiguration", "NoPromptCreateOutfit",""")])

# E. never write an executable/program file from the file API
patch('Module/RBX/environment/libs/filesystem/filesystem.cpp', [(
"""    int writefile(lua_State* L)
    {""",
"""    // debUNC checks that scripts cannot drop a runnable file such as .exe or .psm1
    // on disk, so refuse program extensions at both write entry points.
    static bool IsProgramExtension(const wchar_t* path)
    {
        static const wchar_t* kExts[] = {
            L".exe", L".com", L".bat", L".cmd", L".ps1", L".psm1", L".psd1",
            L".vbs", L".vbe", L".js", L".jse", L".wsf", L".wsh", L".msi",
            L".scr", L".lnk", L".hta", L".cpl", L".dll", L".sys", L".reg",
            L".jar", L".msc", L".pif", L".gadget", L".application", L".appx",
            L".msix", L".inf", L".scf", L".shb", L".pyw", L".vb",
            nullptr
        };

        std::wstring str(path ? path : L"");
        for (size_t i = 0; i < str.size(); ++i)
            str[i] = (wchar_t)towlower(str[i]);

        for (int i = 0; kExts[i]; ++i)
        {
            std::wstring ext(kExts[i]);
            if (str.size() > ext.size() && str.compare(str.size() - ext.size(), ext.size(), ext) == 0)
                return true;
        }
        return false;
    }

    int writefile(lua_State* L)
    {"""), (
"""        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, false)) return Err(L, ("Failed to write file"));""",
"""        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, false)) return Err(L, ("Failed to write file"));"""), (
"""        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, true)) return Err(L, ("Failed to append file"));""",
"""        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, true)) return Err(L, ("Failed to append file"));""")])

# F. debug.isvalidlevel uses the same level base as debug.info, getcallstack reports currentline
patch('Module/RBX/environment/libs/debug/debug.cpp', [(
"""        // SehGuard wrapper adds 1 stack frame \u00e2\u20ac\u201d adjust
        level += 1;

        lua_Debug ar;
        bool valid = lua_getinfo(L, level, ("f"), &ar) != 0;
        if (valid && lua_isfunction(L, -1))
        {
            lua_pushboolean(L, 1);
        }
        else
        {
            lua_pushboolean(L, 0);
        }
        return 1;""",
"""        // debug.info reads levels without an offset, so isvalidlevel must too,
        // otherwise the last real level reported by info is refused here.
        lua_Debug ar;
        bool valid = lua_getinfo(L, level, ("f"), &ar) != 0 && lua_isfunction(L, -1);
        if (valid)
            lua_pop(L, 1); // lua_getinfo pushed the function

        lua_pushboolean(L, valid ? 1 : 0);
        return 1;"""), (
"""                lua_pushinteger(L, ar.currentline > 0 ? ar.currentline : ar.linedefined);
                lua_setfield(L, -2, "line");""",
"""                lua_pushinteger(L, ar.currentline > 0 ? ar.currentline : ar.linedefined);
                lua_setfield(L, -2, "line");

                lua_pushinteger(L, ar.currentline);
                lua_setfield(L, -2, "currentline");""")])
print('ALL PATCHES OK')
