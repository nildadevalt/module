local UIS = game:GetService("UserInputService")
local function say(...) print("[BT] " .. string.format(...)) end
local function state(tag)
  say("%s behavior=%s b1=%s b2=%s b3=%s", tag, tostring(UIS.MouseBehavior),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton2)),
      tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton3)))
end
state("start")
mousemoveabs(500, 400)
task.wait(0.1)
state("after move")
mouse1press(); task.wait(0.1); state("after m1 press")
mouse1release(); task.wait(0.1); state("after m1 release")
mouse2press(); task.wait(0.1); state("after m2 press")
mouse2release(); task.wait(0.1); state("after m2 release")
mousescroll(3); task.wait(0.1); state("after scroll+3")
mousescroll(-3); task.wait(0.1); state("after scroll-3")
keypress(69); task.wait(0.1)
say("E down=%s", tostring(UIS:IsKeyDown(Enum.KeyCode.E)))
keyrelease(69); task.wait(0.1)
say("E down after release=%s", tostring(UIS:IsKeyDown(Enum.KeyCode.E)))
print("SUNCIO-DONE")
