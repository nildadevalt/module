local say = print
say("[BP] start")
local b = buffer.create(4)
buffer.writestring(b, "ABCD", 0)
say("[BP] basic eq=" .. tostring(buffer.tostring(b) == "ABCD") .. " got=" .. buffer.tostring(b) .. " len=" .. tostring(#buffer.tostring(b)))

local b2 = buffer.create(3)
buffer.writeu8(b2, 0, 65)
buffer.writeu8(b2, 1, 66)
buffer.writeu8(b2, 2, 67)
say("[BP] u8 eq=" .. tostring(buffer.tostring(b2) == "ABC") .. " got=" .. buffer.tostring(b2))
say("[BP] readstring=" .. tostring(buffer.readstring(b2, 0, 3)))
print("SUNCIO-DONE")
