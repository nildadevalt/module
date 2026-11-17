local function say(...) print("[EN] " .. string.format(...)) end
local items = Enum.KeyCode:GetEnumItems()
say("count=%d  E.Value=%s  E.Name=%s", #items, tostring(Enum.KeyCode.E.Value), tostring(Enum.KeyCode.E.Name))
say("typeof E=%s  fromItems=%s", typeof(Enum.KeyCode.E), tostring(items[1] and items[1].Name))
local found
for _, k in ipairs(items) do if k.Name == "E" then found = k end end
say("by name in list -> %s (value=%s)", tostring(found), tostring(found and found.Value))
say("index by name -> %s", tostring(Enum.KeyCode["E"]))
local ok, e = pcall(function() return Enum.KeyCode.E end)
say("pcall E ok=%s", tostring(ok))
-- what vim accepts: pass our enum item
local vim = game:GetService("VirtualInputManager")
local n = 0
local c = game:GetService("UserInputService").InputBegan:Connect(function(i) if i.KeyCode == Enum.KeyCode.E then n = n + 1 end end)
local ok2, err = pcall(vim.SendKeyEvent, vim, true, Enum.KeyCode.E, false, game)
task.wait(0.1)
pcall(vim.SendKeyEvent, vim, false, Enum.KeyCode.E, false, game)
task.wait(0.15)
say("send with our enum ok=%s err=%s detected=%d", tostring(ok2), tostring(err), n)
c:Disconnect()
print("SUNCIO-DONE")
