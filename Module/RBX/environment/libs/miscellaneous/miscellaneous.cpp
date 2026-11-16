#include "Miscellaneous.hpp"

#include <lgc.h>
#include <lstate.h>
#include <lualib.h>

#include <Windows.h>
#include <shlobj.h>
#include <Utilitys/ModuleLocation.hpp>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <RBX/execution/execution.hpp>
#include <RBX/environment/libs/script/script.hpp>
#include <RBX/environment/libs/http/http.hpp>
#include <map>
#include <string>
#include <sstream>
#include <lualib.h>
#include <optional>
#include <algorithm>
#include <Windows.h>

#include <chrono>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cctype>
#include <mutex>

#include <cpr/cpr.h>
#include <Dependencies/nlohmann/json.hpp>
#include <RBX/environment/env.hpp>
#include <RBX/environment/yielding/yielding.hpp>

#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>
#include <RBX/execution/execution.hpp>
#include <RBX/environment/yielding/yielding.hpp>
#include <RBX/environment/env.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <RBX/fflags.hpp>


namespace Miscellaneous {

    // ══════════════════════════════════════════════════════════════
    //  Tovek local decompiler client
    // ══════════════════════════════════════════════════════════════

    namespace Tovek {

        static std::mutex LaunchMutex;
        static bool Launched = false;

