local UIS = game:GetService("UserInputService")
local cam = workspace.CurrentCamera
local plr = game.Players.LocalPlayer
local chr = plr.Character or plr.CharacterAdded:Wait()
local root = chr:WaitForChild("HumanoidRootPart")
local function dist() return math.floor((cam.CFrame.Position - root.Position).Magnitude + 0.5) end
local hits, z, n = 0, nil, 0
local c1 = UIS.InputChanged:Connect(function(i)
  if i.UserInputType == Enum.UserInputType.MouseWheel then n = n + 1 z = i.Position.Z end
end)
local c2 = UIS.InputBegan:Connect(function(i)
  if i.UserInputType == Enum.UserInputType.MouseButton1 then hits = hits + 1 end
end)
for _, d in ipairs({ 5, -5, -5, 5 }) do
  n, z = 0, nil
  local d0 = dist()
  mousescroll(d)
  task.wait(0.3)
  print(string.format("[WA] scroll %3d ev=%d Z=%s cam %d->%d  mouseBeh=%s",
    d, n, tostring(z), d0, dist(), tostring(UIS.MouseBehavior)))
end
hits = 0
mouse1press(); task.wait(0.05); mouse1release()
task.wait(0.2)
print("[WA] click after scroll detected=" .. tostring(hits > 0))
c1:Disconnect() c2:Disconnect()
print("SUNCIO-DONE")
