getgenv().__rxs_unlock_mouse = true
local UIS = game:GetService("UserInputService")
local plr = game.Players.LocalPlayer
local cam = workspace.CurrentCamera
local chr = plr.Character or plr.CharacterAdded:Wait()
local root = chr:WaitForChild("HumanoidRootPart")
local function dist() return math.floor((cam.CFrame.Position - root.Position).Magnitude + 0.5) end
for i = 1, 6 do
  mousescroll(5)
  task.wait(0.35)
  print(string.format("[CP] scroll %d dist=%d beh=%s btn1=%s mode=%s",
    i, dist(), tostring(UIS.MouseBehavior),
    tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)),
    tostring(plr.CameraMode)))
end
mouse1press()
task.wait(0.15)
print("[CP] pressed=" .. tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)))
mouse1release()
task.wait(0.2)
print("[CP] released=" .. tostring(not UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)))
print("SUNCIO-DONE")
