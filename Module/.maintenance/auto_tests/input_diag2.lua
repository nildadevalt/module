local UIS = game:GetService("UserInputService")
local function say(...) print("[D2] " .. string.format(...)) end
say("typeof __inputdiag=%s __inputsinject=%s", typeof(__inputdiag), typeof(__inputsinject))
local ok, d = pcall(function() return __inputdiag() end)
say("diag call ok=%s val=%s", tostring(ok), tostring(d))
if ok and type(d) == "table" then
  say("found=%s fg=%s title='%s' client=%dx%d origin=(%d,%d) cursor=(%d,%d)",
      tostring(d.found), tostring(d.foreground), tostring(d.fg_title), d.client_w, d.client_h, d.origin_x, d.origin_y, d.cursor_x, d.cursor_y)
end
say("inject -> %s", tostring(__inputsinject and __inputsinject()))
local d2 = __inputdiag and __inputdiag()
if d2 then say("after fg=%s cursor=(%d,%d) origin=(%d,%d)", tostring(d2.foreground), d2.cursor_x, d2.cursor_y, d2.origin_x, d2.origin_y) end
mousemoveabs(400, 300)
task.wait(0.2)
local d3 = __inputdiag and __inputdiag()
if d3 then say("after move(400,300) cursor=(%d,%d) origin=(%d,%d)", d3.cursor_x, d3.cursor_y, d3.origin_x, d3.origin_y) end
say("UIS mouse=(%.0f,%.0f)", UIS:GetMouseLocation().X, UIS:GetMouseLocation().Y)
print("SUNCIO-DONE")
