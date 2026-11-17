local UIS = game:GetService("UserInputService")
local counts = {}
local conns = {}
for _, sig in ipairs({ UIS.InputBegan, UIS.InputChanged, UIS.InputEnded }) do
  conns[#conns + 1] = sig:Connect(function(i)
    local k = tostring(i.UserInputType)
    counts[k] = (counts[k] or 0) + 1
  end)
end
local function snap(tag)
  local parts = {}
  for k, v in pairs(counts) do parts[#parts + 1] = k .. "=" .. v end
  print(string.format("[NS] %-12s %.2f  %s", tag, os.clock(), table.concat(parts, " ")))
  counts = {}
end
task.wait(1.0)
snap("idle 1s")
task.wait(1.0)
snap("idle 2s")
mousemoveabs(600, 400)
task.wait(0.5)
snap("move +0.5s")
task.wait(0.5)
snap("move +1.0s")
mouse1click()
task.wait(0.5)
snap("click +0.5s")
for _, c in ipairs(conns) do c:Disconnect() end
print("SUNCIO-DONE")
