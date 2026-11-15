print("DIAG-PLACE", game.PlaceId)
local function probe(name, fn)
    local ok, err = pcall(fn)
    print("DIAG", name, ok, err)
end
probe("native environment", function()
    print("DIAG-ENV", getfenv(getgenv) == getgenv(), getfenv(getgenv) == getfenv(print))
    print("DIAG-ENV-CURRENT", getgenv() == getfenv(0), getgenv() == getfenv(1), next(getfenv(getgenv)))
end)
probe("connection function", function()
    local event = Instance.new("BindableEvent")
    local value
    local function handler(arg) value = arg end
    local native = event.Event:Connect(handler)
    for i, conn in ipairs(getconnections(event.Event)) do
        print("DIAG-CONN", i, conn.Function == handler, conn.Function, conn.Thread, conn.LuaConnection, conn.ForeignState)
        if conn.Function then conn.Function("direct") end
        print("DIAG-CONN-RESULT", value)
    end
    native:Disconnect()
    event:Destroy()
end)
probe("binary property", function()
    local part = Instance.new("Part")
    part:AddTag("alpha")
    for _, prop in ipairs({"Name", "Tags", "AttributesSerialize", "MaterialVariantSerialized"}) do
        local ok, value, hidden = pcall(gethiddenproperty, part, prop)
        print("DIAG-PROP", prop, ok, typeof(value), hidden, type(value) == "string" and #value or value)
        if type(value) == "string" then print("DIAG-PROP-HEX", prop, crypt.base64encode(value)) end
    end
    local ok, result = pcall(sethiddenproperty, part, "Tags", "beta\0gamma\0")
    print("DIAG-TAGS-SET", ok, result, table.concat(part:GetTags(), ","))
    print("DIAG-TAGS-READBACK", crypt.base64encode(gethiddenproperty(part, "Tags")))
    part:Destroy()
end)
probe("method hook scope", function()
    local part = Instance.new("Part")
    local hits = 0
    local old
    print("DIAG-HOOK-METHODS", part.IsA == part.GetChildren, part.IsA == game.IsA)
    old = hookfunction(part.IsA, function(self, ...)
        hits += 1
        return old(self, ...)
    end)
    print("DIAG-HOOK-ISA", part:IsA("BasePart"), hits)
    hits = 0
    print("DIAG-HOOK-CHILDREN", #part:GetChildren(), hits)
    hits = 0
    print("DIAG-HOOK-DIRECT", part.IsA(part, "BasePart"), hits)
    hits = 0
    print("DIAG-HOOK-OTHER-DIRECT", #part.GetChildren(part), hits)
    hits = 0
    print("DIAG-HOOK-WRONG-ARGS", pcall(part.IsA), hits)
    restorefunction(part.IsA)
    part:Destroy()
end)
probe("namecall error stack", function()
    xpcall(function() game:__invalid_diagnostic_method() end, function(err)
        print("DIAG-STACK-ERROR", err)
        for level = 1, 8 do
            local fn, source, name = debug.info(level, "fsn")
            if not fn then break end
            print("DIAG-STACK", level, fn, source, name, fn == getrawmetatable(game).__namecall)
        end
    end)
end)
probe("nested namecall", function()
    local part = Instance.new("Part")
    local mt = getrawmetatable(game)
    local old
    old = hookfunction(mt.__namecall, function(self, ...)
        local method = getnamecallmethod()
        if method == "IsA" then
            game:GetService("Players")
            print("DIAG-NESTED-METHOD", method, getnamecallmethod())
        end
        return old(self, ...)
    end)
    local ok, result = pcall(function() return part:IsA("BasePart") end)
    restorefunction(mt.__namecall)
    part:Destroy()
    print("DIAG-NESTED-RESULT", ok, result)
end)
print("DIAG-DONE")
