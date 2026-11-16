local say = print  -- capture before any closure replacement
say("[KA] script started")
local function step(name, fn)
    say("[KA] -> " .. name)
    local ok, err = pcall(fn)
    say("[KA] " .. name .. (ok and " ok" or (" ERR " .. tostring(err))))
end

step("newcclosure", function()
    local fnc = newcclosure(function(a) return a end)
    assert(fnc(1) == 1, "identity")
    assert(type(fnc) == "function")
end)
step("hookfunction", function()
    local orig = game.HttpGet
    local hooked = hookfunction(orig, function(...) return orig(...) end)
    assert(type(hooked) == "function", "hookfunction returned " .. type(hooked))
end)
step("replaceclosure", function()
    local victim = function() return 1 end
    replaceclosure(victim, function() return 2 end)
    assert(victim() == 2, "replaceclosure no effect")
end)
step("hookmetamethod", function()
    local mt = getrawmetatable(game)
    local old = mt.__namecall
    setreadonly(mt, false)
    mt.__namecall = function(self, ...) return old(self, ...) end
    setreadonly(mt, true)
    assert(getrawmetatable(game).__namecall ~= old, "metamethod not swapped")
end)
step("setstackhidden", function()
    setstackhidden(true)
end)
step("checkcaller", function()
    assert(checkcaller() == true)
end)
say("[KA] all steps done, idling")
for i = 1, 30 do task.wait(2) end
say("SUNCIO-DONE")
