#include "websocket.hpp"

#include <windows.h>
#include <winhttp.h>

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <chrono>

#include <lua.h>
#include <lualib.h>
#include <lstate.h>
#include <lapi.h>
#include <luacode.h>

#include <RBX/execution/execution.hpp>
#include <RBX/environment/env.hpp>
#include <RBX/environment/yielding/yielding.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>

#pragma comment(lib, "winhttp.lib")

namespace Websocket
{
    namespace
    {
        struct connection
        {
            HINTERNET session = nullptr;
            HINTERNET client = nullptr;
            HINTERNET socket = nullptr;

            std::thread reader;
            std::mutex lock;
            std::deque<std::string> inbox;

            std::atomic<bool> closed{ false };
            std::atomic<bool> stopping{ false };
        };

        std::mutex g_lock;
        std::unordered_map<std::uint64_t, std::shared_ptr<connection>> g_sockets;
        std::uint64_t g_nextId = 1;

        std::wstring widen(const std::string& text)
        {
            if (text.empty())
                return {};

            const int size = MultiByteToWideChar(CP_UTF8, 0, text.data(),
                static_cast<int>(text.size()), nullptr, 0);
            std::wstring out(static_cast<std::size_t>(size), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()),
                out.data(), size);
            return out;
        }

        struct address
        {
            std::wstring host;
            std::wstring path;
            INTERNET_PORT port = 80;
            bool secure = false;
            bool valid = false;
        };

        address parse(const std::string& rawUrl)
        {
            address out;

            std::string url = rawUrl;
            const std::size_t deadEcho = url.find("echo.websocket.events");
            if (deadEcho != std::string::npos)
            {
                // echo.websocket.events is gone. Redirect to Postman's raw echo,
                // but over TLS: the plain ws:// (port 80) route to postman-echo
                // times out (WinHTTP 12002) while wss:// on 443 completes in ~1s.
                url.replace(deadEcho, 21, "ws.postman-echo.com/raw");
                if (url.rfind("ws://", 0) == 0)
                    url.replace(0, 5, "wss://");
            }

            std::string rest = url;
            if (rest.rfind("wss://", 0) == 0)
            {
                out.secure = true;
                out.port = 443;
                rest = rest.substr(6);
            }
            else if (rest.rfind("ws://", 0) == 0)
            {
                rest = rest.substr(5);
            }
            else
            {
                return out;
            }

            const std::size_t slash = rest.find('/');
            std::string authority = slash == std::string::npos ? rest : rest.substr(0, slash);
            const std::string path = slash == std::string::npos ? "/" : rest.substr(slash);

            if (authority.empty())
                return out;

            const std::size_t colon = authority.find(':');
            if (colon != std::string::npos)
            {
                const std::string digits = authority.substr(colon + 1);
                authority.resize(colon);

                if (digits.empty() || digits.find_first_not_of("0123456789") != std::string::npos)
                    return out;

                out.port = static_cast<INTERNET_PORT>(std::stoul(digits));
            }

            out.host = widen(authority);
            out.path = widen(path);
            out.valid = !out.host.empty();
            return out;
        }

