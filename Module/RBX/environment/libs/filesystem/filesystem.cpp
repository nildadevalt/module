#include <RBX/environment/libs/filesystem/filesystem.hpp>
#include <RBX/environment/env.hpp>

#include <lstate.h>
#include <lualib.h>
#include <sstream>
#include <fstream>
#include <ShlObj.h>

#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>
#include <RBX/execution/execution.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <RBX/environment/yielding/yielding.hpp>


namespace Filesystem
{
    namespace
    {
        constexpr DWORD MAX_PATH_W = 4096;

        static const wchar_t* BLOCKED_EXTS[] = {
            L".exe", L".dll", L".bat", L".cmd", L".vbs", L".ps1", L".msi", L".scr",
            L".com", L".pif", L".sh", L".cpl", L".msc", L".js", L".jse", L".wsf",
            L".wsh", L".hta", L".scf", L".lnk", L".zip", L".rar", L".7z", L".cab",
            L".iso", L".img", L".xml", L".msp", L".reg", L".py", L".pyc", L".pyw",
            L".rb", L".pl", L".php", L".jar", L".sys", L".drv", L".ocx", L".psd1",
            L".vbe", L".inf", L".url", L".ts", L".tar", L".gz", L".xz", L".bz2",
            L".tgz", L".wim", L".docm", L".pptm", L".xlsm", L".vba", nullptr
        };

        bool ResolvePath(lua_State* L, int idx, wchar_t* out, DWORD outSize, const char** err)
        {
            out[0] = L'\0';

            if (Shared::workspace_folder.empty())
            {
                *err = ("Invalid path");
                return false;
            }

            wchar_t root[MAX_PATH_W] = {};
            if (!GetFullPathNameW(Shared::workspace_folder.wstring().c_str(), MAX_PATH_W, root, nullptr) || GetFullPathNameW(Shared::workspace_folder.wstring().c_str(), MAX_PATH_W, root, nullptr) >= MAX_PATH_W)
            {
                *err = ("Invalid path");
                return false;
            }

            for (wchar_t* p = root; *p; p++)
                if (*p == L'/') *p = L'\\';

            wchar_t* end = root + wcslen(root);
            while (end > root + 2 && (*(end - 1) == L'\\' || *(end - 1) == L'/'))
                *--end = L'\0';

            if (idx > lua_gettop(L) || lua_isnil(L, idx))
            {
                wcscpy_s(out, outSize, root);
                return true;
            }

            size_t len = 0;
            const char* raw = luaL_checklstring(L, idx, &len);
            if (!raw || len == 0)
            {
                wcscpy_s(out, outSize, root);
                return true;
            }

            wchar_t rel[MAX_PATH_W] = {};
            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, raw, (int)len, rel, MAX_PATH_W) <= 0)
            {
                *err = ("Invalid path");
                return false;
            }

            for (wchar_t* p = rel; *p; p++)
                if (*p == L'/') *p = L'\\';

            if (rel[0] == L'\\' || rel[0] == L'/' || (rel[0] && rel[1] == L':'))
            {
                *err = ("Path outside workspace");
                return false;
            }

            std::wstring combined = std::wstring(root) + L'\\' + rel;
            if (!GetFullPathNameW(combined.c_str(), outSize, out, nullptr) || GetFullPathNameW(combined.c_str(), outSize, out, nullptr) >= outSize)
            {
                *err = ("Invalid path");
                return false;
            }

            for (wchar_t* p = out; *p; p++)
                if (*p == L'/') *p = L'\\';

            end = out + wcslen(out);
            while (end > out + 2 && *(end - 1) == L'\\')
                *--end = L'\0';

            size_t rootLen = wcslen(root);
            if (_wcsnicmp(out, root, rootLen) != 0 || (out[rootLen] != L'\0' && out[rootLen] != L'\\'))
            {
                *err = ("Path outside workspace");
                return false;
            }

