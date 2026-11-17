local say = print
say("[BP2] start typeof(buffer)=" .. typeof(buffer))
if typeof(buffer) ~= "table" then print("SUNCIO-DONE") return end
local names = {}
for k, v in pairs(buffer) do names[#names+1] = tostring(k) .. ":" .. typeof(v) end
table.sort(names)
say("[BP2] keys=" .. table.concat(names, ","))
local function try(label, fn)
    local ok, err = pcall(fn)
    say("[BP2] " .. label .. " ok=" .. tostring(ok) .. (ok and "" or (" err=" .. tostring(err))))
end
try("create", function() local b = buffer.create(4) say("[BP2] created type=" .. typeof(b)) end)
print("SUNCIO-DONE")