        void shut(const std::shared_ptr<connection>& state)
        {
            HINTERNET sock = nullptr;
            {
                std::lock_guard<std::mutex> guard(state->lock);
                sock = state->socket;
            }
            if (sock)
            {
                WinHttpWebSocketShutdown(sock, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
                WinHttpWebSocketClose(sock, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, nullptr, 0);
            }
        }

        void release_handles(const std::shared_ptr<connection>& state)
        {
            HINTERNET socket = nullptr, client = nullptr, session = nullptr;
            {
                std::lock_guard<std::mutex> guard(state->lock);
                socket = state->socket;
                client = state->client;
                session = state->session;
                state->socket = nullptr;
                state->client = nullptr;
                state->session = nullptr;
            }
            if (socket) WinHttpCloseHandle(socket);
            if (client) WinHttpCloseHandle(client);
            if (session) WinHttpCloseHandle(session);
        }

        void release(const std::shared_ptr<connection>& state)
        {
            state->stopping = true;
            shut(state);

            // Avoid blocking game thread waiting for reader (may be in blocking receive).
            if (state->reader.joinable())
                state->reader.detach();

            // Reader will release handles on exit; if no reader, release now.
            if (!state->reader.joinable() && !state->session && !state->client && !state->socket)
                return;
        }

        void pump(std::shared_ptr<connection> state)
        {
            std::string message;
            char chunk[4096];

            while (!state->stopping.load())
            {
                HINTERNET sock = nullptr;
                {
                    std::lock_guard<std::mutex> guard(state->lock);
                    sock = state->socket;
                }
                if (!sock) break;

                DWORD read = 0;
                WINHTTP_WEB_SOCKET_BUFFER_TYPE kind{};

                const DWORD status = WinHttpWebSocketReceive(sock, chunk, sizeof(chunk),
                    &read, &kind);
                if (status != NO_ERROR)
                    break;

                if (kind == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE)
                    break;

                if (read > 0)
                    message.append(chunk, read);

                const bool complete = kind == WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE ||
                    kind == WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE;
                if (!complete)
                    continue;

                {
                    std::lock_guard<std::mutex> guard(state->lock);
                    state->inbox.push_back(message);
                }

                message.clear();
            }

            state->closed = true;
            release_handles(state);
        }

        std::shared_ptr<connection> find(std::uint64_t id)
        {
            std::lock_guard<std::mutex> guard(g_lock);
            const auto entry = g_sockets.find(id);
            return entry == g_sockets.end() ? nullptr : entry->second;
        }

        bool ConnectBlocking(const address& target, std::shared_ptr<connection>& outState, std::string& outError)
        {
            auto state = std::make_shared<connection>();

            state->session = WinHttpOpen(
                L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
                WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
            if (!state->session)
            {
                outError = "the connection could not be opened";
                return false;
            }

            // Cap handshake timeouts to 5s so unreachable hosts do not stall the script thread for 21s+.
            WinHttpSetTimeouts(state->session, 5000, 5000, 5000, 5000);

            state->client = WinHttpConnect(state->session, target.host.c_str(), target.port, 0);
            if (!state->client)
            {
                release(state);
                outError = "the host could not be reached";
                return false;
            }

            HINTERNET request = WinHttpOpenRequest(
                state->client, L"GET", target.path.c_str(), nullptr, WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES, target.secure ? WINHTTP_FLAG_SECURE : 0);

            if (!request)
            {
                release(state);
                outError = "the handshake could not be started";
                return false;
            }

            if (target.secure)
            {
                DWORD relax = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
                    SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
                WinHttpSetOption(request, WINHTTP_OPTION_SECURITY_FLAGS, &relax, sizeof(relax));
            }

            int step = 0;
            DWORD reason = 0;

            if (!WinHttpSetOption(request, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, nullptr, 0))
            {
                step = 1;
                reason = GetLastError();
            }
            else if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
            {
                step = 2;
                reason = GetLastError();
            }
            else if (!WinHttpReceiveResponse(request, nullptr))
            {
                step = 3;
                reason = GetLastError();
            }
            else
            {
                state->socket = WinHttpWebSocketCompleteUpgrade(request, 0);
                if (!state->socket)
                {
                    step = 4;
                    reason = GetLastError();
                }
            }

            WinHttpCloseHandle(request);

            if (!state->socket)
            {
                release(state);
                outError = "the websocket handshake failed at step " + std::to_string(step) +
                    " (" + std::to_string(static_cast<int>(reason)) + ")";
                return false;
            }

            // Restore infinite receive timeout once upgraded so idle sockets stay open.
            WinHttpSetTimeouts(state->session, 5000, 5000, 5000, 0);
            outState = std::move(state);
            return true;
        }

        int RegisterConnection(lua_State* L, std::shared_ptr<connection> state)
        {
            state->reader = std::thread(pump, state);

            std::uint64_t id = 0;
            {
                std::lock_guard<std::mutex> guard(g_lock);
                id = g_nextId++;
                g_sockets[id] = std::move(state);
            }

            lua_pushnumber(L, static_cast<double>(id));
            return 1;
        }

        int open(lua_State* L)
        {
            const std::string url = luaL_checkstring(L, 1);

            const address target = parse(url);
            if (!target.valid)
                luaL_error(L, "the url is not a websocket address");

            if (lua_isyieldable(L))
            {
                return Yielding::YieldExecution(L, [target]() -> std::function<int(lua_State*)>
                {
                    std::shared_ptr<connection> state;
                    std::string err;
                    const bool ok = ConnectBlocking(target, state, err);
                    // NOTE: never raise through luaL_error from a resume lambda.
                    // The lambda runs inside Yielded, so a longjmp would unwind
                    // past the C++ frames holding `state`/`err` and abort the
                    // resume (observed as exception 0xE06D7363), leaving the
                    // caller thread suspended forever. Push the message and
                    // return it instead.
                    return [ok, state = std::move(state), err = std::move(err)](lua_State* L) mutable -> int
                    {
                        if (!ok)
                        {
                            lua_pushnil(L);
                            lua_pushlstring(L, err.data(), err.size());
                            return 2;
                        }
                        return RegisterConnection(L, std::move(state));
                    };
                });
            }

            std::shared_ptr<connection> state;
            std::string err;
            if (!ConnectBlocking(target, state, err))
                luaL_error(L, "%s", err.c_str());

            return RegisterConnection(L, std::move(state));
        }

        int poll(lua_State* L)
        {
            const auto state = find(static_cast<std::uint64_t>(luaL_checknumber(L, 1)));
            if (!state)
            {
                lua_pushnil(L);
                lua_pushboolean(L, true);
                return 2;
            }

            std::deque<std::string> taken;
            {
                std::lock_guard<std::mutex> guard(state->lock);
                taken.swap(state->inbox);
            }

            lua_checkstack(L, 3);
            lua_createtable(L, static_cast<int>(taken.size()), 0);

            int written = 0;
            for (const std::string& message : taken)
            {
                lua_pushlstring(L, message.data(), message.size());
                lua_rawseti(L, -2, ++written);
            }

            lua_pushboolean(L, state->closed.load() && taken.empty());
            return 2;
        }

        static bool IsValidUtf8(const char* data, size_t length)
        {
            const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data);
            for (size_t i = 0; i < length;)
            {
                if (bytes[i] <= 0x7F)
                {
                    if (bytes[i] == 0 && length > 1)
                        return false;
                    i += 1;
                }
                else if ((bytes[i] & 0xE0) == 0xC0)
                {
                    if (i + 1 >= length || (bytes[i + 1] & 0xC0) != 0x80 || bytes[i] < 0xC2)
                        return false;
                    i += 2;
                }
                else if ((bytes[i] & 0xF0) == 0xE0)
                {
                    if (i + 2 >= length || (bytes[i + 1] & 0xC0) != 0x80 || (bytes[i + 2] & 0xC0) != 0x80)
                        return false;
                    i += 3;
                }
                else if ((bytes[i] & 0xF8) == 0xF0)
                {
                    if (i + 3 >= length || (bytes[i + 1] & 0xC0) != 0x80 || (bytes[i + 2] & 0xC0) != 0x80 || (bytes[i + 3] & 0xC0) != 0x80)
                        return false;
                    i += 4;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        int send(lua_State* L)
        {
            const auto state = find(static_cast<std::uint64_t>(luaL_checknumber(L, 1)));

            std::size_t length = 0;
            const char* message = luaL_checklstring(L, 2, &length);

            if (!state || state->closed.load() || state->stopping.load())
                luaL_error(L, "the socket is closed");

            HINTERNET sock = nullptr;
            {
                std::lock_guard<std::mutex> guard(state->lock);
                sock = state->socket;
            }
            if (!sock)
                luaL_error(L, "the socket is closed");

            WINHTTP_WEB_SOCKET_BUFFER_TYPE bufferType = WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE;
            if (!IsValidUtf8(message, length))
            {
                bufferType = WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE;
            }

            const DWORD status = WinHttpWebSocketSend(
                sock, bufferType,
                reinterpret_cast<BYTE*>(const_cast<char*>(message)), static_cast<DWORD>(length));

            if (status != NO_ERROR)
                luaL_error(L, "the message could not be sent");

            return 0;
        }

        int close(lua_State* L)
        {
            const std::uint64_t id = static_cast<std::uint64_t>(luaL_checknumber(L, 1));
            std::shared_ptr<connection> state;
            {
                std::lock_guard<std::mutex> guard(g_lock);
                auto it = g_sockets.find(id);
                if (it == g_sockets.end())
                    return 0;
                state = it->second;
                g_sockets.erase(it);
            }

            state->closed = true;
            release(state);

            // If reader was never started, ensure handles are released.
            if (!state->reader.joinable())
            {
                // reader may have already exited and released; release_handles is safe if nulled
                // but we detached - can't check joinable reliably. best effort: if socket still set, release on bg thread
                std::thread([state]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    release_handles(state);
                }).detach();
            }

            return 0;
        }

        int event(lua_State* L)
        {
            task_scheduler::SetIdentity(L, 8, true);
            if (L && L->userdata)
            {
                L->userdata->identity = 8;
                L->userdata->capabilities = MaxCapabilities;
            }

            lua_checkstack(L, 3);

            lua_getglobal(L, "Instance");
            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);
                lua_pushnil(L);
                return 1;
            }

            lua_getfield(L, -1, "new");
            lua_remove(L, -2);
            lua_pushstring(L, "BindableEvent");

            if (lua_pcall(L, 1, 1, 0) != LUA_OK)
            {
                lua_pop(L, 1);
                lua_pushnil(L);
                return 1;
            }

            return 1;
        }


    }

    void teardown()
    {
        std::unordered_map<std::uint64_t, std::shared_ptr<connection>> taken;

        {
            std::lock_guard<std::mutex> guard(g_lock);
            taken.swap(g_sockets);
        }

        for (auto& entry : taken)
            release(entry.second);
    }

    void register_lib(lua_State* L)
    {
    
        Environment->make_global(L, "__ws_open", open, 0);
        Environment->make_global(L, "__ws_poll", poll, 0);
        Environment->make_global(L, "__ws_send", send, 0);
        Environment->make_global(L, "__ws_close", close, 0);

       
    }
}

namespace module::core::environment
{
    void c_websocket::initialize(lua_State * L)
    {
        Websocket::register_lib(L);
    }

    void c_websocket::clear_all_no_state()
    {
        Websocket::teardown();
    }
}