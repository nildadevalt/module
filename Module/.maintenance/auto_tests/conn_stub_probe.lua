local say = print
say("[CS] start")
local nv = Instance.new("NumberValue")
local c = nv.Changed:Connect(function() end)
local list = getconnections(nv.Changed)
local e = list[1]
say("[CS] count=" .. tostring(#list) .. " Enabled=" .. tostring(e and e.Enabled) .. " DisconnectType=" .. tostring(e and typeof(e.Disconnect)) .. " Thread=" .. tostring(e and typeof(e.Thread)))
if e then
    local ok = pcall(function() e:Disable() end)
    say("[CS] after Disable ok=" .. tostring(ok) .. " Enabled=" .. tostring(e.Enabled))
    local ok2 = pcall(function() e:Enable() end)
    say("[CS] after Enable ok=" .. tostring(ok2) .. " Enabled=" .. tostring(e.Enabled))
    local ok3 = pcall(function() e:Disconnect() end)
    task.wait(0.1)
    say("[CS] after Disconnect ok=" .. tostring(ok3) .. " listed=" .. tostring(#getconnections(nv.Changed)))
end
pcall(function() c:Disconnect() end)
nv:Destroy()
print("SUNCIO-DONE")
