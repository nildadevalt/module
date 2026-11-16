#include "hooks.hpp"

#include <lapi.h>
#include <lgc.h>
#include <ldebug.h>
#include <Utilitys/Globals.hpp>

#include <RBX/environment/libs/http/http.hpp>
#include <RBX/environment/libs/miscellaneous/miscellaneous.hpp>
#include <RBX/environment/libs/oth/oth.hpp>
#include <RBX/environment/libs/closures/closures.hpp>
#include <RBX/environment/libs/reflection/reflection.hpp>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_set>

lua_CFunction original_index = nullptr;
lua_CFunction original_newindex = nullptr;
lua_CFunction original_namecall = nullptr;
static const char g_NativeInstanceIndexKey = 0;

static std::unordered_set<std::string> blocked_funcs = {
    "Load", "OpenScreenshotsFolder", "OpenVideosFolder", "AddCoreScriptLocal",
    "SaveScriptProfilingData", "GetRobuxBalance", "PerformPurchase", "PerformPurchaseV2",
    "PerformBulkPurchase", "PerformSubscriptionPurchase", "PerformSubscriptionPurchaseV2",
    "PerformCancelSubscription", "PromptPurchase", "PromptProductPurchase",
    "PromptGamePassPurchase", "PromptBundlePurchase", "PromptBulkPurchase",
    "PromptRobloxPurchase", "PromptThirdPartyPurchase", "PromptPremiumPurchase",
    "PromptSubscriptionPurchase", "PromptCancelSubscription", "PromptNativePurchase",
    "PromptNativePurchaseWithLocalPlayer", "PromptCollectiblesPurchase",
    "PrepareCollectiblesPurchase", "GetUserSubscriptionPaymentHistoryAsync",
    "GetUserSubscriptionDetailsInternalAsync", "GetUserSubscriptionStatusAsync",
    "ReportAbuse", "ReportAbuseV3", "TakeScreenshot", "ToggleRecording",
    "OpenBrowserWindow", "OpenNativeOverlay", "ExecuteJavaScript", "ReturnToJavaScript",
    "SendCommand", "EmitHybridEvent", "OpenWeChatAuthWindow", "BroadcastNotification",
    "OpenUrl", "DetectUrl", "RegisterLuaUrl", "StartLuaUrlDelivery", "StopLuaUrlDelivery",
    "SwitchToSettingsApp", "SupportsSwitchToSettingsApp", "GetLastLuaUrl",
    "GetAndClearLastPendingUrl", "IsUrlRegistered", "RequestInternal", "requestInternal",
    "RequestAsync", "GetAsync", "PostAsync", "GetAsyncFullUrl", "PostAsyncFullUrl",
    "RequestLimitedAsync", "HttpRequestAsync", "GetApiV1", "InvokeAsync",
    "RegisterOpenCloud", "CaptureScreenshot", "CreatePostAsync", "DeleteCapture",
    "DeleteCapturesAsync", "GetCaptureFilePathAsync", "GetCaptureUploadDataAsync",
    "GetCaptureSizeAsync", "GetCaptureStorageSizeAsync", "RetrieveCaptures",
    "SaveCaptureToExternalStorage", "SaveCapturesToExternalStorageAsync",
    "SaveScreenshotCapture", "PromptSaveCapturesToGallery", "PromptShareCapture",
    "PostToFeedAsync", "Call", "GetLast", "GetMessageId",
    "GetProtocolMethodRequestMessageId", "GetProtocolMethodResponseMessageId",
    "MakeRequest", "Publish", "PublishProtocolMethodRequest",
    "PublishProtocolMethodResponse", "Subscribe", "SubscribeToProtocolMethodRequest",
    "SubscribeToProtocolMethodResponse", "GetCredentialsHeaders", "GetDeviceAccessToken",
    "GetDeviceIntegrityToken", "GetDeviceIntegrityTokenYield", "NoPromptCreateOutfit",
    "NoPromptDeleteOutfit", "NoPromptRenameOutfit", "NoPromptSaveAvatar",
    "NoPromptSaveAvatarThumbnailCustomization", "NoPromptSetFavorite",
    "NoPromptUpdateOutfit", "PerformCreateOutfitWithDescription", "PerformDeleteOutfit",
    "PerformRenameOutfit", "PerformSaveAvatarWithDescription", "PerformSetFavorite",
    "PerformUpdateOutfit", "PromptCreateOutfit", "PromptDeleteOutfit",
    "PromptRenameOutfit", "PromptSaveAvatar", "PromptSetFavorite", "PromptUpdateOutfit",
    "SetAllowInventoryReadAccess", "PromptAllowInventoryReadAccess",
    "SignalCreateOutfitFailed", "SignalCreateOutfitPermissionDenied",
    "SignalDeleteOutfitFailed", "SignalDeleteOutfitPermissionDenied",
    "SignalRenameOutfitFailed", "SignalRenameOutfitPermissionDenied",
    "SignalSaveAvatarPermissionDenied", "SignalSetFavoriteFailed",
    "SignalSetFavoritePermissionDenied", "SignalUpdateOutfitFailed",
    "SignalUpdateOutfitPermissionDenied", "GetLocalFileContents", "SetBaseUrl",
    "FetchAssetWithFormat", "RegisterUGCValidationFunction", "PerformManagedUpdate",
    "SetPurchasePromptIsShown", "DumpToFileAsync", "Run", "OpenInBrowser_DONOTUSE",
    "TryInstallPlugin", "PromptImportFile", "PromptImportFiles", "UninstallPlugin",
};

