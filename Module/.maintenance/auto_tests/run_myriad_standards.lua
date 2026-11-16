-- Myriad Standards runner.
--
-- Myriad's release API describes checks as:
--   Checks[category][checkName] = { global = "name", cases = { { id, run }, ... } }
-- and each case returns { status = <code>, message = <text> }.
--
-- Standards.Run() awaits every case with no timeout and stalls on this build,
-- so walk the structure directly and bound each case with a hard timeout.
-- NOTE: the full 633-case sweep can terminate the client on this emulator
-- build; pass a category filter to run a subset.
local API = loadstring(game:HttpGetAsync(
    "https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()

local Checks = API.Standards.Checks
local only = shared.MYR_ONLY  -- optional category filter, e.g. "Actors"

local passed, failed, skipped, timedOut = 0, 0, 0, 0
local failures = {}

local function evaluate(category, name, fn)
    local finished, result = false, nil

    task.spawn(function()
        local ok, res = pcall(fn)
        result = ok and res or { status = 500, message = tostring(res) }
        finished = true
    end)

    local start = os.clock()
    while not finished and os.clock() - start < 6 do
        task.wait()
    end

    if not finished then
        timedOut += 1
        table.insert(failures, category .. "/" .. name .. " -> TIMEOUT")
        return
    end

    if type(result) ~= "table" or type(result.status) ~= "number" then
        skipped += 1
        return
    end

    if result.status == 200 then
        passed += 1
    elseif result.status == 400 or result.status == 404 then
        skipped += 1
    else
        failed += 1
        table.insert(failures, category .. "/" .. name .. " -> " ..
            tostring(result.status) .. ": " .. tostring(result.message))
    end
end

for category, group in pairs(Checks) do
    if only and tostring(category) ~= only then
        continue
    end
    if type(group) == "table" then
        for name, spec in pairs(group) do
            if type(spec) == "table" and type(spec.cases) == "table" then
                for index, case in ipairs(spec.cases) do
                    local fn = type(case) == "function" and case
                        or (type(case) == "table" and case.run)
                    local label = type(case) == "table" and tostring(case.id)
                        or ("#" .. tostring(index))
                    if type(fn) == "function" then
                        evaluate(tostring(category), tostring(name) .. "/" .. label, fn)
                    end
                end
            elseif type(spec) == "function" then
                evaluate(tostring(category), tostring(name), spec)
            end
        end
    end
end

print(string.format("MYR_STD_DONE passed=%d failed=%d skipped=%d timeouts=%d",
    passed, failed, skipped, timedOut))
for i, line in ipairs(failures) do
    if i > 80 then break end
    print("MYR_FAIL " .. line)
end
print("SUNCIO-DONE")
