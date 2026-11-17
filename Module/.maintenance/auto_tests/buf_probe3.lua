local say = print
say("[BP3] start")
local b = buffer.create(4)
local function try(label, fn)
    local ok, res = pcall(fn)
    say("[BP3] " .. label .. " ok=" .. tostring(ok) .. " res=" .. tostring(res))
end
try("writestring", function() return buffer.writestring(b, "ABCD", 0) end)
try("len", function() return buffer.len(b) end)
try("tostring", function() return buffer.tostring(b) end)
try("readstring", function() return buffer.readstring(b, 0, 4) end)
try("writeu8", function() return buffer.writeu8(b, 0, 90) end)
try("tostring2", function() return buffer.tostring(b) end)
print("SUNCIO-DONE")
