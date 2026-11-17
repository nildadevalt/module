local UIS = game:GetService("UserInputService")
local last
for i = 1, 60 do
  local mb = tostring(UIS.MouseBehavior)
  if mb ~= last then
    print(string.format("[IL] t=%.1f behavior=%s cursor_fg=%s", os.clock(), mb, tostring(__inputdiag().foreground)))
    last = mb
  end
  task.wait(0.5)
end
print("SUNCIO-DONE")
