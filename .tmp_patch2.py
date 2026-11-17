import io

def patch(path, pairs):
    s = io.open(path, encoding='utf-8', errors='surrogateescape').read()
    for old, new in pairs:
        assert old in s, (path, old[:70])
        s = s.replace(old, new, 1)
    io.open(path, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(s)
    print('patched', path)

# D. block the purchase/profile methods debUNC can still call
patch('Module/RBX/environment/hooks/hooks.cpp', [(
'''    "PromptNativePurchaseWithLocalPlayer", "PromptCollectiblesPurchase",''',
'''    "PromptNativePurchaseWithLocalPlayer", "PromptNativePurchaseWithLocalPlayerWithPaymentSessionId",
    "PromptNativePurchaseWithPaymentSessionId", "PromptCollectiblesPurchase",'''), (
'''"GetDeviceIntegrityTokenYield", "NoPromptCreateOutfit",''',
'''"GetDeviceIntegrityTokenYield", "NoPromptApplyProfileConfiguration", "NoPromptCreateOutfit",''')])

# E. never write an executable/program file from the file API
patch('Module/RBX/environment/libs/filesystem/filesystem.cpp', [(
'''    int writefile(lua_State* L)
    {''',
'''    // debUNC checks that scripts cannot drop a runnable file such as .exe or .psm1
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
    {'''), (
'''        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, false)) return Err(L, ("Failed to write file"));''',
'''        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, false)) return Err(L, ("Failed to write file"));'''), (
'''        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, true)) return Err(L, ("Failed to append file"));''',
'''        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, true)) return Err(L, ("Failed to append file"));''')])

# F. debug.isvalidlevel uses the same level base as debug.info, getcallstack reports currentline
patch('Module/RBX/environment/libs/debug/debug.cpp', [(
'''        level += 1;

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
        return 1;''',
'''        // debug.info reads levels without an offset, so isvalidlevel must too,
        // otherwise the last real level reported by info is refused here.
        lua_Debug ar;
        bool valid = lua_getinfo(L, level, ("f"), &ar) != 0 && lua_isfunction(L, -1);
        if (valid)
            lua_pop(L, 1); // lua_getinfo pushed the function

        lua_pushboolean(L, valid ? 1 : 0);
        return 1;'''), (
'''                lua_pushinteger(L, ar.currentline > 0 ? ar.currentline : ar.linedefined);
                lua_setfield(L, -2, "line");''',
'''                lua_pushinteger(L, ar.currentline > 0 ? ar.currentline : ar.linedefined);
                lua_setfield(L, -2, "line");

                lua_pushinteger(L, ar.currentline);
                lua_setfield(L, -2, "currentline");''')])
print('ALL PATCHES OK')
