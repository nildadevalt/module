local savedGetfenv, savedGetgenv = getfenv, getgenv
local function inspect(label)
    local renv = getrenv()
    print("ENV-INSPECT", label, getfenv == renv.getfenv, savedGetfenv == getfenv, savedGetgenv == getgenv)
    print("ENV-OWN", label, getfenv(getgenv) == getgenv(), getfenv(0) == getgenv(), getfenv() == getgenv())
    print("ENV-NATIVE", label, renv.getfenv(getgenv) == getgenv(), renv.getfenv(0) == getgenv(), renv.getfenv() == getgenv())
    print("ENV-SAVED", label, savedGetfenv(savedGetgenv) == getgenv(), savedGetfenv(0) == getgenv())
end
inspect("before")
--!strict

assert(game.PlaceId == 90441122676618, "sUNC requires its test place")
print("SUNCIO-PLACE " .. string.format("%.0f", game.PlaceId))

getgenv().sUNCDebug = {
    printcheckpoints = false,
    delaybetweentests = 0,
    printtesttimetaken = false,
}

local ok, message = pcall(function()
    loadstring(game:HttpGet("https://script.sunc.io/"))()
end)
assert(ok, tostring(message))
print("SUNCIO-RETURN")
-- The suite reports results on its own threads after returning.
task.wait(35)
print("SUNCIO-DONE")

inspect("after")
print("ENV-INSPECT-DONE")
