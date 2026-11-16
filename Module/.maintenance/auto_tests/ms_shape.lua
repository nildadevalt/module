local API = loadstring(game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
print("[MSH] Standards type=" .. type(API.Standards))
for k, v in pairs(API.Standards) do
    print("[MSH] top " .. tostring(k) .. " = " .. type(v))
end
if type(API.Standards.Checks) == "table" then
    local n = 0
    for k, v in pairs(API.Standards.Checks) do
        n = n + 1
        if n <= 12 then print("[MSH] check " .. tostring(k) .. " = " .. type(v)) end
    end
    print("[MSH] total checks=" .. n)
end
print("SUNCIO-DONE")
