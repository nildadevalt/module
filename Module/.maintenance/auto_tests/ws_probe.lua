print("[WP] start")
local urls = { "ws.postman-echo.com/raw", "wss://ws.postman-echo.com/raw", "ws://127.0.0.1:9999/nope" }
task.spawn(function()
    for _, u in ipairs(urls) do
        local t = os.clock()
        local target = u:find("://") and u or ("ws://" .. u)
        local ok, ws = pcall(WebSocket.connect, target)
        print("[WP] " .. target .. " ok=" .. tostring(ok) .. " type=" .. type(ws) .. " t=" .. string.format("%.1f", os.clock()-t) .. " err=" .. tostring(not ok and ws))
        if ok and type(ws) == "table" then pcall(function() ws:Close() end) end
    end
    print("SUNCIO-DONE")
end)
