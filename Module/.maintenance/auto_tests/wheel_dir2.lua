local UIS = game:GetService("UserInputService")
local vim = game:GetService("VirtualInputManager")
local cam = workspace.CurrentCamera
local plr = game.Players.LocalPlayer
local chr = plr.Character or plr.CharacterAdded:Wait()
local root = chr:WaitForChild("HumanoidRootPart")
local function dist() return math.floor((cam.CFrame.Position - root.Position).Magnitude + 0.5) end
local n, z = 0, nil
local c = UIS.InputChanged:Connect(function(i)
  if i.UserInputType == Enum.UserInputType.MouseWheel then n = n + 1 z = i.Position.Z end
end)
local function trial(fn, label, d)
  n, z = 0, nil
  local d0 = dist()
  fn(d)
  task.wait(0.35)
  local d1 = dist()
  print(string.format("[W2] %-5s d=%3d ev=%d Z=%s cam %d->%d %s",
    label, d, n, tostring(z), d0, d1, d1 < d0 and "ZOOM-IN" or (d1 > d0 and "ZOOM-OUT" or "same")))
end
for _, d in ipairs({ 5, -5 }) do trial(function(x) vim:SendMouseWheelEvent(500, 400, x, game) end, "vim", d) end
for _, d in ipairs({ 5, -5 }) do trial(function(x) __mousescroll_os(x) end, "os", d) end
for _, d in ipairs({ 5, -5 }) do trial(mousescroll, "shim", d) end
c:Disconnect()
print("SUNCIO-DONE")