static std::unordered_set<std::string> blocked_services = {
    "BrowserService", "ScriptContext", "ScriptProfilerService", "MicroProfilerService",
    "TestService", "StudioService", "AccountService", "MessageBusService",
    "OpenCloudService", "HttpRbxApiService", "AvatarEditorService", "CaptureService",
    "LinkingService",
};

static std::string sanitize_key(const char* str, size_t len)
{
    std::string key;
    key.reserve(len);
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] != '\0' && str[i] != '\1')
            key += str[i];
    }
    return key;
}

static bool is_executor_state(lua_State* L)
{
    if (!L || !L->userdata)
        return false;

    auto* data = static_cast<RobloxExtraSpace*>(L->userdata);

    if (data->capabilities == Shared::MaxCapabilities)
        return true;

    if (Shared::ExploitThread && L == Shared::ExploitThread)
        return true;

    // A destroyed game LocalScript also has data->script.expired() == true;
    // identify executor threads by capabilities or executor-tagged callstack frames.
    if (L->ci && L->base_ci)
    {
        for (CallInfo* ci = L->ci; ci > L->base_ci; --ci)
        {
            if (!ci->func || !ttisfunction(ci->func))
                continue;
            Closure* cl = clvalue(ci->func);
            if (!cl->isC)
            {
                if (cl->l.p && cl->l.p->userdata == &Shared::MaxCapabilities)
                    return true;
            }
            else if (Shared::ExecutorClosures.contains(cl))
            {
                return true;
            }
        }
    }

    return false;
}

#include <RBX/environment/libs/signals/signals.hpp>

static int BindToRenderStep_Wrapper(lua_State* L)
{
    Signals::RecordRenderStepBinding(L);
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_insert(L, 1);
    lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
    return lua_gettop(L);
}

static int UnbindFromRenderStep_Wrapper(lua_State* L)
{
    Signals::ForgetRenderStepBindingByArg(L);
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_insert(L, 1);
    lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
    return lua_gettop(L);
}

static bool push_executor_method(lua_State* L, const std::string& key)
{
    lua_CFunction function = nullptr;
    const char* registryKey = nullptr;
    if (key == "HttpGet")
    {
        function = module::core::environment::c_http::http_get;
        registryKey = "module.instance_method.HttpGet";
    }
    else if (key == "HttpGetAsync")
    {
        function = module::core::environment::c_http::http_get;
        registryKey = "module.instance_method.HttpGetAsync";
    }
    else if (key == "HttpPost")
    {
        function = module::core::environment::c_http::http_post;
        registryKey = "module.instance_method.HttpPost";
    }
    else if (key == "HttpPostAsync")
    {
        function = module::core::environment::c_http::http_post;
        registryKey = "module.instance_method.HttpPostAsync";
    }
    else if (key == "GetObjects")
    {
        function = Miscellaneous::GetObjects;
        registryKey = "module.instance_method.GetObjects";
    }
    else if (key == "GetObjectsAsync")
    {
        function = Miscellaneous::GetObjects;
        registryKey = "module.instance_method.GetObjectsAsync";
    }
    else
        return false;

    lua_rawgetfield(L, LUA_REGISTRYINDEX, registryKey);
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
        lua_pushcclosure(L, function, key.c_str(), 0);
        lua_pushvalue(L, -1);
        lua_rawsetfield(L, LUA_REGISTRYINDEX, registryKey);
    }
    return true;
}