        // Directory this module lives in — decompiler.exe is dropped beside it.
        // The path is captured while the loader still knows the module; once
        // the module is hidden from the loader lists the lookup below fails.
        static std::wstring ModuleDirectory()
        {
            const std::wstring directory = ModuleLocation::Resolve();

            if (!directory.empty())
                return directory;

            HMODULE module = nullptr;
            if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&ModuleDirectory), &module))
                return {};

            wchar_t path[MAX_PATH] = {};
            const DWORD length = GetModuleFileNameW(module, path, MAX_PATH);
            if (length == 0 || length >= MAX_PATH)
                return {};

            std::wstring full(path, length);
            const size_t slash = full.find_last_of(L"\\/");
            if (slash == std::wstring::npos)
                return {};
            return full.substr(0, slash);
        }

        std::wstring DecompilerPath()
        {
            static const std::wstring cached = []() -> std::wstring {
                const std::wstring directory = ModuleDirectory();
                if (!directory.empty())
                {
                    const std::wstring candidate = directory + L"\\" + kExeName;
                    if (GetFileAttributesW(candidate.c_str()) != INVALID_FILE_ATTRIBUTES)
                        return candidate;
                }

                wchar_t executable[MAX_PATH] = {};
                const DWORD length = GetModuleFileNameW(nullptr, executable, MAX_PATH);
                if (length > 0 && length < MAX_PATH)
                {
                    std::wstring full(executable, length);
                    const size_t slash = full.find_last_of(L"\\/");
                    if (slash != std::wstring::npos)
                    {
                        const std::wstring candidate = full.substr(0, slash) + L"\\" + kExeName;
                        if (GetFileAttributesW(candidate.c_str()) != INVALID_FILE_ATTRIBUTES)
                            return candidate;
                    }
                }
                return {};
            }();
            return cached;
        }

        bool LaunchServer(std::wstring& error)
        {
            const std::lock_guard<std::mutex> lock(LaunchMutex);

            const std::wstring path = DecompilerPath();
            if (path.empty())
            {
                error = L"decompiler.exe not found next to Module.dll";
                return false;
            }

            if (Launched)
                return true;

            std::wstring command = L"\"" + path + L"\"";

            STARTUPINFOW startup{};
            startup.cb = sizeof(startup);
            startup.dwFlags = STARTF_USESHOWWINDOW;
            startup.wShowWindow = SW_HIDE;

            PROCESS_INFORMATION process{};
            const BOOL ok = CreateProcessW(
                path.c_str(), command.data(), nullptr, nullptr, FALSE,
                CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP,
                nullptr, nullptr, &startup, &process);

            if (!ok)
            {
                error = L"CreateProcessW failed (" + std::to_wstring(GetLastError()) + L")";
                return false;
            }

            CloseHandle(process.hThread);
            CloseHandle(process.hProcess);
            Launched = true;
            return true;
        }

        static int RetryAfterSeconds(const cpr::Header& header, int fallback)
        {
            const auto it = header.find("Retry-After");
            if (it == header.end())
                return fallback;

            char* end = nullptr;
            const long value = std::strtol(it->second.c_str(), &end, 10);
            if (end == it->second.c_str() || value <= 0)
                return fallback;
            return static_cast<int>(value > 30 ? 30 : value);
        }

        // One round-trip: spawn/wait for the server if it is down, honour 503
        // backpressure, then report the server's answer.
        // status: HTTP status, 0 = never reached, -1 = decompiler.exe unusable.
        static std::string Send(const std::string& payload, const std::string& scriptName,
            unsigned encodeKey, int& status)
        {
            status = 0;

            cpr::Header header{
                { "Content-Type", "application/octet-stream" },
                { "X-Encode-Key", std::to_string(encodeKey) },
            };
            if (!scriptName.empty())
                header["X-Script-Name"] = scriptName;

            const std::wstring path = DecompilerPath();
            if (path.empty())
            {
                status = -1;
                return "decompiler.exe not found next to Module.dll";
            }

            const auto deadline = std::chrono::steady_clock::now()
                + std::chrono::seconds(kDeadlineSeconds);

            while (true)
            {
                cpr::Response response = cpr::Post(
                    cpr::Url{ kRawUrl },
                    header,
                    cpr::Body{ payload },
                    cpr::Timeout{ kRequestTimeoutMs });

                // 0 = libcurl never got a response: the server isn't up yet.
                if (response.status_code == 0)
                {
                    std::wstring error;
                    if (!LaunchServer(error))
                    {
                        status = -1;
                        return "-- decompiler.exe failed to start";
                    }
                    if (std::chrono::steady_clock::now() >= deadline)
                    {
                        status = 0;
                        return "-- local decompiler (127.0.0.1:3000) did not answer in time";
                    }
                    Sleep(250);
                    continue;
                }

                // Queue full / slot wait — the server tells us how long to back off.
                if (response.status_code == 503)
                {
                    if (std::chrono::steady_clock::now() >= deadline)
                    {
                        status = 503;
                        return response.text;
                    }
                    Sleep(static_cast<DWORD>(RetryAfterSeconds(response.header, 1)) * 1000);
                    continue;
                }

                status = response.status_code;
                return response.text;
            }
        }

        std::string Request(const std::string& bytecode, const std::string& scriptName, int& status)
        {
            status = 0;

            if (bytecode.empty())
            {
                status = -1;
                return "empty bytecode";
            }

            std::string result;
            int finalStatus = 0;
            bool done = false;
            // First "this isn't parseable Luau bytecode" answer. Kept so a
            // completely hopeless upload still reports why it was rejected.
            std::string shapeError;
            int shapeErrorStatus = 0;

            auto tryVariant = [&](const std::string& payload, unsigned encodeKey)
            {
                if (done)
                    return;

                int attemptStatus = 0;
                std::string body = Send(payload, scriptName, encodeKey, attemptStatus);

                if (attemptStatus == 200)
                {
                    done = true;
                    finalStatus = 200;
                    result = std::move(body);
                    return;
                }

                // Wrong header/key shape: the bytes themselves are still worth
                // re-sending as other plausible readings of the same script.
                if (attemptStatus == 400 && body.find("deserialize") != std::string::npos)
                {
                    if (shapeError.empty())
                    {
                        shapeError = std::move(body);
                        shapeErrorStatus = attemptStatus;
                    }
                    return;
                }

                // Anything else (transport failure, 500, unsupported version
                // message, ...) is the real answer — stop guessing.
                done = true;
                finalStatus = attemptStatus;
                result = std::move(body);
            };

            // 1. as stored by Roblox (opcode-encoded, key 203), then plain.
            tryVariant(bytecode, kEncodeKey);
            tryVariant(bytecode, 1);

            // 2. Byte 0 of a fetched script gets rewritten to the executor's
            //    compiler version before it ever reaches us, which desyncs a
            //    v10-v14 chunk. Restoring it is a one-byte fix, so sweep every
            //    version the server understands until one parses.
            if (!done && !shapeError.empty())
            {
                const unsigned char original = static_cast<unsigned char>(bytecode[0]);

                for (const unsigned encodeKey : { kEncodeKey, 1u })
                {
                    for (unsigned char version = kMinVersion; version <= kMaxVersion; ++version)
                    {
                        if (version == original)
                            continue;

                        std::string fixed = bytecode;
                        fixed[0] = static_cast<char>(version);
                        tryVariant(fixed, encodeKey);

                        if (done)
                            break;
                    }
                    if (done)
                        break;
                }
            }

            if (!done)
            {
                finalStatus = shapeErrorStatus;
                result = shapeError;
            }

            status = finalStatus;
            return result;
        }
    }

    // ══════════════════════════════════════════════════════════════
    //  Existing functions
    // ══════════════════════════════════════════════════════════════

    int SetIdentity(lua_State* L) {
        lua_normalisestack(L, 1);

        if (lua_type(L, 1) != LUA_TNUMBER)
            luaL_typeerrorL(L, 1, lua_typename(L, LUA_TNUMBER));

        int identity = (int)lua_tointeger(L, 1);
        task_scheduler::SetThreadCapabilities(L, identity, identity == 8 ? MaxCapabilities : identity);
        return 0;
    }

    int setthreadidentity(lua_State* L)
    {
        
        luaL_checktype(L, 1, LUA_TNUMBER);
        int identity = (int)lua_tointeger(L, 1);

        if (!L->userdata)
            return 0;

        uint64_t caps = 0xFFFFFFFFFFFFFF00 | Roblox::GetCapabilities(&identity);

        if (identity == 8)
        {
            caps = MaxCapabilities;
        }

        L->userdata->identity = identity;
        L->userdata->capabilities = caps;

        const uintptr_t identity_ptr = Roblox::GetIdentityStruct(*reinterpret_cast<uintptr_t*>(Offsets::RobloxThread::IdentityPtr));

        if (identity_ptr) {
            *reinterpret_cast<uint64_t*>(identity_ptr) = identity;
            *reinterpret_cast<uint64_t*>(identity_ptr + Offsets::Capabilities::Capabilities) = caps;
        }

        return 0;
    }

    int getthreadidentity(lua_State* L)
    {
        
        if (!L->userdata)
        {
            lua_pushinteger(L, 0);
            return 1;
        }
        lua_pushinteger(L, L->userdata->identity);
        return 1;
    }

    int getgenv(lua_State* L)
    {
        lua_State* exploitThread = Shared::ExploitThread;
        if (!exploitThread || exploitThread == L || exploitThread->global != L->global)
        {
            lua_pushvalue(L, LUA_GLOBALSINDEX);
            return 1;
        }

        lua_rawcheckstack(L, 1);
        luaC_threadbarrier(L);
        luaC_threadbarrier(exploitThread);
        lua_pushvalue(exploitThread, LUA_GLOBALSINDEX);
        lua_xmove(exploitThread, L, 1);

        return 1;
    }

    int identifyexecutor(lua_State* L)
    {
        lua_pushstring(L, ("Paradox"));
        lua_pushstring(L, ("1.0"));

        return 2;
    }

    int getexecutorname(lua_State* L)
    {
        lua_pushstring(L, ("Paradox"));

        return 1;
    }

    int GetObjects(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TUSERDATA);
        luaL_checktype(L, 2, LUA_TSTRING);

        lua_getglobal(L, ("game"));
        lua_getfield(L, -1, ("GetService"));
        if (!lua_isfunction(L, -1))
        {
            lua_pop(L, 2);
            luaL_error(L, ("game:GetService is unavailable"));
            return 0;
        }
        lua_pushvalue(L, -2);
        lua_pushstring(L, ("InsertService"));
        if (lua_pcall(L, 2, 1, 0) != LUA_OK)
        {
            const char* err = lua_tostring(L, -1);
            char message[512];
            strncpy_s(message, err ? err : ("failed to get InsertService"), _TRUNCATE);
            luaL_error(L, ("%s"), message);
            return 0;
        }
        lua_remove(L, -2);

        lua_getfield(L, -1, ("LoadLocalAsset"));
        if (!lua_isfunction(L, -1))
        {
            lua_pop(L, 2);
            luaL_error(L, ("InsertService:LoadLocalAsset is unavailable"));
            return 0;
        }

        lua_pushvalue(L, -2);
        lua_pushvalue(L, 2);
        if (lua_pcall(L, 2, 1, NULL) != LUA_OK)
        {
            const char* err = lua_tostring(L, -1);
            char message[512];
            strncpy_s(message, err ? err : ("LoadLocalAsset failed"), _TRUNCATE);
            luaL_error(L, ("%s"), message);
            return 0;
        }

        if (lua_type(L, -1) == LUA_TSTRING)
            luaL_error(L, lua_tostring(L, -1));

        lua_createtable(L, 1, NULL);
        lua_pushvalue(L, -2);
        lua_rawseti(L, -2, 1);

        lua_remove(L, -3);
        lua_remove(L, -2);

        return 1;
    }

    int setfpscap(lua_State* L) {
        
        luaL_checktype(L, 1, LUA_TNUMBER);
        double fps = lua_tonumber(L, 1);

        luaL_argcheck(L, std::isfinite(fps) && fps >= 0 && fps <= INT32_MAX, 1, "FPS cap is outside the supported range");
        // This build's native getter reads an int32 flag; zero selects its default.
        __try {
            *reinterpret_cast<int32_t*>(Offsets::Flags::TaskSchedulerTargetFps) = static_cast<int32_t>(fps);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            luaL_error(L, "FPS cap storage is unavailable");
        }
        return 0;
    }

    int getfpscap(lua_State* L)
    {
        
        __try {
            const int32_t cap = *reinterpret_cast<int32_t*>(Offsets::Flags::TaskSchedulerTargetFps);
            lua_pushinteger(L, cap > 0 ? cap : 60);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            lua_pushnumber(L, 60);
        }
        return 1;
    }

    // ══════════════════════════════════════════════════════════════
    //  setfflag / getfflag — Fast Flag registry access
    //
    //  The client keeps every flag in the FastFlag list rooted at
    //  FFlagList::Pointer; each node carries the flag name at +ToFlag and
    //  the flag value at +ToValue. Names are matched case-sensitively and
    //  values are read/written in their string form.
    // ══════════════════════════════════════════════════════════════
    namespace FFlagAccess
    {
        inline bool SafeRead(uintptr_t addr, void* out, size_t size)
        {
            if (!addr || !out || !size)
                return false;

            MEMORY_BASIC_INFORMATION mbi{};
            if (VirtualQuery(reinterpret_cast<void*>(addr), &mbi, sizeof(mbi)) == 0)
                return false;
            if (mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
                return false;

            __try
            {
                memcpy(out, reinterpret_cast<void*>(addr), size);
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
        }

        inline bool SafeWrite(uintptr_t addr, const void* in, size_t size)
        {
            if (!addr || !in || !size)
                return false;

            MEMORY_BASIC_INFORMATION mbi{};
            if (VirtualQuery(reinterpret_cast<void*>(addr), &mbi, sizeof(mbi)) == 0)
                return false;
            if (mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
                return false;
            if (!(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
                return false;

            __try
            {
                memcpy(reinterpret_cast<void*>(addr), in, size);
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
        }

        // MSVC std::string layout (x64): buf/ptr +16, size +24 capacity.
        inline bool ReadStdString(uintptr_t addr, std::string& out)
        {
            size_t size = 0, capacity = 0;
            if (!SafeRead(addr + 16, &size, sizeof(size)))
                return false;
            if (!SafeRead(addr + 24, &capacity, sizeof(capacity)))
                return false;
            if (size > 0x10000 || capacity > 0x10000 || size > capacity)
                return false;

            if (capacity < 16) // SSO: bytes live inline
            {
                char buffer[17] = {};
                if (!SafeRead(addr, buffer, size ? size : 1))
                    return false;
                buffer[size] = '\0';
                out.assign(buffer, size);
                return true;
            }

            uintptr_t ptr = 0;
            if (!SafeRead(addr, &ptr, sizeof(ptr)) || !ptr)
                return false;
            out.resize(size);
            if (size && !SafeRead(ptr, out.data(), size))
                return false;
            return true;
        }

        inline bool WriteStdString(uintptr_t addr, const std::string& value)
        {
            size_t size = 0, capacity = 0;
            if (!SafeRead(addr + 16, &size, sizeof(size)))
                return false;
            if (!SafeRead(addr + 24, &capacity, sizeof(capacity)))
                return false;
            if (value.size() > capacity || capacity > 0x10000)
                return false; // can't grow the buffer safely

            if (capacity < 16) // SSO
            {
                char buffer[16] = {};
                memcpy(buffer, value.data(), value.size());
                if (!SafeWrite(addr, buffer, 16))
                    return false;
                size_t newSize = value.size();
                return SafeWrite(addr + 16, &newSize, sizeof(newSize));
            }

            uintptr_t ptr = 0;
            if (!SafeRead(addr, &ptr, sizeof(ptr)) || !ptr)
                return false;
            if (value.size() && !SafeWrite(ptr, value.data(), value.size()))
                return false;
            char term = '\0';
            SafeWrite(ptr + value.size(), &term, 1);
            const size_t newSize = value.size();
            return SafeWrite(addr + 16, &newSize, sizeof(size_t));
        }

        inline bool LooksLikeFlagName(const std::string& s)
        {
            if (s.size() < 4 || s.size() > 128)
                return false;
            for (char c : s)
            {
                if (!isalnum(static_cast<unsigned char>(c)) && c != '_')
                    return false;
            }
            return s[0] != '_' && isalpha(static_cast<unsigned char>(s[0]));
        }

        // Reads a flag name from a node using either representation
        // (std::string at +ToFlag, or a raw const char* there).
        inline bool ReadNodeName(uintptr_t node, std::string& out)
        {
            const uintptr_t nameAddr = node + FFlagOffsets::FFlagList::ToFlag;
            if (ReadStdString(nameAddr, out) && LooksLikeFlagName(out))
                return true;

            uintptr_t asCString = 0;
            if (SafeRead(nameAddr, &asCString, sizeof(asCString)) && asCString)
            {
                char buffer[129] = {};
                if (SafeRead(asCString, buffer, 128))
                {
                    buffer[128] = '\0';
                    out.assign(buffer);
                    if (LooksLikeFlagName(out))
                        return true;
                }
            }
            return false;
        }

        // The registry may be stored directly or behind its singleton pointer.
        inline uintptr_t FindFlagNode(const std::string& name)
        {
            uintptr_t head = 0;
            uintptr_t headAddr = 0;
            uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
            if (!base)
                return 0;
            headAddr = ClientOffsets::Flags::Registry;
            uintptr_t indirectRegistry = 0;
            SafeRead(headAddr, &indirectRegistry, sizeof(indirectRegistry));
            for (uintptr_t registry : {headAddr, indirectRegistry})
            {
                if (!registry ||
                    !SafeRead(registry + ClientOffsets::Flags::Reg::ListHead, &head, sizeof(head)) || !head)
                    continue;
                uintptr_t node = 0;
                if (!SafeRead(head + ClientOffsets::Flags::Reg::NodeNext, &node, sizeof(node)))
                    continue;
                std::string nodeName;
                for (size_t steps = 0; node && node != head && steps < ClientOffsets::Flags::Reg::MaxFlags; ++steps)
                {
                    if (!ReadNodeName(node, nodeName))
                        break;
                    if (nodeName == name)
                        return node;
                    uintptr_t next = 0;
                    if (!SafeRead(node + ClientOffsets::Flags::Reg::NodeNext, &next, sizeof(next)))
                        break;
                    if (next == node)
                        break;
                    node = next;
                }
            }
            return 0;
        }

        inline uintptr_t ValueAddr(uintptr_t node)
        {
            uintptr_t entry = 0;
            uintptr_t storage = 0;
            if (!node || !SafeRead(node + ClientOffsets::Flags::Reg::NodeEntry, &entry, sizeof(entry)) || !entry ||
                !SafeRead(entry + ClientOffsets::Flags::Reg::EntryStorage, &storage, sizeof(storage)))
                return 0;
            return storage;
        }

        enum class FlagKind { Unknown, Boolean, Integer, String };

        inline FlagKind GetFlagKind(uintptr_t node)
        {
            uintptr_t entry = 0;
            uintptr_t vtable = 0;
            if (!SafeRead(node + ClientOffsets::Flags::Reg::NodeEntry, &entry, sizeof(entry)) || !entry ||
                !SafeRead(entry, &vtable, sizeof(vtable)))
                return FlagKind::Unknown;
            if (vtable == ClientOffsets::Flags::ValueGetSetBoolVftable ||
                vtable == ClientOffsets::Flags::ValueGetSetDynamicBoolVftable)
                return FlagKind::Boolean;
            if (vtable == ClientOffsets::Flags::ValueGetSetIntVftable ||
                vtable == ClientOffsets::Flags::ValueGetSetDynamicIntVftable)
                return FlagKind::Integer;
            if (vtable == ClientOffsets::Flags::ValueGetSetStringVftable)
                return FlagKind::String;
            return FlagKind::Unknown;
        }

        // Registry vtables identify storage types independently of name prefixes.
        inline bool ReadValueAsString(uintptr_t node, const std::string& name, std::string& out)
        {
            const uintptr_t valueAddr = ValueAddr(node);
            if (!valueAddr) return false;

            const FlagKind kind = GetFlagKind(node);
            const bool boolFlag = kind == FlagKind::Boolean;
            const bool intFlag = kind == FlagKind::Integer;

            // typed flags first — interpreting raw bool/int bytes as a
            // std::string header can false-positive on small values
            if (boolFlag)
            {
                unsigned char b = 0;
                if (SafeRead(valueAddr, &b, sizeof(b)))
                {
                    out = b ? "true" : "false";
                    return true;
                }
                return false;
            }

            if (intFlag)
            {
                int32_t v = 0;
                if (SafeRead(valueAddr, &v, sizeof(v)))
                {
                    out = std::to_string(v);
                    return true;
                }
                return false;
            }

            // FString / string-form registry entries
            if (kind == FlagKind::String && ReadStdString(valueAddr, out))
                return true;
            return false;
        }

        inline bool WriteValueFromString(uintptr_t node, const std::string& name, const std::string& value)
        {
            const uintptr_t valueAddr = ValueAddr(node);
            if (!valueAddr) return false;

            const FlagKind kind = GetFlagKind(node);
            const bool boolFlag = kind == FlagKind::Boolean;
            const bool intFlag = kind == FlagKind::Integer;

            if (boolFlag)
            {
                if (value != "true" && value != "false" && value != "0" && value != "1")
                    return false;
                unsigned char b = (value == "true" || value == "1") ? 1 : 0;
                return SafeWrite(valueAddr, &b, sizeof(b));
            }

            if (intFlag)
            {
                char* end = nullptr;
                errno = 0;
                long long parsed = _strtoi64(value.c_str(), &end, 10);
                if (errno == ERANGE || end == value.c_str() || end != value.c_str() + value.size() ||
                    parsed < INT32_MIN || parsed > INT32_MAX)
                    return false;
                int32_t v32 = static_cast<int32_t>(parsed);
                return SafeWrite(valueAddr, &v32, sizeof(v32));
            }

            // FString / string-form entries: write through the existing
            // buffer (never grows it — no allocator mismatch)
            return kind == FlagKind::String && WriteStdString(valueAddr, value);
        }
    }

    int setfflag(lua_State* L)
    {
        const char* name = luaL_checkstring(L, 1);
        const char* value = luaL_checkstring(L, 2);

        const uintptr_t node = FFlagAccess::FindFlagNode(name);
        if (node)
        {
            if (FFlagAccess::WriteValueFromString(node, name, value))
                return 0;
        }

        if (_stricmp(name, "TaskSchedulerTargetFps") == 0)
        {
            char* end = nullptr;
            errno = 0;
            long long parsed = _strtoi64(value, &end, 10);
            if (errno != ERANGE && end != value && end == value + strlen(value) &&
                parsed >= 0 && parsed <= INT32_MAX)
            {
                bool ok = false;
                [&]() __declspec(noinline) {
                    __try {
                        *reinterpret_cast<int32_t*>(Offsets::Flags::TaskSchedulerTargetFps) = static_cast<int32_t>(parsed);
                        ok = true;
                    } __except (EXCEPTION_EXECUTE_HANDLER) { ok = false; }
                }();
                if (ok) return 0;
            }
        }

        luaL_error(L, ("setfflag: unknown or unsupported flag '%s'"), name);
        return 0;
    }

    int getfflag(lua_State* L)
    {
        const char* name = luaL_checkstring(L, 1);

        const uintptr_t node = FFlagAccess::FindFlagNode(name);
        if (node)
        {
            std::string value;
            if (FFlagAccess::ReadValueAsString(node, name, value))
            {
                lua_pushlstring(L, value.data(), value.size());
                return 1;
            }
        }

        // Fallbacks for common flags when registry lookup fails
        if (_stricmp(name, "TaskSchedulerTargetFps") == 0)
        {
            int32_t cap = 60;
            bool ok = false;
            [&]() __declspec(noinline) {
                __try {
                    cap = *reinterpret_cast<int32_t*>(Offsets::Flags::TaskSchedulerTargetFps);
                    ok = true;
                } __except (EXCEPTION_EXECUTE_HANDLER) { ok = false; }
            }();
            if (ok)
            {
                const int v = cap > 0 ? cap : 60;
                char buf[32]; snprintf(buf, sizeof(buf), "%d", v);
                lua_pushstring(L, buf);
                return 1;
            }
        }
        if (_stricmp(name, "MaxFrameBufferSize") == 0)
        {
            // reasonable default if not discoverable
            lua_pushstring(L, "4096");
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }

    int queueonteleport(lua_State* L)
    {
        size_t length = 0;
        const char* code = luaL_checklstring(L, 1, &length);
        std::lock_guard<std::mutex> lock(Shared::TeleportQueueMutex);
        Shared::TeleportQueue.emplace_back(code, length);
        return 0;
    }

    int clearteleportqueue(lua_State*)
    {
        std::lock_guard<std::mutex> lock(Shared::TeleportQueueMutex);
        Shared::TeleportQueue.clear();
        return 0;
    }

    int gethui(lua_State* L)
    {
        lua_getglobal(L, ("game"));
        if (!lua_isuserdata(L, -1)) { lua_pop(L, 1); lua_pushnil(L); return 1; }

        lua_getfield(L, -1, ("GetService"));
        if (!lua_isfunction(L, -1)) { lua_pop(L, 2); lua_pushnil(L); return 1; }

        lua_pushvalue(L, -2);
        lua_pushstring(L, ("CoreGui"));
        if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
            lua_pop(L, 1);
            lua_pushnil(L);
            return 1;
        }

        if (!lua_isuserdata(L, -1)) { lua_pop(L, 1); lua_pushnil(L); return 1; }
        int coreGuiIdx = lua_gettop(L);

        lua_getfield(L, -1, ("FindFirstChildOfClass"));
        if (!lua_isfunction(L, -1)) { lua_pop(L, 1); lua_pushnil(L); return 1; }

        lua_pushvalue(L, coreGuiIdx);
        lua_pushstring(L, ("ScreenGui"));
        if (lua_pcall(L, 2, 1, 0) == LUA_OK && lua_isuserdata(L, -1)) {
            return 1;
        }

        lua_getglobal(L, ("Instance"));
        lua_getfield(L, -1, ("new"));
        lua_pushstring(L, ("ScreenGui"));
        lua_call(L, 1, 1);
        lua_remove(L, -2);

        if (lua_isuserdata(L, -1)) {
            lua_pushvalue(L, -1);
            lua_setfield(L, coreGuiIdx, ("Name"));
            lua_pushvalue(L, -1);
            lua_setfield(L, coreGuiIdx, ("Parent"));
            return 1;
        }

        lua_pop(L, 1);
        lua_pushnil(L);
        return 1;
    }

    // Full in-game path of an Instance, used as X-Script-Name so the chunk is
    // named after the script instead of an anonymous buffer.
    static bool GetInstancePath(lua_State* L, int index, std::string& out)
    {
        index = lua_absindex(L, index);

        lua_getfield(L, index, ("GetFullName"));
        if (!lua_isfunction(L, -1))
        {
            lua_pop(L, 1);
            return false;
        }

        lua_pushvalue(L, index);
        if (lua_pcall(L, 1, 1, 0) != LUA_OK || !lua_isstring(L, -1))
        {
            lua_pop(L, 1);
            return false;
        }

        const char* value = lua_tostring(L, -1);
        out = value ? value : "";
        lua_pop(L, 1);
        return !out.empty();
    }

    // Reads the bytecode straight off the Instance, exactly as Roblox stores
    // it. getscriptbytecode() is deliberately not used: it rewrites byte 0 to
    // the executor's compiler version before handing the script over, which
    // desynchronises a v10-v14 game chunk and leaves nothing parseable.
    static bool BytecodeFromInstance(lua_State* L, int index, std::string& out, std::string& error)
    {
        std::string className;
        if (!Script::GetInstanceClassName(L, index, className) || !Script::IsScriptClass(className))
        {
            error = "LocalScript, ModuleScript or Script expected";
            return false;
        }

        void** userdata = static_cast<void**>(lua_touserdata(L, index));
        if (!userdata || !*userdata)
        {
            error = "invalid instance";
            return false;
        }

        const std::uintptr_t script = reinterpret_cast<std::uintptr_t>(*userdata);

        bool read = false;
        try
        {
            read = Script::FetchBytecode(script, className == "ModuleScript", out);
        }
        catch (...)
        {
            read = false;
        }

        if (!read || out.empty())
        {
            out.clear();
            error = "could not read script bytecode";
            return false;
        }

        return true;
    }

    int decompile(lua_State* L) {
        
        if (!lua_isuserdata(L, 1) && lua_type(L, 1) != LUA_TSTRING)
            luaL_typeerrorL(L, 1, ("Instance or bytecode string"));

        auto start = std::chrono::steady_clock::now();

        std::string bytecode;
        std::string scriptName;
        std::string readError;

        if (lua_isuserdata(L, 1))
        {
            if (!BytecodeFromInstance(L, 1, bytecode, readError))
            {
                lua_pushlstring(L, readError.c_str(), readError.size());
                return 1;
            }

            if (lua_type(L, 2) == LUA_TSTRING)
                scriptName = lua_tostring(L, 2);
            else
                GetInstancePath(L, 1, scriptName);
        }
        else
        {
            size_t length = 0;
            const char* data = luaL_checklstring(L, 1, &length);
            bytecode.assign(data, length);

            if (lua_type(L, 2) == LUA_TSTRING)
                scriptName = lua_tostring(L, 2);
        }

        int status = 0;
        std::string source = Tovek::Request(bytecode, scriptName, status);

        double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();

        if (status == 200)
        {
            char header[96];
            std::snprintf(header, sizeof(header), "-- Seliware: Decompiled in %.1fs\n\n", elapsed);

            std::string result = std::string(header) + source;
            lua_pushlstring(L, result.c_str(), result.size());
            return 1;
        }

        std::string message = "-- decompile failed";
        if (status > 0)
            message += " (HTTP " + std::to_string(status) + ")";
        message += ": " + source;

        lua_pushlstring(L, message.c_str(), message.size());
        return 1;
    }

    static std::string escape_string(const char* s) {
        if (!s) return "";
        std::string result;
        for (const char* p = s; *p; ++p) {
            switch (*p) {
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += *p; break;
            }
        }
        return result;
    }

    static void serialize_instance(lua_State* L, int instanceIdx, int depth, std::string& out) {
        if (!lua_isuserdata(L, instanceIdx)) return;

        lua_getglobal(L, "typeof");
        lua_pushvalue(L, instanceIdx);
        lua_call(L, 1, 1);
        const char* tp = lua_tostring(L, -1);
        lua_pop(L, 1);
        if (!tp || strcmp(tp, "Instance") != 0) return;

        lua_getfield(L, instanceIdx, "ClassName");
        const char* className = lua_isstring(L, -1) ? lua_tostring(L, -1) : "Instance";
        lua_pop(L, 1);

        lua_getfield(L, instanceIdx, "Name");
        const char* instName = lua_isstring(L, -1) ? lua_tostring(L, -1) : "";
        lua_pop(L, 1);

        std::string indent(depth * 4, ' ');
        std::string varName = "inst_" + std::to_string(reinterpret_cast<uintptr_t>(lua_touserdata(L, instanceIdx)));

        out += indent + "local " + varName + " = Instance.new(\"" + className + "\")\n";
        out += indent + varName + ".Name = \"" + escape_string(instName) + "\"\n";

        lua_getfield(L, instanceIdx, "ClassName");
        const char* cn = lua_isstring(L, -1) ? lua_tostring(L, -1) : "";
        lua_pop(L, 1);

        if (strcmp(cn, "Part") == 0 || strcmp(cn, "MeshPart") == 0 || strcmp(cn, "UnionOperation") == 0 ||
            strcmp(cn, "SpawnLocation") == 0 || strcmp(cn, "WedgePart") == 0 || strcmp(cn, "CornerWedgePart") == 0) {
            lua_getfield(L, instanceIdx, "Position");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "X"); double px = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_getfield(L, -1, "Y"); double py = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_getfield(L, -1, "Z"); double pz = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pop(L, 1);
                char buf[256];
                snprintf(buf, sizeof(buf), "%s%s.Position = Vector3.new(%g, %g, %g)\n", indent.c_str(), varName.c_str(), px, py, pz);
                out += buf;
            }
            else { lua_pop(L, 1); }

            lua_getfield(L, instanceIdx, "Size");
            if (lua_istable(L, -1)) {
                lua_getfield(L, -1, "X"); double sx = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_getfield(L, -1, "Y"); double sy = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_getfield(L, -1, "Z"); double sz = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pop(L, 1);
                char buf[256];
                snprintf(buf, sizeof(buf), "%s%s.Size = Vector3.new(%g, %g, %g)\n", indent.c_str(), varName.c_str(), sx, sy, sz);
                out += buf;
            }
            else { lua_pop(L, 1); }

            lua_getfield(L, instanceIdx, "BrickColor");
            if (!lua_isnil(L, -1) && !lua_isnone(L, -1)) {
                lua_getfield(L, -1, "Name");
                const char* bc = lua_isstring(L, -1) ? lua_tostring(L, -1) : nullptr;
                lua_pop(L, 1);
                lua_pop(L, 1);
                if (bc) {
                    out += indent + varName + ".BrickColor = BrickColor.new(\"" + bc + "\")\n";
                }
            }
            else { lua_pop(L, 1); }

            lua_getfield(L, instanceIdx, "Anchored");
            if (!lua_isnil(L, -1)) {
                out += indent + varName + ".Anchored = " + (lua_toboolean(L, -1) ? "true" : "false") + "\n";
            }
            lua_pop(L, 1);
        }

        lua_getfield(L, instanceIdx, "Parent");
        lua_pop(L, 1);

        out += "\n";
    }

    static void serialize_children(lua_State* L, int instanceIdx, int depth, std::string& out) {
        lua_getfield(L, instanceIdx, "GetChildren");
        if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
        lua_pushvalue(L, instanceIdx);
        if (lua_pcall(L, 1, 1, 0) != LUA_OK) { lua_pop(L, 1); return; }
        if (!lua_istable(L, -1)) { lua_pop(L, 1); return; }

        int tableIdx = lua_gettop(L);
        int len = (int)lua_objlen(L, tableIdx);
        for (int i = 1; i <= len; ++i) {
            lua_rawgeti(L, tableIdx, i);
            if (lua_isuserdata(L, -1)) {
                serialize_instance(L, -1, depth, out);
                serialize_children(L, lua_gettop(L), depth + 1, out);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }

    int saveinstance(lua_State* L) {
        
        const char* savePath = nullptr;
        int top = lua_gettop(L);

        if (top >= 2 && lua_istable(L, 2)) {
            lua_getfield(L, 2, "FileName");
            if (lua_isstring(L, -1)) {
                savePath = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
        }
        else if (top >= 1 && lua_isstring(L, 1)) {
            savePath = lua_tostring(L, 1);
        }

        lua_getglobal(L, "game");
        if (!lua_isuserdata(L, -1)) {
            lua_pop(L, 1);
            luaL_error(L, ("game not found"));
            return 0;
        }
        int gameIdx = lua_gettop(L);

        std::string output;
        output += "-- Generated by executor saveinstance\n";
        output += "local game = game\n";

        lua_getfield(L, gameIdx, "PlaceId");
        char buf[256];
        snprintf(buf, sizeof(buf), "local PlaceId = %g\n", lua_tonumber(L, -1));
        output += buf;
        lua_pop(L, 1);

        lua_getfield(L, gameIdx, "JobId");
        lua_pop(L, 1);

        output += "\n";

        serialize_instance(L, gameIdx, 0, output);
        serialize_children(L, gameIdx, 1, output);

        lua_pop(L, 1);

        std::string outName;
        wchar_t pathBuf[MAX_PATH] = {};
        if (savePath && savePath[0]) {
            outName = savePath;
            if (!Shared::workspace_folder.empty()) {
                std::wstring wsFolder = Shared::workspace_folder.wstring();
                wcscpy_s(pathBuf, MAX_PATH, wsFolder.c_str());
                wchar_t fileName[MAX_PATH] = {};
                MultiByteToWideChar(CP_UTF8, 0, savePath, -1, fileName, MAX_PATH);
                wcscat_s(pathBuf, MAX_PATH, L"\\");
                wcscat_s(pathBuf, MAX_PATH, fileName);
            }
            else {
                MultiByteToWideChar(CP_UTF8, 0, savePath, -1, pathBuf, MAX_PATH);
            }
        }
        else {
            outName = "game.rbxlx";
            if (!Shared::workspace_folder.empty()) {
                std::wstring wsFolder = Shared::workspace_folder.wstring();
                wcscpy_s(pathBuf, MAX_PATH, wsFolder.c_str());
                wcscat_s(pathBuf, MAX_PATH, L"\\game.rbxlx");
            }
            else {
                wchar_t desktop[MAX_PATH] = {};
                SHGetFolderPathW(NULL, CSIDL_DESKTOP, NULL, 0, desktop);
                wcscpy_s(pathBuf, MAX_PATH, desktop);
                wcscat_s(pathBuf, MAX_PATH, L"\\game.rbxlx");
            }
        }

        // Ensure parent directory exists for provided paths
        {
            std::filesystem::path p(pathBuf);
            std::error_code ec;
            if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path(), ec);
        }

        std::ofstream file(pathBuf, std::ios::binary);
        if (file.is_open()) {
            file << output;
            file.close();
        }
        lua_pushlstring(L, outName.data(), outName.size());
        return 1;
    }

    // ══════════════════════════════════════════════════════════════
    //  Registration
    // ══════════════════════════════════════════════════════════════

    void register_lib(lua_State* L)
    {
        Environment->make_global(L, ("getgenv"), getgenv, Miscellaneous::funcnameinc++);
        Environment->make_global(L, ("decompile"), decompile, Miscellaneous::funcnameinc++);
        Environment->make_global(L, ("saveinstance"), saveinstance, Miscellaneous::funcnameinc++);
        Environment->make_global(L, ("identifyexecutor"), identifyexecutor, Miscellaneous::funcnameinc++);
        Environment->make_global(L, ("getexecutorname"), getexecutorname, Miscellaneous::funcnameinc++);
        Environment->make_aliases(L, setthreadidentity, Miscellaneous::funcnameinc, { ("setthreadidentity"), ("setidentity"), ("setthreadcontext") });
       Environment->make_aliases(L, getthreadidentity, Miscellaneous::funcnameinc, { ("getthreadidentity"), ("getidentity"), ("getthreadcontext") });
       Environment->make_global(L, ("setfpscap"), setfpscap, Miscellaneous::funcnameinc++);
       Environment->make_global(L, ("getfpscap"), getfpscap, Miscellaneous::funcnameinc++);
       Environment->make_global(L, ("setfflag"), setfflag, Miscellaneous::funcnameinc++);
       Environment->make_global(L, ("getfflag"), getfflag, Miscellaneous::funcnameinc++);
       Environment->make_global(L, ("GetObjects"), GetObjects, Miscellaneous::funcnameinc++);
      Environment->make_aliases(L, queueonteleport, Miscellaneous::funcnameinc, { ("queueonteleport"), ("queue_on_teleport") });
     Environment->make_aliases(L, clearteleportqueue, Miscellaneous::funcnameinc, { ("clearteleportqueue"), ("clear_teleport_queue"), ("clearqueueonteleport") });
       Environment->make_aliases(L, gethui, Miscellaneous::funcnameinc, { ("gethui"), ("get_hidden_ui"), ("get_hidden_gui") });
    }
}
