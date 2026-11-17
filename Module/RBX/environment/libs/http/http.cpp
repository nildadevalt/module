#include "http.hpp"
#include "gzip.hpp"

// static data member declared in http.hpp has to live in exactly one TU,
// otherwise every reference from register_lib dangles at link time
int module::core::environment::c_http::funcnameinc = 0;

bool module::core::environment::c_http::is_url_blocked(const std::string& url)
{
    std::string lower = url;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const auto& domain : blocked_domains)
        if (lower.find(domain) != std::string::npos)
            return true;
    return false;
}

std::string module::core::environment::c_http::get_hwid()
{
    HW_PROFILE_INFO hw_info{};
    std::string clean;

    if (GetCurrentHwProfile(&hw_info))
    {
        std::wstring w_guid(hw_info.szHwProfileGuid);
        std::string guid(w_guid.begin(), w_guid.end());
        for (char c : guid)
        {
            if (c != '{' && c != '}' && c != '-')
                clean += c;
        }
    }

    if (clean.empty())
        clean = ("fallback");

    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;

    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0)))
        return clean;

    DWORD hash_length = 0;
    DWORD obj_length = 0;
    DWORD bytes_done = 0;

    BCryptGetProperty(algorithm, BCRYPT_HASH_LENGTH,
        reinterpret_cast<PUCHAR>(&hash_length), sizeof(hash_length), &bytes_done, 0);
    BCryptGetProperty(algorithm, BCRYPT_OBJECT_LENGTH,
        reinterpret_cast<PUCHAR>(&obj_length), sizeof(obj_length), &bytes_done, 0);

    std::vector<uint8_t> hash_obj(obj_length);
    std::vector<uint8_t> digest(hash_length);

    if (!BCRYPT_SUCCESS(BCryptCreateHash(algorithm, &hash,
        hash_obj.data(), obj_length, nullptr, 0, 0)))
    {
        BCryptCloseAlgorithmProvider(algorithm, 0);
        return clean;
    }

    BCryptHashData(hash,
        reinterpret_cast<PUCHAR>(clean.data()),
        static_cast<ULONG>(clean.size()), 0);

    char computer_name[MAX_COMPUTERNAME_LENGTH + 1]{};
    DWORD name_len = sizeof(computer_name);
    if (GetComputerNameA(computer_name, &name_len))
        BCryptHashData(hash,
            reinterpret_cast<PUCHAR>(computer_name),
            name_len, 0);

    DWORD volume_serial = 0;
    if (GetVolumeInformationA(("C:\\"), nullptr, 0,
        &volume_serial, nullptr, nullptr, nullptr, 0))
        BCryptHashData(hash,
            reinterpret_cast<PUCHAR>(&volume_serial),
            sizeof(volume_serial), 0);

    BCryptFinishHash(hash, digest.data(), hash_length, 0);
    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(algorithm, 0);

    std::string result(hash_length * 2, '\0');
    for (size_t i = 0; i < hash_length; i++)
        std::snprintf(&result[i * 2], 3, ("%02x"), digest[i]);

    return result;
}

module::core::environment::parsed_url_t module::core::environment::c_http::parse_url(const std::string& url)
{
    parsed_url_t p{};

    size_t scheme_end = url.find(("://"));
    if (scheme_end == std::string::npos)
        return p;

    p.scheme = url.substr(0, scheme_end);
    std::transform(p.scheme.begin(), p.scheme.end(), p.scheme.begin(), ::tolower);

    size_t host_start = scheme_end + 3;
    size_t path_start = url.find('/', host_start);
    std::string host_port;

    if (path_start == std::string::npos)
    {
        host_port = url.substr(host_start);
        p.path = ("/");
    }
    else
    {
        host_port = url.substr(host_start, path_start - host_start);
        p.path = url.substr(path_start);
    }

    size_t colon = host_port.find(':');
    if (colon != std::string::npos)
    {
        p.host = host_port.substr(0, colon);
        p.port = std::stoi(host_port.substr(colon + 1));
    }
    else
    {
        p.host = host_port;
        p.port = (p.scheme == ("https")) ? 443 : 80;
    }

    return p;
}

