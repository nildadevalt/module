local bindable = Instance.new("BindableFunction")
local remote = Instance.new("RemoteFunction")
local expected = function(a, b) return a + b end
bindable.OnInvoke = expected
remote.OnServerInvoke = expected
getgenv().__callbackProbe = {bindable, remote, expected}
print("NERO_CALLBACK", getcallbackvalue(bindable, "OnInvoke"), getcallbackvalue(remote, "OnServerInvoke"))
local function indexFrame()
    local value
    xpcall(function() return game[{}] end, function()
        value = debug.info(2, "f")
        print("NERO_HOOK_FRAME", value, debug.info(value, "n"))
    end)
    return value
end
local before = indexFrame()
local old
old = hookmetamethod(game, "__index", function(...) return old(...) end)
local after = indexFrame()
print("NERO_HOOK", before == after, before, old, after)
hookmetamethod(game, "__index", old)
