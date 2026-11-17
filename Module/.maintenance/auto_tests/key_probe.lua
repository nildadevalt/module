local UIS = game:GetService("UserInputService")
local function say(...) print("[KY] " .. string.format(...)) end
local vim = game:GetService("VirtualInputManager")
say("typeof keypress=%s vim=%s", typeof(keypress), typeof(vim))
local n = 0
local c = UIS.InputBegan:Connect(function(i) if i.KeyCode == Enum.KeyCode.E then n = n + 1 end end)
say("manual vim ok=%s", tostring(pcall(vim.SendKeyEvent, vim, true, Enum.KeyCode.E, false, game)))
task.wait(0.1)
pcall(vim.SendKeyEvent, vim, false, Enum.KeyCode.E, false, game)
task.wait(0.15)
say("manual detected=%d", n)
n = 0
say("our keypress ok=%s", tostring(pcall(keypress, 69)))
task.wait(0.1)
say("our keypress held=%s", tostring(UIS:IsKeyDown(Enum.KeyCode.E)))
pcall(keyrelease, 69)
task.wait(0.15)
say("our keypress detected=%d", n)
local found
for _, k in ipairs(Enum.KeyCode:GetEnumItems()) do if k.Value == 69 then found = k end end
say("enum lookup 69 -> %s", tostring(found))
c:Disconnect()
print("SUNCIO-DONE")
