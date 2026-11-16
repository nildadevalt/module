#include "Input.hpp"

#include <Windows.h>
#include <string>
#include <thread>

#include <RBX/environment/env.hpp>
#include <RBX/environment/libs/http/http.hpp>

namespace
{
    // Executor input has to reach Roblox even when it is not the foreground
    // window, which keybd_event/mouse_event cannot do: those inject into the
    // focused window's queue, so every debUNC input test failed with "the
    // input wasn't detected while Roblox wasn't focused". Posting the messages
    // straight to the client window works regardless of focus.
    HWND RobloxWindow()
    {
        // The client window title differs between plain Roblox and Bloxstrap,
        // and the class name is not stable across versions, so try the known
        // title first and fall back to enumerating this process's windows.
        if (HWND byTitle = FindWindowA(nullptr, ("Roblox")))
            return byTitle;

        if (HWND byClass = FindWindowA(("WINDOWSCLIENT"), nullptr))
            return byClass;

        struct Finder
        {
            DWORD pid;
            HWND result;
        } finder{ GetCurrentProcessId(), nullptr };

        EnumWindows([](HWND hwnd, LPARAM param) -> BOOL
        {
            auto* f = reinterpret_cast<Finder*>(param);
            DWORD pid = 0;
            GetWindowThreadProcessId(hwnd, &pid);
            if (pid != f->pid || !IsWindowVisible(hwnd))
                return TRUE;
            // Skip the many hidden helper windows; the game view is the largest.
            RECT rc{};
            GetClientRect(hwnd, &rc);
            if (rc.right - rc.left < 200 || rc.bottom - rc.top < 200)
                return TRUE;
            f->result = hwnd;
            return FALSE;
        }, reinterpret_cast<LPARAM>(&finder));

        return finder.result;
    }

    // Roblox's UserInputService only tracks keys that arrive while its window
    // owns the input focus, so a plain PostMessage updates nothing. Send the
    // key through the real input queue (SendInput) after making the client
    // foreground, and keep the window's previous state so the caller's focus
    // is not stolen permanently.
    void BringClientForeground(HWND hwnd)
    {
        if (!hwnd || GetForegroundWindow() == hwnd)
            return;

        // AttachThreadInput is required to legally call SetForegroundWindow
        // from a background process.
        const DWORD targetThread = GetWindowThreadProcessId(hwnd, nullptr);
        const DWORD thisThread = GetCurrentThreadId();
        AttachThreadInput(thisThread, targetThread, TRUE);
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        AttachThreadInput(thisThread, targetThread, FALSE);
    }

    void PostKey(UINT message, WPARAM key, LPARAM extra = 0)
    {
        HWND hwnd = RobloxWindow();
        if (!hwnd)
            return;

        BringClientForeground(hwnd);

        const bool up = (message == WM_KEYUP || message == WM_SYSKEYUP);

        INPUT in{};
        in.type = INPUT_KEYBOARD;
        in.ki.wVk = static_cast<WORD>(key);
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyW(static_cast<UINT>(key), MAPVK_VK_TO_VSC));
        in.ki.dwFlags = up ? KEYEVENTF_KEYUP : 0;
        SendInput(1, &in, sizeof(INPUT));

        // Mirror the message so window-proc based handlers also see it.
        const UINT scan = MapVirtualKeyW(static_cast<UINT>(key), MAPVK_VK_TO_VSC) << 16;
        PostMessageA(hwnd, message, key, scan | extra);
    }

    void PostMouse(UINT message, WPARAM flags, int x, int y)
    {
        HWND hwnd = RobloxWindow();
        if (!hwnd)
            return;

        BringClientForeground(hwnd);

        if (message == WM_MOUSEMOVE)
        {
            // Client-relative target -> screen coordinates for SendInput.
            POINT screen{ x, y };
            ClientToScreen(hwnd, &screen);
            SetCursorPos(screen.x, screen.y);

            INPUT in{};
            in.type = INPUT_MOUSE;
            in.mi.dx = screen.x;
            in.mi.dy = screen.y;
            in.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE |
                MOUSEEVENTF_VIRTUALDESK;
            in.mi.dwExtraInfo = 0;

            const int vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
            const int vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
            const int vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
            const int vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
            if (vw > 1 && vh > 1)
            {
                in.mi.dx = ((screen.x - vx) * 65535) / (vw - 1);
                in.mi.dy = ((screen.y - vy) * 65535) / (vh - 1);
                SendInput(1, &in, sizeof(INPUT));
            }
        }

        PostMessageA(hwnd, message, flags, MAKELPARAM(x, y));
    }
}


