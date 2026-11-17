local UIS = game:GetService("UserInputService")
local seen = {}
local c = UIS.InputBegan:Connect(function(i) seen[#seen + 1] = tostring(i.KeyCode) end)
keypress(0x7C)
task.wait(0.1)
local down = UIS:IsKeyDown(Enum.KeyCode.F13)
keyrelease(0x7C)
task.wait(0.1)
c:Disconnect()
print("[KB] events=" .. table.concat(seen, ",") .. " F13down=" .. tostring(down))

local part = workspace:FindFirstChildOfClass("Part")
for name, sig in pairs({ Touched = part.Touched, Heartbeat = game:GetService("RunService").Heartbeat }) do
  local ok, res = pcall(getsignalarguments, sig)
  local line = "[GS] " .. name .. " args ok=" .. tostring(ok) .. " -> "
  if ok then
    local parts = {}
    for i, v in ipairs(res) do parts[#parts + 1] = tostring(v) end
    line = line .. table.concat(parts, "|")
  else
    line = line .. tostring(res)
  end
  print(line)
  local ok2, res2 = pcall(getsignalargumentsinfo, sig)
  local line2 = "[GS] " .. name .. " info ok=" .. tostring(ok2) .. " -> "
  if ok2 then
    local parts = {}
    for i, v in ipairs(res2) do
      parts[#parts + 1] = tostring(v.Name) .. ":" .. tostring(v.Type)
    end
    line2 = line2 .. table.concat(parts, "|")
  else
    line2 = line2 .. tostring(res2)
  end
  print(line2)
end

print("[GS] cloneref(UIS)=" .. tostring(cloneref(game:GetService("UserInputService"))))
print("[GS] cloneref(game)=" .. tostring(cloneref(game)))
print("SUNCIO-DONE")