module::core::environment::http_result_t module::core::environment::c_http::do_request(
    request_method_t method, const std::string& url,
    const httplib::Headers& headers, const std::string& body, const std::string& content_type)
{
    http_result_t result{};

    parsed_url_t p = parse_url(url);
    if (p.host.empty())
    {
        result.error = ("invalid url");
        return result;
    }

    std::string scheme_host = p.scheme + ("://") + p.host + (":") + std::to_string(p.port);
    std::string ct = content_type.empty() ? ("application/octet-stream") : content_type;

    try
    {
        httplib::Client cli(scheme_host);
        cli.set_follow_location(true);
        cli.set_connection_timeout(30, 0);
        cli.set_read_timeout(30, 0);
        cli.set_write_timeout(30, 0);
        // httplib is built without zlib, so leaving its decompressor on makes a
        // gzip response fail during the read ("Failed to read connection").
        // Read the body verbatim and let gzip.cpp decode it below.
        cli.set_decompress(false);

        if (p.scheme == ("https"))
            cli.enable_server_certificate_verification(false);

        httplib::Result res;

        switch (method)
        {
        case H_GET:     res = cli.Get(p.path, headers); break;
        case H_HEAD:    res = cli.Head(p.path, headers); break;
        case H_POST:    res = cli.Post(p.path, headers, body, ct.c_str()); break;
        case H_PUT:     res = cli.Put(p.path, headers, body, ct.c_str()); break;
        case H_DELETE:  res = cli.Delete(p.path, headers, body, ct.c_str()); break;
        case H_OPTIONS: res = cli.Options(p.path, headers); break;
        case H_PATCH:   res = cli.Patch(p.path, headers, body, ct.c_str()); break;
        default:
            result.error = ("invalid request method");
            return result;
        }

        if (!res)
        {
            result.error = ("request failed: ") + httplib::to_string(res.error());
            return result;
        }

        result.success = true;
        result.status_code = res->status;
        result.body = res->body;
        result.headers = res->headers;

        // Servers may gzip the body; decode it so scripts see plain text.
        const bool gzipEncoded = res->get_header_value("Content-Encoding") == "gzip";
        if (gzipEncoded || Gzip::LooksGzipped(result.body))
        {
            std::string decoded;
            if (Gzip::Decompress(result.body, decoded))
                result.body = std::move(decoded);
        }
    }
    catch (const std::exception& ex)
    {
        result.error = std::string(("request failed: ")) + ex.what();
    }
    catch (...)
    {
        result.error = ("request failed: unknown exception");
    }

    return result;
}

