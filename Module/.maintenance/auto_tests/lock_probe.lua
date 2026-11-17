local UIS = game:GetService("UserInputService")
local function say(...) print("[LK] " .. string.format(...)) end
say("t=%.2f behavior=%s", os.clock(), tostring(UIS.MouseBehavior))
local vp = workspace.CurrentCamera.ViewportSize
mousemoveabs(vp.X / 2, vp.Y / 2)
say("after move t=%.2f behavior=%s", os.clock(), tostring(UIS.MouseBehavior))
for i = 1, 12 do
  task.wait(0.5)
  say("t=%.2f behavior=%s mouse=(%.0f,%.0f)", os.clock(), tostring(UIS.MouseBehavior),
      UIS:GetMouseLocation().X, UIS:GetMouseLocation().Y)
end
print("SUNCIO-DONE")
