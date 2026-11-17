local UIS = game:GetService("UserInputService")
local function say(...) print("[VIM] " .. string.format(...)) end
local ok, vim = pcall(function() return game:GetService("VirtualInputManager") end)
say("service ok=%s type=%s", tostring(ok), typeof(vim))
if not ok or typeof(vim) ~= "Instance" then print("SUNCIO-DONE") return end

local hit, nb1 = 0, 0
local c1 = UIS.InputBegan:Connect(function(i) hit = hit + 1 if i.UserInputType == Enum.UserInputType.MouseButton1 then nb1 = nb1 + 1 end end)
local ok2, err = pcall(function()
  vim:SendMouseButtonEvent(500, 400, 0, true, game, 1)
end)
say("SendMouseButtonEvent ok=%s err=%s", tostring(ok2), tostring(err))
task.wait(0.1)
say("pressed=%s began=%d m1=%d", tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)), hit, nb1)
pcall(function() vim:SendMouseButtonEvent(500, 400, 0, false, game, 1) end)
task.wait(0.2)
say("after up: pressed=%s began=%d m1=%d mouse=(%.0f,%.0f)",
    tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)), hit, nb1,
    UIS:GetMouseLocation().X, UIS:GetMouseLocation().Y)

local keyhit = 0
local c2 = UIS.InputBegan:Connect(function(i) if i.KeyCode == Enum.KeyCode.E then keyhit = keyhit + 1 end end)
local ok3, err3 = pcall(function() vim:SendKeyEvent(true, Enum.KeyCode.E, false, game) end)
task.wait(0.1)
local ok4 = pcall(function() vim:SendKeyEvent(false, Enum.KeyCode.E, false, game) end)
task.wait(0.15)
say("SendKeyEvent ok=%s/%s err=%s keyhit=%d", tostring(ok3), tostring(ok4), tostring(err3), keyhit)
c1:Disconnect(); c2:Disconnect()
print("SUNCIO-DONE")
