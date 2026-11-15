#pragma once

#include <Windows.h>

#include <string>

// Where this module lives on disk.
//
// The emulator hides Module.dll from the loader lists right after it is
// loaded. Once that happens GetModuleHandleEx(FROM_ADDRESS) no longer finds
// the module, so every lookup of "the folder of Module.dll" has to use the
// path that was captured while the loader still knew about it.
namespace ModuleLocation
{
    inline std::wstring Directory;
    inline std::wstring Path;

    // Call this from DllMain, before the module can be hidden.
    inline void Capture(HMODULE Module)
    {
        if (!Module)
            return;

        wchar_t buffer[MAX_PATH]{};
        const DWORD length = GetModuleFileNameW(Module, buffer, MAX_PATH);

        if (length == 0 || length >= MAX_PATH)
            return;

        Path.assign(buffer, length);

        const size_t slash = Path.find_last_of(L"\\/");

        Directory = (slash == std::wstring::npos) ? std::wstring() : Path.substr(0, slash);
    }

    // Folder that holds Module.dll. Falls back to asking the loader when the
    // module was never captured (e.g. the file was mapped by hand).
    inline std::wstring Resolve()
    {
        if (!Directory.empty())
            return Directory;

        HMODULE module = nullptr;

        if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&Resolve), &module) || !module)
            return {};

        wchar_t buffer[MAX_PATH]{};
        const DWORD length = GetModuleFileNameW(module, buffer, MAX_PATH);

        if (length == 0 || length >= MAX_PATH)
            return {};

        Path.assign(buffer, length);

        const size_t slash = Path.find_last_of(L"\\/");

        Directory = (slash == std::wstring::npos) ? std::wstring() : Path.substr(0, slash);
        return Directory;
    }
}
