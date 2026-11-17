getgenv().__rxs_unlock_mouse = true
local UIS = game:GetService("UserInputService")
local function say(...) print("[WH] " .. string.format(...)) end
local events = {}
local c = UIS.InputChanged:Connect(function(i)
  if i.UserInputType == Enum.UserInputType.MouseWheel then
    events[#events + 1] = i.Position.Z
  end
end)
mousemoveabs(600, 400); task.wait(0.1)
events = {}
mousescroll(1); task.wait(0.2)
say("scroll(1) -> %d events: %s", #events, table.concat(events, ","))
events = {}
mousescroll(-1); task.wait(0.2)
say("scroll(-1) -> %d events: %s", #events, table.concat(events, ","))
c:Disconnect()
print("SUNCIO-DONE")
