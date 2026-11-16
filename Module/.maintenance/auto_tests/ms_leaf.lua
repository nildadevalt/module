local API = loadstring(game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
local c = API.Standards.Checks.Actors.getactors
for k, v in pairs(c) do print("[MSL] field " .. tostring(k) .. " = " .. type(v)) end
print("[MSL] finished")
print("SUNCIO-DONE")
