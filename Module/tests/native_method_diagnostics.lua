--!strict

assert(game.PlaceId == 90441122676618, "Wrong test place")
local function probe(label, object, methodName, colonCall, arguments)
    local target = object[methodName]
    local hits = 0
    local old
    local success, message = pcall(function()
        assert(target == object[methodName], "Method reference changes between reads")
        old = hookfunction(target, function(...)
            hits += 1
            return old(...)
        end)
        colonCall()
        assert(hits == 1, "Colon call missed its method hook")
        target(object, table.unpack(arguments))
        assert(hits == 2, "Direct call missed its method hook")
    end)
    if old then restorefunction(target) end
    print("NATIVE-METHOD", label, success, message, hits)
end

local part = Instance.new("Part")
probe("Instance.IsA", part, "IsA", function() return part:IsA("BasePart") end, {"BasePart"})
probe("Instance.GetChildren", part, "GetChildren", function() return part:GetChildren() end, {})
probe("DataModel.GetService", game, "GetService", function() return game:GetService("Players") end, {"Players"})

local cf = CFrame.new(1, 2, 3)
probe("CFrame.Inverse", cf, "Inverse", function() return cf:Inverse() end, {})
local vector = Vector3.new(1, 0, 0)
probe("Vector3.Dot", vector, "Dot", function() return vector:Dot(vector) end, {vector})
local color = Color3.new(1, 0, 0)
probe("Color3.Lerp", color, "Lerp", function() return color:Lerp(color, 0.5) end, {color, 0.5})
local random = Random.new(42)
probe("Random.NextNumber", random, "NextNumber", function() return random:NextNumber(0, 0) end, {0, 0})
probe("Enum.GetEnumItems", Enum.Material, "GetEnumItems", function() return Enum.Material:GetEnumItems() end, {})

local httpTarget = game.HttpGet
local httpOld
local success, message = pcall(function()
    assert(httpTarget == game.HttpGet, "HttpGet reference changes between reads")
    httpOld = hookfunction(httpTarget, function() return "intercepted" end)
    assert(game:HttpGet("") == "intercepted", "HttpGet colon call missed its hook")
    assert(httpTarget(game, "") == "intercepted", "HttpGet direct call missed its hook")
end)
if httpOld then restorefunction(httpTarget) end
print("NATIVE-METHOD", "DataModel.HttpGet", success, message)
part:Destroy()
print("NATIVE-METHOD-DONE")
