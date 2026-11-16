-- debUNC automated runner for auto_sunc.py
local ok, err = pcall(function()
    local url = "https://raw.githubusercontent.com/alyssagithub/debUNC/main/debUNC.luau"
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
print("SUNCIO-DONE")
