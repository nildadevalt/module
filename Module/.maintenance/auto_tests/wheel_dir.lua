local UIS = game:GetService("UserInputService")
local vim = game:GetService("VirtualInputManager")
local function say(...) print("[WD] " .. string.format(...)) end

local function capture(delta, label)
  local z, dz, n = nil, nil, 0
  local c = UIS.InputChanged:Connect(function(i)
    if i.UserInputType == Enum.UserInputType.MouseWheel then
      n = n + 1
      z = i.Position.Z
      local ok, d = pcall(function() return i.Delta.Z end)
      if ok then dz = d end
    end
  end)
  if label == "vim" then
    pcall(vim.SendMouseWheelEvent, vim, 500, 400, delta, game)
  else
    mousescroll(delta)
  end
  task.wait(0.2)
  c:Disconnect()
  say("%-4s delta=%4d -> events=%d Position.Z=%s Delta.Z=%s", label, delta, n, tostring(z), tostring(dz))
end

for _, d in ipairs({ 1, -1, 3, -3, 120, -120 }) do capture(d, "vim") end
for _, d in ipairs({ 1, -1, 3, -3 }) do capture(d, "ours") end
print("SUNCIO-DONE")
