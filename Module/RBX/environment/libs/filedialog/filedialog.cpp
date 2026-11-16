#include "FileDialog.hpp"

#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>
#include <RBX/environment/env.hpp>
#include <RBX/environment/yielding/yielding.hpp>

#include <shlobj.h>
#include <commdlg.h>
#include <objbase.h>

#include <string>
#include <vector>
#include <thread>


namespace FileDialog {

    int openfiledialog(lua_State* L)
    {
        std::string filter = lua_isstring(L, 1) ? lua_tostring(L, 1) : std::string("All Files\0*.*\0", 17);
        std::string title = lua_isstring(L, 2) ? lua_tostring(L, 2) : "Open File";

        auto work = [filter, title]() -> std::function<int(lua_State*)> {
            char filename[MAX_PATH] = {};
            OPENFILENAMEA ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = filter.c_str();
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = title.c_str();
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

            std::string result;
            bool ok = GetOpenFileNameA(&ofn) != FALSE;
            if (ok) result = filename;

            return [ok, result](lua_State* L) -> int {
                if (ok) lua_pushstring(L, result.c_str());
                else lua_pushnil(L);
                return 1;
            };
        };

        if (lua_isyieldable(L))
            return Yielding::YieldExecution(L, work);

        auto resume = work();
        return resume(L);
    }

    int openfilesdialog(lua_State* L)
    {
        std::string filter = lua_isstring(L, 1) ? lua_tostring(L, 1) : std::string("All Files\0*.*\0", 17);
        std::string title = lua_isstring(L, 2) ? lua_tostring(L, 2) : "Open Files";

        auto work = [filter, title]() -> std::function<int(lua_State*)> {
            std::vector<char> filenames(32768, 0);
            OPENFILENAMEA ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = filter.c_str();
            ofn.lpstrFile = filenames.data();
            ofn.nMaxFile = (DWORD)filenames.size();
            ofn.lpstrTitle = title.c_str();
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

            std::vector<std::string> results;
            if (GetOpenFileNameA(&ofn)) {
                char* dir = filenames.data();
                char* file = filenames.data() + strlen(filenames.data()) + 1;
                if (*file == '\0') {
                    results.push_back(dir);
                } else {
                    while (*file) {
                        char fullpath[MAX_PATH];
                        snprintf(fullpath, MAX_PATH, "%s\\%s", dir, file);
                        results.push_back(fullpath);
                        file += strlen(file) + 1;
                    }
                }
            }

            return [results](lua_State* L) -> int {
                lua_newtable(L);
                int idx = 1;
                for (auto& s : results) {
                    lua_pushstring(L, s.c_str());
                    lua_rawseti(L, -2, idx++);
                }
                return 1;
            };
        };

        if (lua_isyieldable(L))
            return Yielding::YieldExecution(L, work);

        auto resume = work();
        return resume(L);
    }

    int openfolderdialog(lua_State* L)
    {
        std::string title = lua_isstring(L, 1) ? lua_tostring(L, 1) : "Select Folder";

        auto work = [title]() -> std::function<int(lua_State*)> {
            char path[MAX_PATH] = {};
            BROWSEINFOA bi = {};
            bi.lpszTitle = title.c_str();
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

            LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
            std::string result;
            bool ok = false;
            if (pidl) {
                if (SHGetPathFromIDListA(pidl, path)) { result = path; ok = true; }
                CoTaskMemFree(pidl);
            }
            return [ok, result](lua_State* L) -> int {
                if (ok) lua_pushstring(L, result.c_str());
                else lua_pushnil(L);
                return 1;
            };
        };

        if (lua_isyieldable(L))
            return Yielding::YieldExecution(L, work);

        auto resume = work();
        return resume(L);
    }

    int savefiledialog(lua_State* L)
    {
        std::string filter = lua_isstring(L, 1) ? lua_tostring(L, 1) : std::string("All Files\0*.*\0", 17);
        std::string title = lua_isstring(L, 2) ? lua_tostring(L, 2) : "Save File";

        auto work = [filter, title]() -> std::function<int(lua_State*)> {
            char filename[MAX_PATH] = {};
            OPENFILENAMEA ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = filter.c_str();
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = title.c_str();
            ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

            std::string result;
            bool ok = GetSaveFileNameA(&ofn) != FALSE;
            if (ok) result = filename;

            return [ok, result](lua_State* L) -> int {
                if (ok) lua_pushstring(L, result.c_str());
                else lua_pushnil(L);
                return 1;
            };
        };

        if (lua_isyieldable(L))
            return Yielding::YieldExecution(L, work);

        auto resume = work();
        return resume(L);
    }

    void register_lib(lua_State* L)
    {
        Environment->make_global(L, ("openfiledialog"), openfiledialog, FileDialog::funcnameinc++);
        Environment->make_global(L, ("openfilesdialog"), openfilesdialog, FileDialog::funcnameinc++);
        Environment->make_global(L, ("openfolderdialog"), openfolderdialog, FileDialog::funcnameinc++);
        Environment->make_global(L, ("savefiledialog"), savefiledialog, FileDialog::funcnameinc++);
    }
}
