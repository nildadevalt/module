--!strict

assert(game.PlaceId == 90441122676618, "Wrong test place")
print("SUNCIO-PLACE " .. string.format("%.0f", game.PlaceId))
local originalWarn
local captured = false
originalWarn = hookfunction(warn, function(...)
    local message = tostring(select(1, ...))
    if not captured and string.find(message, "generic namecall hook detected", 1, true) then
        captured = true
        print("SUNC-FAILURE-TRACE", debug.traceback())
        for level = 2, 7 do
            local fn, source, name = debug.info(level, "fsn")
            if not fn then break end
            print("SUNC-FAILURE-FRAME", level, source, name)
            if islclosure(fn) then
                local ok, constants = pcall(debug.getconstants, fn)
                if ok then
                    local count = 0
                    for index, value in pairs(constants) do
                        if type(value) == "string" and #value < 180 then
                            count += 1
                            if count > 64 then break end
                            print("SUNC-FAILURE-CONSTANT", level, index, value)
                        end
                    end
                end
            end
        end
    end
    return originalWarn(...)
end)
getgenv().sUNCDebug = {
    printcheckpoints = true,
    delaybetweentests = 0,
    printtesttimetaken = true,
}
local ok, message = pcall(function()
    loadstring(game:HttpGet("https://script.sunc.io/"))()
end)
restorefunction(warn)
assert(ok, tostring(message))
print("SUNCIO-RETURN")
task.wait(35)
print("SUNCIO-DONE")
