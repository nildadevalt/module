local UIS = game:GetService("UserInputService")
local events = {}
local c1 = UIS.InputBegan:Connect(function(i) events[#events + 1] = "B:" .. tostring(i.KeyCode) .. "/" .. tostring(i.UserInputType) end)
local c2 = UIS.InputEnded:Connect(function(i) events[#events + 1] = "E:" .. tostring(i.KeyCode) .. "/" .. tostring(i.UserInputType) end)

keypress(0x7C)
task.wait(0.1)
print("[KC] after keypress(0x7C) F13 down=" .. tostring(UIS:IsKeyDown(Enum.KeyCode.F13)))
keyrelease(0x7C)
task.wait(0.1)
print("[KC] events: " .. table.concat(events, " "))
c1:Disconnect() c2:Disconnect()

-- GetService in different execution contexts
local function probe(name, fn)
  local ok, res = pcall(fn)
  print(string.format("[KC] %-14s ok=%s value=%s", name, tostring(ok), tostring(res)))
end
probe("toplevel", function() return tostring(game:GetService("UserInputService")) end)
probe("coroutine", function()
  local co = coroutine.create(function()
    local v = tostring(game:GetService("UserInputService"))
    coroutine.yield(v)
  end)
  local ok2, v = coroutine.resume(co)
  return v
end)
probe("task.spawn", function()
  local got
  task.spawn(function() got = tostring(game:GetService("UserInputService")) end)
  task.wait(0.05)
  return got
end)
probe("task.defer", function()
  local got
  task.defer(function() got = tostring(game:GetService("UserInputService")) end)
  task.wait(0.05)
  return got
end)
probe("syn.thread", function()
  local t = coroutine.running()
  return tostring(game:GetService("UserInputService"))
end)
probe("via game.Parent", function() return tostring(game:FindService("UserInputService")) end)
print("SUNCIO-DONE")
