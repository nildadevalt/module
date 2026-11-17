local say = print
say("[BP4] start")
local b = buffer.create(4)
local ok, err = pcall(buffer.writestring, b, "ABCD", 0)
say("[BP4] writestring ok=" .. tostring(ok) .. " err=" .. tostring(err))
say("[BP4] tostring=[" .. buffer.tostring(b) .. "] len=" .. tostring(#buffer.tostring(b)))
local got = false
pcall(function()
    raknet.addsendhook(function(pkt)
        if got then return end
        got = true
        local s = buffer.tostring(pkt.AsBuffer)
        say("[BP4] pkt Size=" .. tostring(pkt.Size) .. " bufLen=" .. tostring(#s) .. " eq=" .. tostring(s == pkt.AsString))
        local n = math.min(#s, 8)
        local hex = {}
        for i = 1, n do hex[#hex+1] = string.format("%02X", string.byte(s, i)) end
        say("[BP4] bufHead=" .. table.concat(hex, " "))
    end)
end)
for i = 1, 10 do task.wait(0.4); if got then break end end
print("SUNCIO-DONE")
