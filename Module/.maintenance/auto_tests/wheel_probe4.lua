local UIS = game:GetService("UserInputService")
local players = game:GetService("Players")
local cam = workspace.CurrentCamera

local function report(tag)
    print(string.format("[W4] %s focused=%s behavior=%s mb1=%s camDist=%.1f",
        tag, tostring(__client_focused()), tostring(UIS.MouseBehavior),
        tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)),
        (cam.CFrame.Position - (players.LocalPlayer.Character and players.LocalPlayer.Character:GetPivot().Position or Vector3.zero)).Magnitude))
end

local events = {}
local conn = UIS.InputChanged:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseWheel then
        table.insert(events, string.format("pos=(%.0f,%.0f,%.0f)", input.Position.X, input.Position.Y, input.Position.Z))
    end
end)

task.wait(0.3)
report("start")

print("[W4] --- unfocused mousescroll(-5) ---")
mousescroll(-5)
task.wait(0.4)
print("[W4] events after -5: " .. table.concat(events, " "))
report("after -5")

table.clear(events)
print("[W4] --- unfocused mousescroll(5) ---")
mousescroll(5)
task.wait(0.4)
print("[W4] events after +5: " .. table.concat(events, " "))
report("after +5")

conn:Disconnect()
print("SUNCIO-DONE")
