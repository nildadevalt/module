print("[S4] start")
task.spawn(function()
    local s = Signal.new()
    local pack = nil
    local c = s:Connect(function(...) pack = table.pack(...) end)
    task.delay(0.1, function() print("[S4] firing"); s:Fire("v") end)
    local n = 0
    repeat task.wait() n = n + 1 until pack ~= nil or n > 200
    print("[S4] done n=" .. n .. " pack=" .. tostring(pack and pack[1]))
    c:Disconnect()
    print("SUNCIO-DONE")
end)
print("[S4] spawned")
