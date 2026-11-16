-- Local debUNC runner: reads the checked-out debUNC.luau off disk so the
-- suite does not depend on github being reachable from the game process.
local path = "debUNC.luau"

local ok, err = pcall(function()
    assert(isfile(path), "debUNC.luau not found next to the workspace folder")
    local source = readfile(path)
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
