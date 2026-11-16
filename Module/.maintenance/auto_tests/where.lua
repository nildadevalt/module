print("[W] listfiles('') ->")
local ok, files = pcall(listfiles, "")
if ok and type(files) == "table" then
    for i, f in ipairs(files) do if i <= 25 then print("[W]  " .. tostring(f)) end end
else
    print("[W] listfiles err=" .. tostring(files))
end
local ok2, w = pcall(function() return writefile("__probe.txt", "x") end)
print("[W] writefile ok=" .. tostring(ok2) .. " err=" .. tostring(w))
print("SUNCIO-DONE")