static inline void sync_executor_tls_capabilities(lua_State* L)
{
    if (!L || !L->userdata)
        return;

    auto* data = static_cast<RobloxExtraSpace*>(L->userdata);
    if (data->capabilities != Shared::MaxCapabilities)
        return;

    const uintptr_t identity_ptr = Roblox::GetIdentityStruct(*reinterpret_cast<uintptr_t*>(Offsets::RobloxThread::IdentityPtr));
    if (!identity_ptr)
        return;

    auto* tlsId = reinterpret_cast<uint64_t*>(identity_ptr);
    auto* tlsCaps = reinterpret_cast<uint64_t*>(identity_ptr + Offsets::Capabilities::Capabilities);
    if (*tlsId != static_cast<uint64_t>(data->identity) || (*tlsCaps & 0x3FULL) != 0x3FULL)
    {
        *tlsId = static_cast<uint64_t>(data->identity);
        *tlsCaps = data->capabilities;
    }
}

int index_hook(lua_State* L)
{
    sync_executor_tls_capabilities(L);

    if (!is_executor_state(L))
        return original_index(L);

    std::string key;
    if (lua_isstring(L, 2))
    {
        size_t len = 0;
        const char* str = lua_tolstring(L, 2, &len);
        if (str)
            key = sanitize_key(str, len);
    }

    if (key.empty())
        return original_index(L);

    if (blocked_funcs.count(key) > 0)
    {
        luaL_error(L, "attempt to access blocked function '%s'", key.c_str());
        return 0;
    }

    if (is_executor_state(L))
    {
        if (push_executor_method(L, key))
            return 1;
        else if (key == "BindToRenderStep")
        {
            int res = original_index(L);
            if (res == 1 && lua_isfunction(L, -1))
            {
                lua_pushcclosure(L, BindToRenderStep_Wrapper, "BindToRenderStep", 1);
                return 1;
            }
            return res;
        }
        else if (key == "UnbindFromRenderStep")
        {
            int res = original_index(L);
            if (res == 1 && lua_isfunction(L, -1))
            {
                lua_pushcclosure(L, UnbindFromRenderStep_Wrapper, "UnbindFromRenderStep", 1);
                return 1;
            }
            return res;
        }
    }

    Reflection::ScopedScriptable scopedScriptable(L, 1, key.c_str());
    return original_index(L);
}

int newindex_hook(lua_State* L)
{
    sync_executor_tls_capabilities(L);

    if (!is_executor_state(L) || !lua_isstring(L, 2))
        return original_newindex(L);

    size_t len = 0;
    const char* str = lua_tolstring(L, 2, &len);
    std::string key = str ? sanitize_key(str, len) : std::string();
    Reflection::ScopedScriptable scopedScriptable(L, 1, key.c_str());
    return original_newindex(L);
}

