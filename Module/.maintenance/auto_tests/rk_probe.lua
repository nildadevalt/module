local say = print
say("[RK] start type(raknet)=" .. typeof(raknet))
local got = false
local ok, err = pcall(function()
    raknet.addsendhook(function(pkt)
        if got then return end
        got = true
        local parts = {
            "Size=" .. tostring(pkt.Size),
            "lenAsString=" .. tostring(#pkt.AsString),
            "AsBufferType=" .. typeof(pkt.AsBuffer),
        }
        if pkt.AsBuffer ~= nil then
            local ok2, s = pcall(buffer.tostring, pkt.AsBuffer)
            parts[#parts+1] = "bufOk=" .. tostring(ok2)
            parts[#parts+1] = "bufEqStr=" .. tostring(ok2 and s == pkt.AsString)
        end
        parts[#parts+1] = "bufferGlobal=" .. typeof(buffer)
        say("[RK] " .. table.concat(parts, " "))
    end)
end)
say("[RK] addhook ok=" .. tostring(ok) .. " err=" .. tostring(err))
for i = 1, 12 do
    task.wait(0.5)
    if got then break end
end
say("[RK] got=" .. tostring(got))
say("SUNCIO-DONE")
