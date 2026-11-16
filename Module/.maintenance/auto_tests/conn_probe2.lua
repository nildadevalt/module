local say = print
say("[CP2] start")
local nv = Instance.new("NumberValue")
local fired = 0
nv.Changed:Connect(function() fired += 1 end)

local e = getconnections(nv.Changed)[1]
say("[CP2] entryDisconnect=" .. tostring((function() local o,k = pcall(function() return typeof(e.Disconnect) end); return o and k or "ERR" end)()))
say("[CP2] objTostring=" .. tostring((function() local o,k = pcall(tostring, e.Object); return o and k or "ERR" end)()))
say("[CP2] objDisconnect=" .. tostring((function() local o,k = pcall(function() return typeof(e.Object.Disconnect) end); return o and k or "ERR" end)()))

local ok, err = pcall(function() e:Disconnect() end)
say("[CP2] e:Disconnect ok=" .. tostring(ok) .. " err=" .. tostring(err))
nv.Value = 1
task.wait(); task.wait()
say("[CP2] fired(expect 0)=" .. tostring(fired))
nv:Destroy()
say("SUNCIO-DONE")
