print("[MD] start")
local t0 = os.clock()
local ok, src = pcall(function() return game:HttpGetAsync("https://github.com/Standard-2/Myriad/releases/latest/download/api.luau") end)
print("[MD] fetch ok=" .. tostring(ok) .. " type=" .. type(src) .. " len=" .. tostring(type(src)=="string" and #src))
if type(src) == "string" then
    local fn, err = loadstring(src)
    print("[MD] loadstring fn=" .. tostring(fn ~= nil) .. " err=" .. tostring(err))
    if fn then
        local ok2, api = pcall(fn)
        print("[MD] api ok=" .. tostring(ok2) .. " type=" .. type(api))
        if ok2 and type(api) == "table" then
            for k, v in pairs(api) do if type(k)=="string" then print("[MD] key " .. k .. "=" .. type(v)) end end
        else
            print("[MD] api err=" .. tostring(api))
        end
    end
end
print("[MD] t=" .. string.format("%.1f", os.clock()-t0))
print("SUNCIO-DONE")
