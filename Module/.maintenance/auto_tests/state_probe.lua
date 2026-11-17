local UIS = game:GetService("UserInputService")
local function snap(tag)
  local ok, mb = pcall(function() return UIS.MouseBehavior end)
  print(string.format("[ST] %-10s t=%.1f behavior=%s b1=%s b2=%s b3=%s mouse=(%.0f,%.0f)",
      tag, os.clock(), tostring(mb),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton2)),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton3)),
      UIS:GetMouseLocation().X, UIS:GetMouseLocation().Y))
end
snap("start")
for i = 1, 6 do
  task.wait(5)
  snap("idle" .. i)
end
print("SUNCIO-DONE")
