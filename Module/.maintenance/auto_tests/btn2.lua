local UIS = game:GetService("UserInputService")
local seen = {}
local conns = {
  UIS.InputBegan:Connect(function(i) seen[#seen+1] = "B:"..tostring(i.UserInputType).."@"..tostring(math.floor(i.Position.X))..","..tostring(math.floor(i.Position.Y)) end),
  UIS.InputEnded:Connect(function(i) seen[#seen+1] = "E:"..tostring(i.UserInputType) end),
}
mouse1press()
task.wait(0.15)
print("[BT] after press: down="..tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)).." events="..table.concat(seen,","))
seen = {}
mouse1release()
task.wait(0.15)
print("[BT] after release: down="..tostring(UIS:IsMouseButtonPressed(Enum.UserInputType.MouseButton1)).." events="..table.concat(seen,","))
seen = {}
mouse1click()
task.wait(0.15)
print("[BT] click events="..table.concat(seen,","))
local d = __inputdiag()
print(string.format("[BT] diag found=%s fg=%s fgtitle=%s client=%dx%d cursor=%d,%d origin=%d,%d",
  tostring(d.found), tostring(d.foreground), tostring(d.fg_title), d.client_w, d.client_h, d.cursor_x, d.cursor_y, d.origin_x, d.origin_y))
for _, c in ipairs(conns) do c:Disconnect() end
print("SUNCIO-DONE")
