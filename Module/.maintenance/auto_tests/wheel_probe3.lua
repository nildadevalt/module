local UIS = game:GetService("UserInputService")
local VIM = game:GetService("VirtualInputManager")
local seen = {}
local conn = UIS.InputChanged:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseWheel then
        local p, d = input.Position, input.Delta
        print(string.format("[W] uitype=Wheel pos=(%.1f,%.1f,%.1f) delta=(%.1f,%.1f,%.1f) gp=%s",
            p.X, p.Y, p.Z, d.X, d.Y, d.Z, tostring(input.IsGameProcessedEvent)))
    end
end)

task.wait(0.3)
print("[W] focused=" .. tostring(__client_focused()))

local diag = __inputdiag()
print("[W] hwnd=" .. tostring(diag.found) .. " fg=" .. tostring(diag.foreground) .. " client=" .. tostring(diag.client_w) .. "x" .. tostring(diag.client_h))

local cx, cy = math.floor(diag.client_w / 2), math.floor(diag.client_h / 2)
print("[W] --- VIM +5 ---")
pcall(function() VIM:SendMouseWheelEvent(cx, cy, 5, game) end)
task.wait(0.4)
print("[W] --- VIM -5 ---")
pcall(function() VIM:SendMouseWheelEvent(cx, cy, -5, game) end)
task.wait(0.4)
print("[W] --- VIM -1 ---")
pcall(function() VIM:SendMouseWheelEvent(cx, cy, -1, game) end)
task.wait(0.4)
print("[W] --- mousescroll(-3) ---")
mousescroll(-3)
task.wait(0.4)
print("[W] --- mousescroll(3) ---")
mousescroll(3)
task.wait(0.5)
conn:Disconnect()
print("SUNCIO-DONE")
