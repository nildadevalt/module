local UIS = game:GetService("UserInputService")
local function say(...) print("[IN] " .. string.format(...)) end

local vp = workspace.CurrentCamera.ViewportSize
say("viewport=%.0fx%.0f  start=(%.0f,%.0f)  focused=%s", vp.X, vp.Y, UIS:GetMouseLocation().X, UIS:GetMouseLocation().Y, tostring(UIS.WindowFocused))
local b0 = UIS:GetMouseLocation()
mousemoveabs(400, 300)
task.wait(0.25)
local a1 = UIS:GetMouseLocation()
say("abs target=(400,300)  before=(%.0f,%.0f)  after=(%.0f,%.0f)  delta=(%.0f,%.0f)",
    b0.X, b0.Y, a1.X, a1.Y, a1.X - 400, a1.Y - 300)

mousemoveabs(700, 450)
task.wait(0.25)
local a2 = UIS:GetMouseLocation()
say("abs target=(700,450)  after=(%.0f,%.0f)  delta=(%.0f,%.0f)", a2.X, a2.Y, a2.X - 700, a2.Y - 450)

mousemoverel(60, 40)
task.wait(0.25)
local a3 = UIS:GetMouseLocation()
say("rel +60,+40        from=(%.0f,%.0f)  after=(%.0f,%.0f)  delta=(%.0f,%.0f)",
    a2.X, a2.Y, a3.X, a3.Y, a3.X - a2.X - 60, a3.Y - a2.Y - 40)

local clicked, keyed = 0, 0
local c1 = UIS.InputBegan:Connect(function(i) if i.UserInputType == Enum.UserInputType.MouseButton1 then clicked = clicked + 1 end end)
local c2 = UIS.InputBegan:Connect(function(i) if i.KeyCode == Enum.KeyCode.E then keyed = keyed + 1 end end)
mouse1click()
task.wait(0.3)
say("mouse1click detected=%d", clicked)
local t0 = os.clock()
keypress(0x45)
task.wait(0.4)
say("keypress(E) detected=%d  elapsed=%.2fs", keyed, os.clock() - t0)
say("window focused=%s active=%s", tostring(UIS.WindowFocused), tostring(UIS.WindowActive))
c1:Disconnect(); c2:Disconnect()
print("SUNCIO-DONE")

local d = __inputdiag()
say("diag found=%s fg=%s title='%s' client=%dx%d origin=(%d,%d) cursor=(%d,%d)",
    tostring(d.found), tostring(d.foreground), d.fg_title, d.client_w, d.client_h, d.origin_x, d.origin_y, d.cursor_x, d.cursor_y)
say("force focus -> %s", tostring(__inputsinject()))
local d2 = __inputdiag()
say("after inject fg=%s cursor=(%d,%d)", tostring(d2.foreground), d2.cursor_x, d2.cursor_y)
local clk = 0
local c3 = UIS.InputBegan:Connect(function(i) clk = clk + 1; say("input began: %s %s", tostring(i.UserInputType), tostring(i.KeyCode)) end)
mouse1click()
task.wait(0.35)
say("click after inject detected=%d", clk)
c3:Disconnect()
print("SUNCIO-DONE")
