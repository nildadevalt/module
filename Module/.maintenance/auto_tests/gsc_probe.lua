print("[GS] start")
local found = nil
for _, m in ipairs(game:GetService("CoreGui").RobloxGui.Modules:GetChildren()) do
    if m:IsA("ModuleScript") then found = m break end
end
print("[GS] module=" .. tostring(found))
if found then
    local ok, gen = pcall(function() return getscriptclosure(found)() end)
    print("[GS] getscriptclosure ok=" .. tostring(ok) .. " type=" .. type(gen) .. " err=" .. tostring(not ok and gen))
    local ok2, orig = pcall(function() return getrenv().require(found) end)
    print("[GS] require ok=" .. tostring(ok2) .. " type=" .. type(orig))
end
print("[GS] getscriptclosure type=" .. type(getscriptclosure) .. " getscriptfunction=" .. type(getscriptfunction))
print("SUNCIO-DONE")
