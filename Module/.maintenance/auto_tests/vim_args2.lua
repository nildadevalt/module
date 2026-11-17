local UIS = game:GetService("UserInputService")
local vim = game:GetService("VirtualInputManager")
local got = {}
local c1 = UIS.InputBegan:Connect(function(i) got[#got + 1] = "B:" .. i.UserInputType.Name end)
local c2 = UIS.InputEnded:Connect(function(i) got[#got + 1] = "E:" .. i.UserInputType.Name end)
local function t(label, fn)
  got = {}
  local ok, err = pcall(fn)
  task.wait(0.12)
  print(string.format("[V3] %-34s ok=%s err=%s fired=[%s]", label, tostring(ok), tostring(err), table.concat(got, " ")))
end
for id = 0, 3 do
  t("down id=" .. id, function() vim:SendMouseButtonEvent(400, 300, id, true, game, 0) end)
  t("up   id=" .. id, function() vim:SendMouseButtonEvent(400, 300, id, false, game, 0) end)
end
c1:Disconnect() c2:Disconnect()
print("SUNCIO-DONE")