std::string module::core::environment::c_http::get_status_phrase(int code)
{
    switch (code)
    {
    case 100: return ("Continue");
    case 101: return ("Switching Protocols");
    case 102: return ("Processing");
    case 103: return ("Early Hints");
    case 200: return ("OK");
    case 201: return ("Created");
    case 202: return ("Accepted");
    case 203: return ("Non-Authoritative Information");
    case 204: return ("No Content");
    case 205: return ("Reset Content");
    case 206: return ("Partial Content");
    case 207: return ("Multi-Status");
    case 208: return ("Already Reported");
    case 226: return ("IM Used");
    case 300: return ("Multiple Choices");
    case 301: return ("Moved Permanently");
    case 302: return ("Found");
    case 303: return ("See Other");
    case 304: return ("Not Modified");
    case 305: return ("Use Proxy");
    case 307: return ("Temporary Redirect");
    case 308: return ("Permanent Redirect");
    case 400: return ("Bad Request");
    case 401: return ("Unauthorized");
    case 402: return ("Payment Required");
    case 403: return ("Forbidden");
    case 404: return ("Not Found");
    case 405: return ("Method Not Allowed");
    case 406: return ("Not Acceptable");
    case 407: return ("Proxy Authentication Required");
    case 408: return ("Request Timeout");
    case 409: return ("Conflict");
    case 410: return ("Gone");
    case 411: return ("Length Required");
    case 412: return ("Precondition Failed");
    case 413: return ("Payload Too Large");
    case 414: return ("URI Too Long");
    case 415: return ("Unsupported Media Type");
    case 416: return ("Range Not Satisfiable");
    case 417: return ("Expectation Failed");
    case 418: return ("I'm a teapot");
    case 422: return ("Unprocessable Entity");
    case 423: return ("Locked");
    case 424: return ("Failed Dependency");
    case 426: return ("Upgrade Required");
    case 428: return ("Precondition Required");
    case 429: return ("Too Many Requests");
    case 431: return ("Request Header Fields Too Large");
    case 451: return ("Unavailable For Legal Reasons");
    case 500: return ("Internal Server Error");
    case 501: return ("Not Implemented");
    case 502: return ("Bad Gateway");
    case 503: return ("Service Unavailable");
    case 504: return ("Gateway Time-out");
    case 505: return ("HTTP Version Not Supported");
    case 506: return ("Variant Also Negotiates");
    case 507: return ("Insufficient Storage");
    case 508: return ("Loop Detected");
    case 510: return ("Not Extended");
    case 511: return ("Network Authentication Required");
    default:  return std::string();
    }
}

