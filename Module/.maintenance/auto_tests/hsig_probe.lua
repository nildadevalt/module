-- Mirrors debUNC's hooksignal / issignalhooked / restoresignal contract.
local say = print
say("[HS] start")
local nv = Instance.new("NumberValue")
local sibling = Instance.new("NumberValue")
local allowed = Instance.new("NumberValue")

local fired, sibCount, allowCount = 0, 0, 0
local hookArg, hookValue

local function OnChanged() fired += 1 end

nv.Changed:Connect(OnChanged)
sibling.Changed:Connect(function() sibCount += 1 end)

local ok, err = pcall(function()
    say("[HS] hooked returned " .. tostring(hooksignal(nv.Changed, function(info, value)
        hookArg, hookValue = info, value
        return false
    end)))
end)
say("[HS] hooksignal ok=" .. tostring(ok) .. " err=" .. tostring(err))
say("[HS] ishooked=" .. tostring(issignalhooked(nv.Changed)))

nv.Value = 1
sibling.Value = 1
task.wait()
task.wait()

say("[HS] hookArg=" .. tostring(hookArg) .. " isTable=" .. tostring(typeof(hookArg) == "table"))
say("[HS] hookArg.Function==OnChanged: " .. tostring(typeof(hookArg) == "table" and hookArg.Function == OnChanged))
say("[HS] hookValue=" .. tostring(hookValue))
say("[HS] fired(suppressed, expect 0)=" .. tostring(fired))
say("[HS] sibCount(expect 1)=" .. tostring(sibCount))

allowed.Changed:Connect(function() allowCount += 1 end)
pcall(hooksignal, allowed.Changed, function() return true end)
allowed.Value = 1
task.wait()
task.wait()
say("[HS] allowCount(expect 1)=" .. tostring(allowCount))

restoresignal(nv.Changed)
say("[HS] after restore ishooked=" .. tostring(issignalhooked(nv.Changed)))
nv.Value = 2
task.wait()
task.wait()
say("[HS] after restore fired(expect 1)=" .. tostring(fired))

nv:Destroy(); sibling:Destroy(); allowed:Destroy()
say("SUNCIO-DONE")
