local UIS = game:GetService("UserInputService")
local vim = game:GetService("VirtualInputManager")
local got = {}
local c = UIS.InputBegan:Connect(function(i) got[#got + 1] = i.UserInputType.Name .. "@" .. math.floor(i.Position.X) .. "," .. math.floor(i.Position.Y) end)
local function t(label, fn)
  got = {}
  local ok, err = pcall(fn)
  task.wait(0.12)
  print(string.format("[VA] %-42s ok=%s err=%s fired=[%s]", label, tostring(ok), tostring(err), table.concat(got, " ")))
end
t("(x,y,1,true,game,false)", function() vim:SendMouseButtonEvent(400, 300, 1, true, game, false) end)
t("(x,y,1,false,game,false)", function() vim:SendMouseButtonEvent(400, 300, 1, false, game, false) end)
t("(x,y,true,1,game,false)", function() vim:SendMouseButtonEvent(400, 300, true, 1, game, false) end)
t("(x,y,1,true,false,game)", function() vim:SendMouseButtonEvent(400, 300, 1, true, false, game) end)
t("(x,y,true,game,false,1)", function() vim:SendMouseButtonEvent(400, 300, true, game, false, 1) end)
c:Disconnect()
print("SUNCIO-DONE")
