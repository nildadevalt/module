#pragma once

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <windows.h>

#include <managers/logging/logging.h>

// Unified logging
//
//   LOG();                                          -> appends "CALL <functionname>" to Logs.log
//   LOGF("TaskScheduler", "found WHSJ at 0x%llX", addr); -> OutputDebugString + ImGui log + Logs.log
//
// Logs.log is created next to Module.dll (the directory this dll was loaded from)
// and is (re)created empty every time the dll is loaded.

#ifdef _DEBUG
    #define LOG_ENABLED 1
#else
    #define LOG_ENABLED 0
#endif

namespace LogFile
{
    struct State
    {
        CRITICAL_SECTION lock;
        HANDLE handle;
        char path[MAX_PATH];
    };

    inline State MakeState()
    {
        State state{};

        InitializeCriticalSection(&state.lock);
        state.handle = INVALID_HANDLE_VALUE;
        state.path[0] = '\0';

        char module_path[MAX_PATH]{};
        HMODULE module = nullptr;

        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(&MakeState), &module) && module)
        {
            GetModuleFileNameA(module, module_path, MAX_PATH);
        }

        char* slash = strrchr(module_path, '\\');
        if (slash)
        {
            slash[1] = '\0';
            snprintf(state.path, MAX_PATH, "%sLogs.log", module_path);

            state.handle = CreateFileA(state.path, FILE_APPEND_DATA,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

            if (state.handle != INVALID_HANDLE_VALUE)
                SetFilePointer(state.handle, 0, nullptr, FILE_END);
        }

        return state;
    }

    inline State& Get()
    {
        static State state = MakeState();
        return state;
    }

    inline void Init()
    {
        Get();
    }

    inline const char* Path()
    {
        return Get().path;
    }

    inline void Write(const char* text, int length)
    {
        if (!text || length <= 0)
            return;

        State& state = Get();
        if (state.handle == INVALID_HANDLE_VALUE)
            return;

        EnterCriticalSection(&state.lock);

        DWORD written = 0;
        WriteFile(state.handle, text, static_cast<DWORD>(length), &written, nullptr);
        FlushFileBuffers(state.handle);

        LeaveCriticalSection(&state.lock);
    }

    // called by LOG() — one line per entry: "CALL <functionname>"
    inline void Call(const char* name)
    {
        if (!name)
            return;

        char line[512]{};
        const int length = snprintf(line, sizeof(line), "CALL %s\n", name);
        Write(line, length);
    }

    // called by LOGF() — one line per entry: "[source] message"
    inline void Message(const char* source, const char* text)
    {
        if (!text)
            return;

        char line[2300]{};
        const int length = snprintf(line, sizeof(line), "[%s] %s\n", source ? source : "log", text);
        Write(line, length);
    }
}

inline void Log_Internal(const char* source, const char* fmt, ...)
{
    char msg[2048]{};

    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    // OutputDebugStringA — visible in DbgView / debugger
    char dbg[2300]{};
    snprintf(dbg, sizeof(dbg), "[justafatguy67][%s] %s\n", source, msg);
    OutputDebugStringA(dbg);

    // ImGui in-app log
    log_mgr.add_log(dbg);

    // Logs.log next to the dll
    LogFile::Message(source, msg);
}

// call at the top of a function: writes "CALL <functionname>" into Logs.log
#define LOG() LogFile::Call(__FUNCTION__)

// formatted log: OutputDebugString + ImGui + Logs.log
#define LOGF(source, ...) do { Log_Internal(source, __VA_ARGS__); } while(0)
