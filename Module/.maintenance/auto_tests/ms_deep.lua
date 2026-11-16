local API = loadstring(game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
local C = API.Standards.Checks
local n = 0
for cat, g in pairs(C) do
    n = n + 1
    if n <= 3 then
        print("[MSD] cat=" .. tostring(cat) .. " gtype=" .. type(g))
        if type(g) == "table" then
            local m = 0
            for k, v in pairs(g) do
                m = m + 1
                if m <= 4 then print("[MSD]   " .. tostring(k) .. " = " .. type(v)) end
            end
            print("[MSD]   total=" .. m)
        end
    end
end
print("[MSD] cats=" .. n)
print("SUNCIO-DONE")