int namecall_hook(lua_State* L)
{
    Closures::RestoreNamecallAtom(L);
    sync_executor_tls_capabilities(L);
    const bool executorState = is_executor_state(L);

    std::string key;
    if (L->namecall)
    {
        const char* str = L->namecall->data;
        size_t len = L->namecall->len;
        if (str)
            key = sanitize_key(str, len);
    }

    if (key.empty())
        return original_namecall(L);

    if (executorState && blocked_services.count(key) > 0)
    {
        luaL_error(L, "attempt to access blocked service '%s'", key.c_str());
        return 0;
    }

    if (executorState && blocked_funcs.count(key) > 0)
    {
        luaL_error(L, "attempt to access blocked function '%s'", key.c_str());
        return 0;
    }

    if (executorState)
    {
        if (push_executor_method(L, key))
        {
            Closure* method = clvalue(luaA_toobject(L, -1));
            if (Closures::HookRecords.contains(method))
            {
                lua_insert(L, 1);
                luaD_callint(L, L->base, LUA_MULTRET, lua_isyieldable(L) != 0);
                return Closures::finish_yieldable_call(L);
            }
            lua_pop(L, 1);
        }
        if (key == "HttpGet" || key == "HttpGetAsync")
            return module::core::environment::c_http::http_get(L);
        else if (key == "HttpPost" || key == "HttpPostAsync")
            return module::core::environment::c_http::http_post(L);
        else if (key == "GetObjects" || key == "GetObjectsAsync")
            return Miscellaneous::GetObjects(L);
        else if (key == "BindToRenderStep")
        {
            Signals::RecordRenderStepBinding(L);
            return original_namecall(L);
        }
        else if (key == "UnbindFromRenderStep")
        {
            Signals::ForgetRenderStepBindingByArg(L);
            return original_namecall(L);
        }
    }

    if (Closures::HookRecords.empty())
        return original_namecall(L);

    // A shared method hook applies to every caller of that closure, including
    // ordinary game threads. Executor-specific methods remain in the gate above.
    // A bound method closure and the generic __namecall closure are distinct.
    // Route only an explicitly hooked method through its own closure so
    // instance:Method(...) and instance.Method(instance, ...) agree.
    // Resolving a colon-call's bound method must not also invoke a user
    // __index hook: the VM's namecall dispatch never does that.
    TString* methodName = L->namecall;
    const int methodSlot = L->cachedslot;
    // The native index closure carries its own cache upvalues. Calling its
    // C entrypoint under a __namecall frame would supply the wrong upvalues.
    lua_pushlightuserdata(L, const_cast<char*>(&g_NativeInstanceIndexKey));
    lua_rawget(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, 1);
    lua_pushlstring(L, key.data(), key.size());
    // A missing method must be diagnosed by the native namecall body.
    // Letting the lookup raise here changes both the error's stack frame
    // and the behavior of error handlers that inspect the failing call.
    const int lookupStatus = lua_pcall(L, 2, 1, 0);
    L->namecall = methodName;
    L->cachedslot = methodSlot;
    if (lookupStatus != LUA_OK)
    {
        lua_pop(L, 1);
        return original_namecall(L);
    }
    if (lua_isfunction(L, -1))
    {
        Closure* method = clvalue(luaA_toobject(L, -1));
        if (Closures::HookRecords.contains(method))
        {
            lua_insert(L, 1);
            luaD_callint(L, L->base, LUA_MULTRET, lua_isyieldable(L) != 0);
            return Closures::finish_yieldable_call(L);
        }
    }
    lua_pop(L, 1);
    return original_namecall(L);
}

bool IsInstanceNamecallFunction(lua_CFunction function)
{
    return function == namecall_hook || function == original_namecall;
}

void initialize_hooks(lua_State* L)
{
    int original_top = lua_gettop(L);

    lua_getglobal(L, "game");

    luaL_getmetafield(L, -1, "__index");
    if (lua_type(L, -1) == LUA_TFUNCTION)
    {
        Closure* index_closure = clvalue(luaA_toobject(L, -1));
        if (original_index == nullptr)
            original_index = index_closure->c.f;
        lua_pushlightuserdata(L, const_cast<char*>(&g_NativeInstanceIndexKey));
    lua_rawget(L, LUA_REGISTRYINDEX);
        if (!lua_isfunction(L, -1))
        {
            lua_pop(L, 1);
            Closure* native = Closures::clone_closure(L, index_closure);
            native->c.f = original_index;
            Closures::push_closure(L, native);
            lua_pushlightuserdata(L, const_cast<char*>(&g_NativeInstanceIndexKey));
            lua_insert(L, -2);
            lua_rawset(L, LUA_REGISTRYINDEX);
        }
        else
            lua_pop(L, 1);
        index_closure->c.f = index_hook;
    }
    lua_pop(L, 1);

    luaL_getmetafield(L, -1, "__newindex");
    if (lua_type(L, -1) == LUA_TFUNCTION)
    {
        Closure* newindex_closure = clvalue(luaA_toobject(L, -1));
        if (original_newindex == nullptr)
            original_newindex = newindex_closure->c.f;
        newindex_closure->c.f = newindex_hook;
    }
    lua_pop(L, 1);

    luaL_getmetafield(L, -1, "__namecall");
    if (lua_type(L, -1) == LUA_TFUNCTION)
    {
        Closure* namecall_closure = clvalue(luaA_toobject(L, -1));
        if (original_namecall == nullptr)
            original_namecall = namecall_closure->c.f;
        namecall_closure->c.f = namecall_hook;
    }
    lua_pop(L, 1);

    lua_settop(L, original_top);
}