static void read_game_ids(lua_State* L, std::string& job_id, std::string& place_id)
{
    lua_getglobal(L, ("game"));
    if (!lua_isnil(L, -1) && (lua_isuserdata(L, -1) || lua_istable(L, -1)))
    {
        lua_getfield(L, -1, ("JobId"));
        if (lua_isstring(L, -1))
            job_id = lua_tostring(L, -1);
        lua_pop(L, 1);

        if (job_id.empty())
        {
            lua_getfield(L, -1, ("GameId"));
            if (lua_isnumber(L, -1))
                job_id = std::to_string(static_cast<uint64_t>(lua_tonumber(L, -1)));
            else if (lua_isstring(L, -1))
                job_id = lua_tostring(L, -1);
            lua_pop(L, 1);
        }

        lua_getfield(L, -1, ("PlaceId"));
        if (lua_isnumber(L, -1))
            place_id = std::to_string(static_cast<uint64_t>(lua_tonumber(L, -1)));
        else if (lua_isstring(L, -1))
            place_id = lua_tostring(L, -1);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

inline int module::core::environment::c_http::http_get(lua_State* L)
{
    std::string url;

    if (!lua_isstring(L, 1))
    {
        luaL_checkstring(L, 2);
        url = lua_tostring(L, 2);
    }
    else
    {
        url = lua_tostring(L, 1);
    }

    if (url.find(("http://")) != 0 && url.find(("https://")) != 0)
    {
        luaL_argerror(L, 2, ("invalid protocol (expected 'http' or 'https')"));
        return 0;
    }

    if (is_url_blocked(url))
    {
        luaL_error(L, ("url is blocked"));
        return 0;
    }

    std::string game_id;
    std::string place_id;
    read_game_ids(L, game_id, place_id);

    std::string hwid = get_hwid();

    using Json = nlohmann::json;
    httplib::Headers headers;
    Json session_id_json;

    session_id_json[("GameId")] = game_id;
    session_id_json[("PlaceId")] = place_id;

    headers.insert({ ("User-Agent"), ("Roblox/WinInet") });
    headers.insert({ ("Roblox-Session-Id"), session_id_json.dump() });
    headers.insert({ ("Roblox-Place-Id"), place_id });
    headers.insert({ ("Roblox-Game-Id"), game_id });
    headers.insert({ ("Exploit-Identifier"), ("Paradox") });
    headers.insert({ ("Exploit-Guid"), hwid });
    headers.insert({ ("Fingerprint"), hwid });
    headers.insert({ ("Paradox-Fingerprint"), hwid });
    headers.insert({ ("Paradox-User-Identifier"), hwid });
    headers.insert({ ("Accept"), ("*/*") });

    return Yielding::YieldExecution(L, [url, headers]() -> std::function<int(lua_State*)>
        {
            LOGF("HTTP", "GET started");
            http_result_t result = do_request(H_GET, url, headers, "");
            LOGF("HTTP", "GET finished: status=%d bytes=%zu error=%s", result.status_code, result.body.size(), result.error.c_str());

            return [result](lua_State* L) -> int
                {
                    // A failed request must not look like a successful body. Raising
                    // from this resume callback surfaces as "resume callback raised a
                    // native exception", so report nil plus the reason instead.
                    if (!result.success)
                    {
                        lua_pushnil(L);
                        lua_pushstring(L, result.error.empty() ? ("request failed") : result.error.c_str());
                        return 2;
                    }

                    if (result.status_code != 200)
                    {
                        lua_pushnil(L);
                        lua_pushfstring(L, ("HTTP %d"), result.status_code);
                        return 2;
                    }

                    lua_pushlstring(L, result.body.data(), result.body.size());
                    return 1;
                };
        });
}

inline int module::core::environment::c_http::request(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, ("Url"));
    if (!lua_isstring(L, -1))
        luaL_error(L, ("missing or invalid 'Url'"));

    std::string url = lua_tostring(L, -1);
    lua_pop(L, 1);

    if (url.find(("http://")) != 0 && url.find(("https://")) != 0)
    {
        luaL_error(L, ("invalid protocol (expected 'http' or 'https')"));
        return 0;
    }

    if (is_url_blocked(url))
    {
        luaL_error(L, ("url is blocked"));
        return 0;
    }

    auto method = H_GET;
    lua_getfield(L, 1, ("Method"));
    if (lua_isstring(L, -1))
    {
        std::string method_str = lua_tostring(L, -1);
        std::transform(method_str.begin(), method_str.end(), method_str.begin(), ::tolower);
        if (!request_method_map.count(method_str))
            luaL_error(L, ("invalid request method: '%s'"), method_str.c_str());

        method = request_method_map[method_str];
    }
    lua_pop(L, 1);

    httplib::Headers headers;
    lua_getfield(L, 1, ("Headers"));
    if (lua_istable(L, -1))
    {
        lua_pushnil(L);
        while (lua_next(L, -2))
        {
            if (!lua_isstring(L, -2) || !lua_isstring(L, -1))
                luaL_error(L, ("'Headers' must have string keys/values"));

            std::string key = lua_tostring(L, -2);
            if (_stricmp(key.c_str(), ("Content-Length")) == 0)
                luaL_error(L, ("'Content-Length' cannot be overwritten"));

            headers.insert({ key, lua_tostring(L, -1) });
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    std::string cookie_header;
    lua_getfield(L, 1, ("Cookies"));
    if (lua_istable(L, -1))
    {
        lua_pushnil(L);
        while (lua_next(L, -2))
        {
            if (!lua_isstring(L, -2) || !lua_isstring(L, -1))
                luaL_error(L, ("'Cookies' must have string keys/values"));

            if (!cookie_header.empty()) cookie_header += ("; ");
            cookie_header += lua_tostring(L, -2);
            cookie_header += ("=");
            cookie_header += lua_tostring(L, -1);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    if (!cookie_header.empty())
        headers.insert({ ("Cookie"), cookie_header });

    std::string body;
    lua_getfield(L, 1, ("Body"));
    if (lua_isstring(L, -1))
    {
        if (method == H_GET || method == H_HEAD)
            luaL_error(L, ("'Body' is not allowed for GET/HEAD requests"));

        body = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    std::string game_id;
    std::string place_id;
    read_game_ids(L, game_id, place_id);

    std::string hwid = get_hwid();

    nlohmann::json session_id_json;
    session_id_json[("GameId")] = game_id;
    session_id_json[("PlaceId")] = place_id;

    auto has_header = [&](const std::string& key) -> bool
        {
            for (auto& h : headers)
                if (_stricmp(h.first.c_str(), key.c_str()) == 0)
                    return true;
            return false;
        };

    if (!has_header(("User-Agent")))
        headers.insert({ ("User-Agent"), ("Paradox/1.0") });
    if (!has_header(("Roblox-Session-Id")))
        headers.insert({ ("Roblox-Session-Id"), session_id_json.dump() });
    if (!has_header(("Roblox-Place-Id")))
        headers.insert({ ("Roblox-Place-Id"), place_id });
    if (!has_header(("Roblox-Game-Id")))
        headers.insert({ ("Roblox-Game-Id"), game_id });
    if (!has_header(("Exploit-Identifier")))
        headers.insert({ ("Exploit-Identifier"), ("Paradox") });
    if (!has_header(("Exploit-Guid")))
        headers.insert({ ("Exploit-Guid"), hwid });
    if (!has_header(("Fingerprint")))
        headers.insert({ ("Fingerprint"), hwid });
    if (!has_header(("Paradox-Fingerprint")))
        headers.insert({ ("Paradox-Fingerprint"), hwid });
    if (!has_header(("Paradox-User-Identifier")))
        headers.insert({ ("Paradox-User-Identifier"), hwid });
    if (!has_header(("Accept")))
        headers.insert({ ("Accept"), ("*/*") });

    std::string content_type;
    for (auto it = headers.begin(); it != headers.end(); ++it)
    {
        if (_stricmp(it->first.c_str(), "Content-Type") == 0)
        {
            content_type = it->second;
            headers.erase(it);
            break;
        }
    }

    return Yielding::YieldExecution(L, [=]() -> std::function<int(lua_State*)>
        {
            http_result_t response = do_request(method, url, headers, body, content_type);

            return [response](lua_State* L) -> int
                {
                    if (!response.success)
                    {
                        lua_pushstring(L, (("request failed: ") + response.error).c_str());
                        return 1;
                    }

                    lua_newtable(L);
                    lua_pushboolean(L, response.status_code >= 200 && response.status_code < 300);
                    lua_setfield(L, -2, ("Success"));

                    lua_pushinteger(L, response.status_code);
                    lua_setfield(L, -2, ("StatusCode"));

                    std::string phrase = get_status_phrase(response.status_code);
                    lua_pushstring(L, phrase.c_str());
                    lua_setfield(L, -2, ("StatusMessage"));

                    lua_newtable(L);
                    for (auto& header : response.headers)
                    {
                        lua_pushstring(L, header.first.c_str());
                        lua_pushstring(L, header.second.c_str());
                        lua_settable(L, -3);
                    }
                    lua_setfield(L, -2, ("Headers"));

                    lua_newtable(L);
                    auto set_cookie_range = response.headers.equal_range(("Set-Cookie"));
                    for (auto it = set_cookie_range.first; it != set_cookie_range.second; ++it)
                    {
                        const std::string& cookie = it->second;
                        size_t eq = cookie.find('=');
                        size_t sc = cookie.find(';');
                        if (eq == std::string::npos) continue;
                        std::string key = cookie.substr(0, eq);
                        std::string val = cookie.substr(eq + 1, (sc == std::string::npos ? cookie.size() : sc) - eq - 1);
                        lua_pushstring(L, key.c_str());
                        lua_pushstring(L, val.c_str());
                        lua_settable(L, -3);
                    }
                    lua_setfield(L, -2, ("Cookies"));

                    lua_pushlstring(L, response.body.data(), response.body.size());
                    lua_setfield(L, -2, ("Body"));

                    return 1;
                };
        });
}

inline int module::core::environment::c_http::http_post(lua_State* L)
{
    std::string url;
    std::string body;
    std::string content_type = ("application/json");

    int arg_offset = 0;
    if (!lua_isstring(L, 1))
    {
        arg_offset = 1;
        luaL_checkstring(L, 2);
    }

    url = lua_tostring(L, 1 + arg_offset);

    if (lua_isstring(L, 2 + arg_offset))
        body = lua_tostring(L, 2 + arg_offset);

    if (lua_isstring(L, 3 + arg_offset))
        content_type = lua_tostring(L, 3 + arg_offset);
    else if (lua_isnumber(L, 3 + arg_offset))
    {
        int ct = static_cast<int>(lua_tointeger(L, 3 + arg_offset));
        switch (ct)
        {
        case 0: content_type = ("application/json"); break;
        case 1: content_type = ("application/xml"); break;
        case 2: content_type = ("application/x-www-form-urlencoded"); break;
        case 3: content_type = ("text/plain"); break;
        case 4: content_type = ("text/xml"); break;
        default: content_type = ("application/json"); break;
        }
    }

    if (url.find(("http://")) != 0 && url.find(("https://")) != 0)
    {
        luaL_argerror(L, 1 + arg_offset, ("invalid protocol (expected 'http' or 'https')"));
        return 0;
    }

    if (is_url_blocked(url))
    {
        luaL_error(L, ("url is blocked"));
        return 0;
    }

    std::string game_id;
    std::string place_id;
    read_game_ids(L, game_id, place_id);

    std::string hwid = get_hwid();

    using Json = nlohmann::json;
    httplib::Headers headers;
    Json session_id_json;

    session_id_json[("GameId")] = game_id;
    session_id_json[("PlaceId")] = place_id;

    headers.insert({ ("User-Agent"), ("Roblox/WinInet") });
    headers.insert({ ("Roblox-Session-Id"), session_id_json.dump() });
    headers.insert({ ("Roblox-Place-Id"), place_id });
    headers.insert({ ("Roblox-Game-Id"), game_id });
    headers.insert({ ("Exploit-Identifier"), ("Paradox") });
    headers.insert({ ("Exploit-Guid"), hwid });
    headers.insert({ ("Fingerprint"), hwid });
    headers.insert({ ("Paradox-Fingerprint"), hwid });
    headers.insert({ ("Paradox-User-Identifier"), hwid });
    headers.insert({ ("Accept"), ("*/*") });

    return Yielding::YieldExecution(L, [url, body, headers, content_type]() -> std::function<int(lua_State*)>
        {
            http_result_t result = do_request(H_POST, url, headers, body, content_type);

            return [result](lua_State* L) -> int
                {
                    if (!result.success)
                    {
                        lua_pushstring(L, (("http_post failed: ") + result.error).c_str());
                        return 1;
                    }

                    if (result.status_code < 200 || result.status_code >= 300)
                    {
                        std::stringstream err;
                        err << ("http_post returned status: ") << result.status_code;
                        lua_pushstring(L, err.str().c_str());
                        return 1;
                    }

                    lua_pushlstring(L, result.body.data(), result.body.size());
                    return 1;
                };
        });
}

void module::core::environment::c_http::register_lib(lua_State* L)
{
    funcnameinc = 0;

    Environment->make_global(L, ("HttpGet"), c_http::http_get, funcnameinc++);
    Environment->make_global(L, ("HttpPost"), c_http::http_post, funcnameinc++);
    Environment->make_global(L, ("request"), c_http::request, funcnameinc++);

    Environment->make_aliases(L, c_http::http_get, funcnameinc++, { ("httpget"), ("HttpGetAsync"), ("httpgetasync"), ("syn_httpget"), ("gethttp"), ("httpGet") });
    Environment->make_aliases(L, c_http::http_post, funcnameinc++, { ("httppost"), ("HttpPostAsync"), ("httppostasync"), ("syn_httppost"), ("posthttp"), ("httpPost") });
    Environment->make_aliases(L, c_http::request, funcnameinc++, { ("http_request"), ("httprequest"), ("syn_request"), ("krnl_request"), ("webrequest") });

    lua_newtable(L);
    Environment->make_table_function(L, ("request"), c_http::request, funcnameinc++);
    Environment->make_table_function(L, ("Request"), c_http::request, funcnameinc++);
    Environment->make_table_function(L, ("http_request"), c_http::request, funcnameinc++);
    lua_setglobal(L, ("http"));
}
