-- Ubuntu's Executor Vulnerability Test (purplesstrat/UEVT)
-- Checks whether dangerous executor/Roblox endpoints are blocked.
--   ✅ = blocked (safe)   ⛔ = vulnerable   ⏺️ = unsupported
local url = "https://raw.githubusercontent.com/purplesstrat/UEVT/main/source"

local ok, src = pcall(function()
    return game:HttpGet(url)
end)

if not ok or type(src) ~= "string" or #src == 0 then
    warn("[VULN] fetch failed: " .. tostring(src))
    print("SUNCIO-DONE")
    return
end

print("[VULN] fetched " .. #src .. " bytes")

local fn, err = loadstring(src)
if not fn then
    warn("[VULN] loadstring failed: " .. tostring(err))
    print("SUNCIO-DONE")
    return
end

local ran, runErr = pcall(fn)
print("[VULN] finished ok=" .. tostring(ran) .. " err=" .. tostring(runErr))
print("SUNCIO-DONE")
