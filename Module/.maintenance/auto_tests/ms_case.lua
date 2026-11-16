local API = loadstring(game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
local spec = API.Standards.Checks.Actors.getactors
print("[MSC] spec type=" .. type(spec) .. " cases type=" .. type(spec.cases) .. " n=" .. tostring(#spec.cases))
local i = 0
for k, v in pairs(spec.cases) do
    i = i + 1
    if i <= 3 then
        print("[MSC] case key=" .. tostring(k) .. " type=" .. type(v))
        if type(v) == "table" then
            for kk, vv in pairs(v) do print("[MSC]   ." .. tostring(kk) .. " = " .. type(vv)) end
        end
    end
end
print("SUNCIO-DONE")
