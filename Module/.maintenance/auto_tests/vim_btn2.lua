local UIS = game:GetService("UserInputService")
local vim = game:GetService("VirtualInputManager")
local began, ended = 0, 0
local c1 = UIS.InputBegan:Connect(function(i) if i.UserInputType == Enum.UserInputType.MouseButton1 then began = began + 1 end end)
local c2 = UIS.InputEnded:Connect(function(i) if i.UserInputType == Enum.UserInputType.MouseButton1 then ended = ended + 1 end end)
for _, flagged in ipairs({ true, false }) do
  began, ended = 0, 0
  local ok = pcall(function() vim:SendMouseButtonEvent(400, 300, 1, true, game, flagged) end)
  task.wait(0.12)
  local down = UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)
  pcall(function() vim:SendMouseButtonEvent(400, 300, 1, false, game, flagged) end)
  task.wait(0.12)
  print(string.format("[V2] gameProcessed=%s ok=%s began=%d downAfterDown=%s ended=%d downAfterUp=%s",
    tostring(flagged), tostring(ok), began, tostring(down), ended,
    tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1))))
end
c1:Disconnect() c2:Disconnect()
print("SUNCIO-DONE")