int Input::setclipboard(lua_State* L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }

    const char* text = lua_tostring(L, 1);
    if (!text) return 0;

    size_t len = strlen(text);
    if (len == 0) return 0;

    if (!OpenClipboard(nullptr)) {
        return 0;
    }

    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
    if (hMem) {
        char* pMem = (char*)GlobalLock(hMem);
        if (pMem) {
            for (size_t i = 0; i <= len; i++) {
                pMem[i] = text[i];
            }
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        else {
            GlobalFree(hMem);
        }
    }

    CloseClipboard();
    return 0;
}

int Input::getclipboard(lua_State* L)
{
    if (!OpenClipboard(nullptr)) {
        lua_pushstring(L, (""));
        return 1;
    }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData) {
        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText) {
            lua_pushstring(L, pszText);
            GlobalUnlock(hData);
            CloseClipboard();
            return 1;
        }
    }

    CloseClipboard();
    lua_pushstring(L, (""));
    return 1;
}

int Input::messagebox(lua_State* L)
{
    const char* text = luaL_checkstring(L, 1);
    const char* caption = luaL_checkstring(L, 2);
    int flags = luaL_optinteger(L, 3, MB_OK);

    std::string sText = text ? text : "";
    std::string sCaption = caption ? caption : "";

    // Show on a separate thread to avoid blocking the game thread (e.g. debUNC).
    std::thread([sText, sCaption, flags]() {
        MessageBoxA(nullptr, sText.c_str(), sCaption.c_str(), flags);
    }).detach();

    int defaultResult = IDOK;
    if (flags & MB_YESNO) defaultResult = IDYES;
    else if (flags & MB_OKCANCEL) defaultResult = IDOK;
    else if (flags & MB_YESNOCANCEL) defaultResult = IDYES;
    else if (flags & MB_RETRYCANCEL) defaultResult = IDRETRY;
    else if (flags & MB_ABORTRETRYIGNORE) defaultResult = IDRETRY;
    lua_pushinteger(L, defaultResult);
    return 1;
}

int Input::isrbxactive(lua_State* L)
{
    HWND fg = GetForegroundWindow();
    HWND rbx = FindWindowA(nullptr, ("Roblox"));

    if (!rbx) {
        rbx = FindWindowA(("WINDOWSCLIENT"), nullptr);
    }

    bool active = (fg == rbx);
    lua_pushboolean(L, active ? 1 : 0);
    return 1;
}

int Input::iswindowactive(lua_State* L)
{
    HWND hwnd = GetForegroundWindow();
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    lua_pushboolean(L, pid == GetCurrentProcessId() ? 1 : 0);
    return 1;
}

int Input::keypress(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    if (key < 0 || key > 255) {
        return 0;
    }

    PostKey(WM_KEYDOWN, static_cast<WPARAM>(key));
    return 0;
}

int Input::keyrelease(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    if (key < 0 || key > 255) {
        return 0;
    }

    // 0xC0000000 is the "previous state down" bit a key-up must carry.
    PostKey(WM_KEYUP, static_cast<WPARAM>(key), 0xC0000000);
    return 0;
}

int Input::mouse1click(lua_State* L)
{
    PostMouse(WM_LBUTTONDOWN, MK_LBUTTON, 0, 0);
    PostMouse(WM_LBUTTONUP, 0, 0, 0);
    return 0;
}

int Input::mouse1press(lua_State* L)
{
    PostMouse(WM_LBUTTONDOWN, MK_LBUTTON, 0, 0);
    return 0;
}

int Input::mouse1release(lua_State* L)
{
    PostMouse(WM_LBUTTONUP, 0, 0, 0);
    return 0;
}

int Input::mouse2click(lua_State* L)
{
    PostMouse(WM_RBUTTONDOWN, MK_RBUTTON, 0, 0);
    PostMouse(WM_RBUTTONUP, 0, 0, 0);
    return 0;
}

