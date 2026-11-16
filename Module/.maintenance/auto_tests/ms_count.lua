local API = loadstring(game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
local C = API.Standards.Checks
print("[MC] C type=" .. type(C))
local cats, checks, cases = 0, 0, 0
for cat, g in pairs(C) do
    cats = cats + 1
    for name, spec in pairs(g) do
        checks = checks + 1
        if type(spec) == "table" and type(spec.cases) == "table" then
            for _ in pairs(spec.cases) do cases = cases + 1 end
        end
    end
end
print("[MC] cats=" .. cats .. " checks=" .. checks .. " cases=" .. cases)
print("SUNCIO-DONE")
