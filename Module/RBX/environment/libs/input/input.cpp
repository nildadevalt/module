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
        // Pick the largest visible top-level window owned by this process: the
        // game view. The old code trusted the first window whose title was
        // "Roblox", but the client also creates small hidden windows with that
        // title, and mapping client coordinates through one of those put every
        // synthetic cursor move and click outside the real viewport.
        struct Finder
        {
            DWORD pid;
            HWND result;
            long bestArea;
        } finder{ GetCurrentProcessId(), nullptr, 0 };

        EnumWindows([](HWND hwnd, LPARAM param) -> BOOL
        {
            auto* f = reinterpret_cast<Finder*>(param);
            DWORD pid = 0;
            GetWindowThreadProcessId(hwnd, &pid);
            if (pid != f->pid || !IsWindowVisible(hwnd))
                return TRUE;

            RECT rc{};
            if (!GetClientRect(hwnd, &rc))
                return TRUE;

            const long area = (rc.right - rc.left) * (rc.bottom - rc.top);
            if (rc.right - rc.left < 200 || rc.bottom - rc.top < 200 || area <= f->bestArea)
                return TRUE;

            f->result = hwnd;
            f->bestArea = area;
            return TRUE;
        }, reinterpret_cast<LPARAM>(&finder));

        if (finder.result)
            return finder.result;

        if (HWND byClass = FindWindowA(("WINDOWSCLIENT"), nullptr))
            return byClass;

        return FindWindowA(nullptr, ("Roblox"));
    }

    // Roblox's UserInputService only tracks keys that arrive while its window
    // owns the input focus, so a plain PostMessage updates nothing. Send the
    // key through the real input queue (SendInput) after making the client
    // foreground, and keep the window's previous state so the caller's focus
    // is not stolen permanently.
    // Generic "become the foreground window" helper: attaching our input queue
    // to the thread that currently owns the foreground window is what makes
    // SetForegroundWindow legal for a background process.
    void ForceForeground(HWND hwnd)
    {
        if (!hwnd || GetForegroundWindow() == hwnd)
            return;

        // SetForegroundWindow is refused for a background process unless its
        // input queue is attached to the thread that currently owns the
        // foreground window, so attach to *that* thread before asking (the
        // previous code attached to the target thread, which never grants the
        // permission, so focus stealing silently failed and every synthetic
        // click and key landed in whatever app the user had in front).
        const DWORD thisThread = GetCurrentThreadId();
        HWND fg = GetForegroundWindow();
        const DWORD fgThread = fg ? GetWindowThreadProcessId(fg, nullptr) : 0;
        const DWORD targetThread = GetWindowThreadProcessId(hwnd, nullptr);

        const bool attachFg = fgThread && fgThread != thisThread &&
            AttachThreadInput(thisThread, fgThread, TRUE);
        const bool attachTarget = targetThread && targetThread != thisThread &&
            targetThread != fgThread && AttachThreadInput(thisThread, targetThread, TRUE);

        SetForegroundWindow(hwnd);
        BringWindowToTop(hwnd);
        SetFocus(hwnd);

        if (attachTarget)
            AttachThreadInput(thisThread, targetThread, FALSE);
        if (attachFg)
            AttachThreadInput(thisThread, fgThread, FALSE);
    }

    void BringClientForeground(HWND hwnd)
    {
        if (!hwnd || GetForegroundWindow() == hwnd)
            return;
        ForceForeground(hwnd);
    }

    // Diagnostics / shim helper: try to make the client the foreground window
    // and report whether the attempt succeeded.
    bool FocusClient()
    {
        HWND hwnd = RobloxWindow();
        if (!hwnd)
            return false;
        BringClientForeground(hwnd);
        return GetForegroundWindow() == hwnd;
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

    // Synthetic clicks only register when the cursor actually sits inside the
    // game's client area, so park it in the middle of the viewport first when
    // it is somewhere else (other monitor, over another window, off-screen).
    void EnsureCursorInClient(HWND hwnd)
    {
        RECT rc{};
        POINT p{};
        if (!GetClientRect(hwnd, &rc) || !GetCursorPos(&p))
            return;

        POINT client = p;
        ScreenToClient(hwnd, &client);
        if (client.x >= 0 && client.y >= 0 && client.x < rc.right && client.y < rc.bottom)
            return;

        client.x = (rc.right - rc.left) / 2;
        client.y = (rc.bottom - rc.top) / 2;
        ClientToScreen(hwnd, &client);
        SetCursorPos(client.x, client.y);
    }

    void PostMouse(UINT message, WPARAM flags, int x, int y)
    {
        HWND hwnd = RobloxWindow();
        if (!hwnd)
            return;

        BringClientForeground(hwnd);

        if (message == WM_MOUSEMOVE)
        {
            // Roblox samples the real cursor, so SetCursorPos alone is what the
            // game sees. The previous SendInput(MOUSEEVENTF_ABSOLUTE) pair used
            // the already-converted screen point as if it were a 0..65535
            // fraction, which threw the cursor to a corner and pinned it there:
            // every mouse test then read one frozen GetMouseLocation value.
            RECT rc{};
            if (GetClientRect(hwnd, &rc) && rc.right > 0 && rc.bottom > 0)
            {
                if (x < 0) x = 0;
                if (y < 0) y = 0;
                if (x > rc.right - 1) x = rc.right - 1;
                if (y > rc.bottom - 1) y = rc.bottom - 1;
            }

            POINT screen{ x, y };
            ClientToScreen(hwnd, &screen);
            SetCursorPos(screen.x, screen.y);
            return;
        }

        EnsureCursorInClient(hwnd);
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

int Input::inputdiag(lua_State* L)
{
    HWND hwnd = RobloxWindow();
    RECT rc{};
    POINT origin{ 0, 0 }, cursor{};
    if (hwnd)
    {
        GetClientRect(hwnd, &rc);
        ClientToScreen(hwnd, &origin);
    }
    GetCursorPos(&cursor);

    HWND fg = GetForegroundWindow();
    char fgTitle[128]{};
    if (fg)
        GetWindowTextA(fg, fgTitle, sizeof(fgTitle) - 1);

    lua_newtable(L);
    lua_pushboolean(L, hwnd != nullptr);              lua_setfield(L, -2, "found");
    lua_pushboolean(L, hwnd && fg == hwnd);           lua_setfield(L, -2, "foreground");
    lua_pushstring(L, fgTitle);                       lua_setfield(L, -2, "fg_title");
    lua_pushinteger(L, rc.right - rc.left);           lua_setfield(L, -2, "client_w");
    lua_pushinteger(L, rc.bottom - rc.top);           lua_setfield(L, -2, "client_h");
    lua_pushinteger(L, origin.x);                     lua_setfield(L, -2, "origin_x");
    lua_pushinteger(L, origin.y);                     lua_setfield(L, -2, "origin_y");
    lua_pushinteger(L, cursor.x);                     lua_setfield(L, -2, "cursor_x");
    lua_pushinteger(L, cursor.y);                     lua_setfield(L, -2, "cursor_y");
    return 1;
}

int Input::inputsinject(lua_State* L)
{
    HWND hwnd = RobloxWindow();
    if (!hwnd)
        return 0;

    // Force the client truly to the front, then report whether Windows agreed.
    ShowWindow(hwnd, SW_RESTORE);
    const DWORD targetThread = GetWindowThreadProcessId(hwnd, nullptr);
    const DWORD thisThread = GetCurrentThreadId();
    if (targetThread != thisThread)
        AttachThreadInput(thisThread, targetThread, TRUE);
    BringWindowToTop(hwnd);
    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);
    SetFocus(hwnd);
    if (targetThread != thisThread)
        AttachThreadInput(thisThread, targetThread, FALSE);

    lua_pushboolean(L, GetForegroundWindow() == hwnd);
    return 1;
}

int Input::mousescroll(lua_State* L)
{
    int delta = luaL_checkinteger(L, 1);

    HWND hwnd = RobloxWindow();
    if (!hwnd)
        return 0;

    BringClientForeground(hwnd);
    EnsureCursorInClient(hwnd);

    // A real wheel injection is what UserInputService reacts to; the window
    // message is only kept as a mirror for handlers that read wParam.
    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.mouseData = static_cast<DWORD>(delta * WHEEL_DELTA);
    in.mi.dwFlags = MOUSEEVENTF_WHEEL;
    SendInput(1, &in, sizeof(INPUT));

    // Mirror the message for handlers that read wParam: the injected wheel only
    // updates the input queue, and window-proc based code never sees it.
    POINT cursor{};
    GetCursorPos(&cursor);
    ScreenToClient(hwnd, &cursor);

    PostMessageA(hwnd, WM_MOUSEWHEEL,
        MAKEWPARAM(0, static_cast<WORD>(delta * WHEEL_DELTA)),
        MAKELPARAM(cursor.x, cursor.y));
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
    Environment->make_global(L, ("__inputdiag"), Input::inputdiag, Input::funcnameinc++);
    Environment->make_global(L, ("__inputsinject"), Input::inputsinject, Input::funcnameinc++);
    Environment->make_global(L, ("__client_focused"), +[](lua_State* L) -> int
    {
        // Read-only check: never steal focus. Grabbing focus makes the camera
        // lock the mouse, which then breaks every following input test.
        HWND hwnd = RobloxWindow();
        lua_pushboolean(L, hwnd && GetForegroundWindow() == hwnd);
        return 1;
    }, Input::funcnameinc++);

    Environment->make_global(L, ("__wheel_os"), +[](lua_State* L) -> int
    {
        // VirtualInputManager collapses every wheel notch to +1, so scrolling
        // down zoomed in exactly like scrolling up. A real Win32 wheel keeps
        // both sign and magnitude but is only delivered to the foreground
        // window, so borrow the foreground for the injection and hand it
        // straight back - leaving the game focused makes the camera lock the
        // mouse and breaks the input tests that follow.
        const int delta = static_cast<int>(luaL_checkinteger(L, 1));
        HWND hwnd = RobloxWindow();
        if (!hwnd)
        {
            lua_pushboolean(L, 0);
            return 1;
        }

        EnsureCursorInClient(hwnd);
        HWND previous = GetForegroundWindow();
        if (previous != hwnd)
            ForceForeground(hwnd);

        const bool injected = GetForegroundWindow() == hwnd;
        if (injected)
        {
            INPUT in{};
            in.type = INPUT_MOUSE;
            in.mi.mouseData = static_cast<DWORD>(delta * WHEEL_DELTA);
            in.mi.dwFlags = MOUSEEVENTF_WHEEL;
            SendInput(1, &in, sizeof(INPUT));
        }

        if (injected && previous && previous != hwnd)
            ForceForeground(previous);

        lua_pushboolean(L, injected);
        return 1;
    }, Input::funcnameinc++);

    Environment->make_global(L, ("__wheel_os_cycle"), +[](lua_State* L) -> int
    {
        // Roblox only feeds a wheel event into UserInputService when the window
        // owns the foreground, and VirtualInputManager flattens every notch to
        // +1 so scrolling down zoomed in. Borrow the foreground for a single
        // frame, inject a real wheel event that keeps the caller's sign, then
        // hand the foreground back before the next frame can lock the camera.
        const int delta = static_cast<int>(luaL_checkinteger(L, 1));

        HWND hwnd = RobloxWindow();
        if (!hwnd)
        {
            lua_pushboolean(L, 0);
            return 1;
        }

        EnsureCursorInClient(hwnd);

        HWND previous = GetForegroundWindow();
        if (previous != hwnd)
            ForceForeground(hwnd);

        if (GetForegroundWindow() != hwnd)
        {
            lua_pushboolean(L, 0);
            return 1;
        }

        INPUT in{};
        in.type = INPUT_MOUSE;
        in.mi.mouseData = static_cast<DWORD>(delta * WHEEL_DELTA);
        in.mi.dwFlags = MOUSEEVENTF_WHEEL;
        SendInput(1, &in, sizeof(INPUT));

        // Let the client pump translate the wheel into an InputObject before the
        // foreground is released; 90ms covers two frames at 60Hz.
        Sleep(90);

        if (previous && previous != hwnd)
            ForceForeground(previous);

        lua_pushboolean(L, 1);
        return 1;
    }, Input::funcnameinc++);

    Environment->make_global(L, ("__focus_client"), +[](lua_State* L) -> int
    {
        // Try to pull the client window to the front. Only when this succeeds is
        // SendInput routed at the game; otherwise the shim keeps using the
        // VirtualInputManager path, which works while unfocused.
        lua_pushboolean(L, FocusClient());
        return 1;
    }, Input::funcnameinc++);
    Environment->make_global(L, ("mousescroll"), Input::mousescroll, Input::funcnameinc++);
}