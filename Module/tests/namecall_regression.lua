--!strict

assert(game.PlaceId == 90441122676618, "Wrong test place")
local mt = getrawmetatable(game)
local errorFrame
local ok = xpcall(function()
    game:__module_missing_method_regression()
end, function(message)
    errorFrame = debug.info(2, "f")
    return message
end)
assert(not ok, "Invalid method unexpectedly succeeded")
assert(errorFrame == mt.__namecall, "Invalid namecall error came from another metamethod")
print("NAMECALL-REGRESSION native error frame passed")

local part = Instance.new("Part")
local hits = 0
local old
old = hookfunction(part.IsA, function(self, ...)
    hits += 1
    return old(self, ...)
end)
assert(part:IsA("BasePart") and hits == 1, "Colon call missed its method hook")
assert(part.IsA(part, "BasePart") and hits == 2, "Direct call missed its method hook")
assert(#part:GetChildren() == 0 and hits == 2, "Unrelated method entered the hook")
assert(restorefunction(part.IsA), "Method restoration failed")
assert(part:IsA("BasePart") and hits == 2, "Restored method still called its hook")
part:Destroy()
print("NAMECALL-REGRESSION method dispatch passed")

local httpTarget = game.HttpGet
assert(httpTarget == game.HttpGet, "HttpGet method reference changed between reads")
local httpHits = 0
hookfunction(httpTarget, function()
    httpHits += 1
    return "intercepted"
end)
assert(game:HttpGet("") == "intercepted" and httpHits == 1, "HttpGet colon call missed its hook")
assert(httpTarget(game, "") == "intercepted" and httpHits == 2, "HttpGet direct call missed its hook")
assert(restorefunction(httpTarget), "HttpGet restoration failed")
assert(httpTarget == game.HttpGet, "Restoring HttpGet changed its method reference")
print("NAMECALL-REGRESSION virtual method dispatch passed")
print("NAMECALL-REGRESSION-DONE")