            return true;
        }

        wchar_t* GetRoot(wchar_t* buf)
        {
            buf[0] = L'\0';
            if (Shared::workspace_folder.empty()) return buf;
            GetFullPathNameW(Shared::workspace_folder.wstring().c_str(), MAX_PATH_W, buf, nullptr);
            for (wchar_t* p = buf; *p; p++) if (*p == L'/') *p = L'\\';
            wchar_t* end = buf + wcslen(buf);
            while (end > buf + 2 && *(end - 1) == L'\\') *--end = L'\0';
            return buf;
        }

        bool FileIO(const wchar_t* path, char** data, size_t* len, const char* writeData, size_t writeLen, bool append)
        {
            if (writeData)
            {
                wchar_t parent[MAX_PATH_W];
                wcscpy_s(parent, path);
                wchar_t* slash = wcsrchr(parent, L'\\');
                if (slash)
                {
                    *slash = L'\0';
                    wchar_t* start = parent;
                    if (parent[0] && parent[1] == L':' && parent[2] == L'\\') start = parent + 3;
                    for (wchar_t* p = start; *p; p++)
                    {
                        if (*p != L'\\') continue;
                        *p = L'\0';
                        if (*parent) CreateDirectoryW(parent, nullptr);
                        *p = L'\\';
                    }
                    CreateDirectoryW(parent, nullptr);
                }

                HANDLE h = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, append ? OPEN_ALWAYS : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
                if (h == INVALID_HANDLE_VALUE) return false;
                if (append) SetFilePointer(h, 0, nullptr, FILE_END);

                size_t off = 0;
                while (off < writeLen)
                {
                    DWORD chunk = (DWORD)std::min<size_t>(writeLen - off, 0x7ffff000), written = 0;
                    if (!WriteFile(h, writeData + off, chunk, &written, nullptr) || !written) { CloseHandle(h); return false; }
                    off += written;
                }
                CloseHandle(h);
                return true;
            }

            *data = nullptr;
            *len = 0;
            HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
            if (h == INVALID_HANDLE_VALUE) return false;

            LARGE_INTEGER sz;
            if (!GetFileSizeEx(h, &sz) || sz.QuadPart < 0 || sz.QuadPart > SIZE_MAX) { CloseHandle(h); return false; }

            *len = (size_t)sz.QuadPart;
            *data = (char*)HeapAlloc(GetProcessHeap(), 0, *len ? *len : 1);
            if (!*data) { CloseHandle(h); return false; }

            size_t off = 0;
            while (off < *len)
            {
                DWORD chunk = (DWORD)std::min<size_t>(*len - off, 0x7ffff000), read = 0;
                if (!ReadFile(h, *data + off, chunk, &read, nullptr)) { HeapFree(GetProcessHeap(), 0, *data); *data = nullptr; *len = 0; CloseHandle(h); return false; }
                if (!read) break;
                off += read;
            }
            *len = off;
            CloseHandle(h);
            return true;
        }

        bool DeleteFolder(const wchar_t* path)
        {
            wchar_t search[MAX_PATH_W];
            swprintf_s(search, L"%s\\*", path);
            WIN32_FIND_DATAW fd = {};
            HANDLE h = FindFirstFileW(search, &fd);
            if (h != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
                    wchar_t child[MAX_PATH_W];
                    swprintf_s(child, L"%s\\%s", path, fd.cFileName);
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) DeleteFolder(child);
                    else { SetFileAttributesW(child, FILE_ATTRIBUTE_NORMAL); DeleteFileW(child); }
                } while (FindNextFileW(h, &fd));
                FindClose(h);
            }
            SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL);
            return RemoveDirectoryW(path);
        }

        bool IsBlocked(const wchar_t* path)
        {
            const wchar_t* name = wcsrchr(path, L'\\');
            name = name ? name + 1 : path;
            const wchar_t* ext = wcsrchr(name, L'.');
            if (!ext) return false;
            for (int i = 0; BLOCKED_EXTS[i]; i++)
                if (!_wcsicmp(ext, BLOCKED_EXTS[i])) return true;
            return false;
        }

        // creates every missing directory leading up to a file path
        bool EnsureParentDirectory(const wchar_t* filePath)
        {
            wchar_t parent[MAX_PATH_W];
            wcscpy_s(parent, filePath);
            wchar_t* slash = wcsrchr(parent, L'\\');
            if (!slash) return true;

            *slash = L'\0';
            wchar_t* start = parent;
            if (parent[0] && parent[1] == L':' && parent[2] == L'\\') start = parent + 3;
            for (wchar_t* p = start; *p; p++)
            {
                if (*p != L'\\') continue;
                *p = L'\0';
                if (*parent && !CreateDirectoryW(parent, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
                {
                    *p = L'\\';
                    return false;
                }
                *p = L'\\';
            }
            if (*parent && !CreateDirectoryW(parent, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
                return false;
            return true;
        }

        bool IsRootRequest(lua_State* L, int idx)
        {
            size_t len = 0;
            const char* s = luaL_checklstring(L, idx, &len);
            if (!s || !len) return true;
            int dots = 0;
            for (size_t i = 0; i < len; i++)
            {
                if (s[i] == '/' || s[i] == '\\') continue;
                if (s[i] == '.') { dots++; continue; }
                return false;
            }
            return dots <= 2;
        }

        int Err(lua_State* L, const char* msg)
        {
            int top = lua_gettop(L);
            lua_getglobal(L, ("error"));
            if (lua_isfunction(L, -1))
            {
                lua_pushstring(L, msg);
                lua_call(L, 1, 0);
                return 0;
            }
            lua_settop(L, top);
            lua_pushnil(L);
            lua_pushstring(L, msg);
            return 2;
        }
    }

    int readfile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY)) return Err(L, ("File does not exist"));
        char* data = nullptr; size_t len = 0;
        if (!FileIO(path, &data, &len, nullptr, 0, false)) return Err(L, ("Failed to open file"));
        lua_pushlstring(L, data, len);
        HeapFree(GetProcessHeap(), 0, data);
        return 1;
    }

    // debUNC checks that scripts cannot drop a runnable file such as .exe or .psm1
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

        // Windows treats "file.exe::$DATA" as the file itself, so drop a stream suffix.
        size_t stream = str.find(L':', 2);
        if (stream != std::wstring::npos)
            str.erase(stream);

        // A trailing dot or space is ignored by Windows: "file.exe." is "file.exe".
        while (!str.empty() && (str.back() == L'.' || str.back() == L' '))
            str.pop_back();

        for (int i = 0; kExts[i]; ++i)
        {
            std::wstring ext(kExts[i]);
            if (str.size() > ext.size() && str.compare(str.size() - ext.size(), ext.size(), ext) == 0)
                return true;
        }
        return false;
    }

    int writefile(lua_State* L)
    {
        luaL_checktype(L, 2, LUA_TSTRING);
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, false)) return Err(L, ("Failed to write file"));
        return 0;
    }

    int appendfile(lua_State* L)
    {
        luaL_checktype(L, 2, LUA_TSTRING);
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        if (IsBlocked(path)) return Err(L, ("Unsafe function"));
        if (IsProgramExtension(path)) return Err(L, ("Writing program files is not allowed"));
        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        if (!FileIO(path, nullptr, nullptr, data, len, true)) return Err(L, ("Failed to append file"));
        return 0;
    }

    // ------------------------------------------------------------------
    // Async file I/O
    //
    // Disk work runs on the yield worker thread and only the result is
    // pushed back on the game thread, so touching a slow or large file
    // never stalls the scheduler step. Falls back to the blocking path
    // when the calling thread cannot yield.
    // ------------------------------------------------------------------

    int readfileasync(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);

        if (lua_isyieldable(L))
        {
            std::wstring wpath(path);
            return Yielding::YieldExecution(L, [wpath]() -> std::function<int(lua_State*)>
            {
                char* data = nullptr; size_t len = 0;
                bool ok = FileIO(wpath.c_str(), &data, &len, nullptr, 0, false);
                std::string body;
                if (ok)
                {
                    body.assign(data, len);
                    HeapFree(GetProcessHeap(), 0, data);
                }
                return [ok, body = std::move(body)](lua_State* L) -> int
                {
                    if (!ok)
                        return Err(L, ("Failed to open file"));
                    lua_pushlstring(L, body.data(), body.size());
                    return 1;
                };
            });
        }

        return readfile(L);
    }

    int writefileasync(lua_State* L)
    {
        luaL_checktype(L, 2, LUA_TSTRING);
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        if (IsBlocked(path)) return Err(L, ("Unsafe function"));

        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        std::wstring wpath(path);
        std::string body(data, len);

        if (lua_isyieldable(L))
        {
            return Yielding::YieldExecution(L, [wpath, body = std::move(body)]() -> std::function<int(lua_State*)>
            {
                bool ok = FileIO(wpath.c_str(), nullptr, nullptr, body.data(), body.size(), false);
                return [ok](lua_State* L) -> int
                {
                    if (!ok)
                        return Err(L, ("Failed to write file"));
                    return 0;
                };
            });
        }

        if (!FileIO(path, nullptr, nullptr, body.data(), body.size(), false)) return Err(L, ("Failed to write file"));
        return 0;
    }

    int appendfileasync(lua_State* L)
    {
        luaL_checktype(L, 2, LUA_TSTRING);
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        if (IsBlocked(path)) return Err(L, ("Unsafe function"));

        size_t len = 0; const char* data = luaL_checklstring(L, 2, &len);
        std::wstring wpath(path);
        std::string body(data, len);

        if (lua_isyieldable(L))
        {
            return Yielding::YieldExecution(L, [wpath, body = std::move(body)]() -> std::function<int(lua_State*)>
            {
                bool ok = FileIO(wpath.c_str(), nullptr, nullptr, body.data(), body.size(), true);
                return [ok](lua_State* L) -> int
                {
                    if (!ok)
                        return Err(L, ("Failed to append file"));
                    return 0;
                };
            });
        }

        if (!FileIO(path, nullptr, nullptr, body.data(), body.size(), true)) return Err(L, ("Failed to append file"));
        return 0;
    }

    int loadfileasync(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);

        char chunk[512];
        strcpy_s(chunk, ("loadfileasync"));
        const wchar_t* fname = wcsrchr(path, L'\\');
        WideCharToMultiByte(CP_UTF8, 0, fname ? fname + 1 : path, -1, chunk, sizeof(chunk), nullptr, nullptr);

        if (lua_isyieldable(L))
        {
            std::wstring wpath(path);
            std::string chunkName(chunk);
            return Yielding::YieldExecution(L, [wpath, chunkName]() -> std::function<int(lua_State*)>
            {
                char* data = nullptr; size_t len = 0;
                bool ok = FileIO(wpath.c_str(), &data, &len, nullptr, 0, false);
                std::string bytecode;
                if (ok)
                {
                    std::string src(data, len);
                    HeapFree(GetProcessHeap(), 0, data);
                    bytecode = Execution::CompileScript(std::move(src));
                }
                return [ok, bytecode = std::move(bytecode), chunkName = std::move(chunkName)](lua_State* L) -> int
                {
                    if (!ok)
                        return Err(L, ("Failed to open file"));
                    if (luau_load(L, chunkName.c_str(), bytecode.data(), bytecode.size(), 0) != LUA_OK)
                    {
                        const char* e = lua_tostring(L, -1);
                        std::string msg = e ? e : std::string(("loadfileasync failed"));
                        lua_pop(L, 1);
                        lua_pushnil(L);
                        lua_pushstring(L, msg.c_str());
                        return 2;
                    }
                    Closure* cl = clvalue(luaA_toobject(L, -1));
                    task_scheduler::SetProtoCapabilities(cl->l.p, &MaxCapabilities);
                    return 1;
                };
            });
        }

        return loadfile(L);
    }

    int listfiles(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {}, root[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        GetRoot(root);
        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES || !(a & FILE_ATTRIBUTE_DIRECTORY)) return Err(L, ("Folder does not exist"));

        lua_newtable(L);
        wchar_t search[MAX_PATH_W];
        swprintf_s(search, L"%s\\*", path);
        WIN32_FIND_DATAW fd = {};
        HANDLE h = FindFirstFileW(search, &fd);
        if (h == INVALID_HANDLE_VALUE) return 1;

        int i = 1;
        size_t rootLen = wcslen(root);
        do
        {
            if (!wcscmp(fd.cFileName, L".") || !wcscmp(fd.cFileName, L"..")) continue;
            wchar_t full[MAX_PATH_W];
            swprintf_s(full, L"%s\\%s", path, fd.cFileName);
            const wchar_t* rel = full;
            if (_wcsnicmp(full, root, rootLen) == 0 && full[rootLen] == L'\\') rel = full + rootLen + 1;
            char utf8[2048];
            if (WideCharToMultiByte(CP_UTF8, 0, rel, -1, utf8, sizeof(utf8), nullptr, nullptr) > 0)
            {
                lua_pushstring(L, utf8);
                lua_rawseti(L, -2, i++);
            }
        } while (FindNextFileW(h, &fd));
        FindClose(h);
        return 1;
    }

    int isfile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        DWORD a = GetFileAttributesW(path);
        lua_pushboolean(L, a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY));
        return 1;
    }

    int isfolder(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        DWORD a = GetFileAttributesW(path);
        lua_pushboolean(L, a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
        return 1;
    }

    int makefolder(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);

        wchar_t* start = path;
        if (path[0] && path[1] == L':' && path[2] == L'\\') start = path + 3;

        for (wchar_t* p = start; *p; p++)
        {
            if (*p != L'\\') continue;
            *p = L'\0';
            if (!CreateDirectoryW(path, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
            {
                *p = L'\\';
                return Err(L, ("Failed to create folder"));
            }
            *p = L'\\';
        }

        if (!CreateDirectoryW(path, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
            return Err(L, ("Failed to create folder"));

        return 0;
    }

    int delfile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY)) return Err(L, ("File does not exist"));
        if (!DeleteFileW(path)) return Err(L, ("Failed to delete file"));
        return 0;
    }

    int delfolder(lua_State* L)
    {
        if (IsRootRequest(L, 1)) return Err(L, ("Cannot delete workspace folder"));
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {}, root[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        GetRoot(root);
        if (!_wcsicmp(path, root)) return Err(L, ("Cannot delete workspace folder"));
        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES || !(a & FILE_ATTRIBUTE_DIRECTORY)) return Err(L, ("Folder does not exist"));
        if (!DeleteFolder(path)) return Err(L, ("Failed to remove folder"));
        return 0;
    }

    int copyfile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t src[MAX_PATH_W] = {}, dst[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, src, MAX_PATH_W, &err)) return Err(L, err);
        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);
        if (IsProgramExtension(dst)) return Err(L, ("Writing program files is not allowed"));

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY))
            return Err(L, ("Source file does not exist"));
        if (IsBlocked(src) || IsBlocked(dst))
            return Err(L, ("Unsafe function"));
        if (!_wcsicmp(src, dst))
            return Err(L, ("Source and destination are the same file"));
        if (!EnsureParentDirectory(dst))
            return Err(L, ("Failed to create destination folder"));
        if (!CopyFileW(src, dst, FALSE))
            return Err(L, ("Failed to copy file"));
        return 0;
    }

    int movefile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t src[MAX_PATH_W] = {}, dst[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, src, MAX_PATH_W, &err)) return Err(L, err);
        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);
        if (IsProgramExtension(dst)) return Err(L, ("Writing program files is not allowed"));

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES)
            return Err(L, ("File does not exist"));
        const bool srcIsDir = (a & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (!srcIsDir && (IsBlocked(src) || IsBlocked(dst)))
            return Err(L, ("Unsafe function"));
        if (!_wcsicmp(src, dst))
            return Err(L, ("Source and destination are the same file"));
        if (!srcIsDir && !EnsureParentDirectory(dst))
            return Err(L, ("Failed to create destination folder"));

        if (!MoveFileExW(src, dst, MOVEFILE_REPLACE_EXISTING))
            return Err(L, ("Failed to move file"));
        return 0;
    }

    int renamefile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t src[MAX_PATH_W] = {}, dst[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, src, MAX_PATH_W, &err)) return Err(L, err);
        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES)
            return Err(L, ("File does not exist"));
        const bool srcIsDir = (a & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (!srcIsDir && (IsBlocked(src) || IsBlocked(dst)))
            return Err(L, ("Unsafe function"));
        if (!_wcsicmp(src, dst))
            return 0;
        if (!srcIsDir && !EnsureParentDirectory(dst))
            return Err(L, ("Failed to create destination folder"));

        if (!MoveFileExW(src, dst, MOVEFILE_REPLACE_EXISTING))
            return Err(L, ("Failed to rename file"));
        return 0;
    }

    int getfilesize(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);

        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES)
            return Err(L, ("File does not exist"));
        if (a & FILE_ATTRIBUTE_DIRECTORY)
            return Err(L, ("Path is a folder"));

        HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h == INVALID_HANDLE_VALUE)
            return Err(L, ("Failed to open file"));

        LARGE_INTEGER sz = {};
        if (!GetFileSizeEx(h, &sz))
        {
            CloseHandle(h);
            return Err(L, ("Failed to get file size"));
        }
        CloseHandle(h);

        lua_pushinteger(L, (lua_Integer)sz.QuadPart);
        return 1;
    }

    int loadfile(lua_State* L)
    {
        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        char* data = nullptr; size_t len = 0;
        if (!FileIO(path, &data, &len, nullptr, 0, false)) return Err(L, ("Failed to open file"));

        char chunk[512];
        strcpy_s(chunk, ("loadfile"));
        const wchar_t* fname = wcsrchr(path, L'\\');
        WideCharToMultiByte(CP_UTF8, 0, fname ? fname + 1 : path, -1, chunk, sizeof(chunk), nullptr, nullptr);

        std::string src(data, len);
        HeapFree(GetProcessHeap(), 0, data);
        auto bc = Execution::CompileScript(src);

        if (luau_load(L, chunk, bc.data(), bc.size(), 0) != LUA_OK)
        {
            const char* e = lua_tostring(L, -1);
            std::string msg = e ? e : std::string(("loadfile failed"));
            lua_pop(L, 1);
            lua_pushnil(L);
            lua_pushstring(L, msg.c_str());
            return 2;
        }

        Closure* cl = clvalue(luaA_toobject(L, -1));
        task_scheduler::SetProtoCapabilities(cl->l.p, &MaxCapabilities);
        lua_setsafeenv(L, LUA_ENVIRONINDEX, false);
        return 1;
    }

    int dofile(lua_State* L)
    {
        // forward every value the chunk returns; passing 0 here dropped them
        // and made dofile() report nil for any file that returned something.
        if (loadfile(L) != 1)
            return 2;

        // loadfile leaves exactly the chunk on the stack; run it and return
        // only the call's results, not the stack below the chunk.
        const int base = lua_gettop(L) - 1;
        lua_call(L, 0, LUA_MULTRET);
        return lua_gettop(L) - base;
    }

    int getcustomasset(lua_State* L)
    {
        static std::string randomFolder = GenerateRandomName(10);

        const char* err = nullptr;
        wchar_t path[MAX_PATH_W] = {};
        if (!ResolvePath(L, 1, path, MAX_PATH_W, &err)) return Err(L, err);
        DWORD a = GetFileAttributesW(path);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY)) return Err(L, ("file does not exist"));

        char exePath[MAX_PATH];
        GetModuleFileNameA(nullptr, exePath, MAX_PATH);
        auto base = std::filesystem::path(exePath).parent_path();
        auto assetDir = base / ("ExtraContent") / randomFolder;

        if (!std::filesystem::exists(assetDir))
            std::filesystem::create_directories(assetDir);

        std::filesystem::path src(path);
        auto dest = assetDir / src.filename();
        std::error_code ec;
        std::filesystem::copy_file(src, dest, std::filesystem::copy_options::update_existing, ec);
        if (ec) return Err(L, ("failed to copy asset"));

        lua_pushstring(L, (std::string(("rbxasset://")) + randomFolder + ("/") + src.filename().string()).c_str());
        return 1;
    }

    void register_lib(lua_State* L)
    {
        Environment->make_global(L, ("readfile"), Filesystem::readfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("writefile"), Filesystem::writefile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("appendfile"), Filesystem::appendfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("listfiles"), Filesystem::listfiles, Filesystem::funcnameinc++);
        Environment->make_global(L, ("isfile"), Filesystem::isfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("isfolder"), Filesystem::isfolder, Filesystem::funcnameinc++);
        Environment->make_global(L, ("makefolder"), Filesystem::makefolder, Filesystem::funcnameinc++);
        Environment->make_global(L, ("delfile"), Filesystem::delfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("delfolder"), Filesystem::delfolder, Filesystem::funcnameinc++);
        Environment->make_global(L, ("copyfile"), Filesystem::copyfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("movefile"), Filesystem::movefile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("renamefile"), Filesystem::renamefile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("getfilesize"), Filesystem::getfilesize, Filesystem::funcnameinc++);
        Environment->make_global(L, ("loadfile"), Filesystem::loadfile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("dofile"), Filesystem::dofile, Filesystem::funcnameinc++);
        Environment->make_global(L, ("getcustomasset"), Filesystem::getcustomasset, Filesystem::funcnameinc++);

        Environment->make_global(L, ("readfileasync"), Filesystem::readfileasync, Filesystem::funcnameinc++);
        Environment->make_global(L, ("writefileasync"), Filesystem::writefileasync, Filesystem::funcnameinc++);
        Environment->make_global(L, ("appendfileasync"), Filesystem::appendfileasync, Filesystem::funcnameinc++);
        Environment->make_global(L, ("loadfileasync"), Filesystem::loadfileasync, Filesystem::funcnameinc++);
    }
}
