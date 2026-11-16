-- debUNC runner with DebugMode so a crashing test is named instead of
-- taking the whole client down silently.
shared.DebugMode = true

local url = "https://raw.githubusercontent.com/alyssagithub/debUNC/main/debUNC.luau"

local ok, err = pcall(function()
    local source = game:HttpGet(url)
    local fn, loadErr = loadstring(source)
    if not fn then
        error("loadstring failed: " .. tostring(loadErr))
    end
    fn()
end)

if not ok then
    warn("[DEBUNC-RUNNER] error: " .. tostring(err))
end
print("[DEBUNC-RUNNER] finished")
print("SUNCIO-DONE")