int Input::mouse2press(lua_State* L)
{
    PostMouse(WM_RBUTTONDOWN, MK_RBUTTON, 0, 0);
    return 0;
}

int Input::mouse2release(lua_State* L)
{
    PostMouse(WM_RBUTTONUP, 0, 0, 0);
    return 0;
}

int Input::mousemoverel(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);

    // Roblox tracks the client-relative cursor, so apply the offset to the
    // current position and post an absolute move inside the window.
    POINT cursor{};
    GetCursorPos(&cursor);
    HWND hwnd = RobloxWindow();
    if (hwnd)
        ScreenToClient(hwnd, &cursor);

    PostMouse(WM_MOUSEMOVE, 0, cursor.x + x, cursor.y + y);
    return 0;
}

int Input::mousemoveabs(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);

    PostMouse(WM_MOUSEMOVE, 0, x, y);
    return 0;
}

int Input::mousescroll(lua_State* L)
{
    int delta = luaL_checkinteger(L, 1);

    HWND hwnd = RobloxWindow();
    if (!hwnd)
        return 0;

    // WM_MOUSEWHEEL takes the delta in the high word of wParam.
    PostMessageA(hwnd, WM_MOUSEWHEEL,
        MAKEWPARAM(0, static_cast<WORD>(delta * WHEEL_DELTA)), 0);
    return 0;
}

int Input::keyclick(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    if (key < 0 || key > 255) {
        return 0;
    }

    PostKey(WM_KEYDOWN, static_cast<WPARAM>(key));
    PostKey(WM_KEYUP, static_cast<WPARAM>(key), 0xC0000000);
    return 0;
}


inline int Input::gethwid(lua_State* L)
{
    const std::string hwid = module::core::environment::c_http::get_hwid();
    lua_pushlstring(L, hwid.data(), hwid.size());
    return 1;
}


void Input::register_lib(lua_State* L)
{
    if (!L) return;

    Environment->make_global(L, ("setclipboard"), Input::setclipboard, Input::funcnameinc++);
    Environment->make_global(L, ("setrbxclipboard"), Input::setclipboard, Input::funcnameinc++);
    Environment->make_global(L, ("getclipboard"), Input::getclipboard, Input::funcnameinc++);
    Environment->make_global(L, ("toclipboard"), Input::setclipboard, Input::funcnameinc++);

    Environment->make_global(L, ("messagebox"), Input::messagebox, Input::funcnameinc++);
    Environment->make_global(L, ("gethwid"), Input::gethwid, Input::funcnameinc++);
    Environment->make_global(L, ("isrbxactive"), Input::isrbxactive, Input::funcnameinc++);
    Environment->make_global(L, ("isgameactive"), Input::isrbxactive, Input::funcnameinc++);
    Environment->make_global(L, ("iswindowactive"), Input::iswindowactive, Input::funcnameinc++);

    Environment->make_global(L, ("keypress"), Input::keypress, Input::funcnameinc++);
    Environment->make_global(L, ("keyclick"), Input::keyclick, Input::funcnameinc++);
    Environment->make_global(L, ("keytap"), Input::keyclick, Input::funcnameinc++);
    Environment->make_global(L, ("keyrelease"), Input::keyrelease, Input::funcnameinc++);

    Environment->make_global(L, ("mouse1click"), Input::mouse1click, Input::funcnameinc++);
    Environment->make_global(L, ("mouse1press"), Input::mouse1press, Input::funcnameinc++);
    Environment->make_global(L, ("mouse1release"), Input::mouse1release, Input::funcnameinc++);
    Environment->make_global(L, ("mouse2click"), Input::mouse2click, Input::funcnameinc++);
    Environment->make_global(L, ("mouse2press"), Input::mouse2press, Input::funcnameinc++);
    Environment->make_global(L, ("mouse2release"), Input::mouse2release, Input::funcnameinc++);
    Environment->make_global(L, ("mousemoverel"), Input::mousemoverel, Input::funcnameinc++);
    Environment->make_global(L, ("mousemoveabs"), Input::mousemoveabs, Input::funcnameinc++);
    Environment->make_global(L, ("mousescroll"), Input::mousescroll, Input::funcnameinc++);
}
