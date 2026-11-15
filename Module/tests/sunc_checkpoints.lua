--!strict

assert(game.PlaceId == 90441122676618, "sUNC requires its test place")
print("SUNCIO-PLACE " .. string.format("%.0f", game.PlaceId))

getgenv().sUNCDebug = {
    printcheckpoints = true,
    delaybetweentests = 0,
    printtesttimetaken = true,
}

local ok, message = pcall(function()
    loadstring(game:HttpGet("https://script.sunc.io/"))()
end)
assert(ok, tostring(message))
print("SUNCIO-RETURN")
task.wait(35)
print("SUNCIO-DONE")
