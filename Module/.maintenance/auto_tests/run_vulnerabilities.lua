-- Myriad Vulnerabilities check.
--
-- Findings: the public Myriad release API exposes only two suites —
--   Standards.Checks   (19 categories / 140 checks / 633 cases)  <- real tests
--   Vulnerabilities    ({ [200]=.., [400]=.., [404]=.. })         <- legend only
-- "Vulnerabilities" is a status-code legend used to label Standards results, it
-- contains no runnable checks. The real vulnerabilities suite ships inside the
-- official testing place (79035306837882), which is not joinable from here, so
-- this reports the limitation instead of fabricating a pass/fail count.
local ok, API = pcall(function()
    return loadstring(game:HttpGetAsync(
        "https://github.com/Standard-2/Myriad/releases/latest/download/api.luau"))()
end)

local verdict = "unavailable"
local detail = "API did not load"

if ok and type(API) == "table" and type(API.Standards) == "table" then
    local vuln = API.Vulnerabilities
    local runnable = 0

    if type(vuln) == "table" then
        for _, value in pairs(vuln) do
            if type(value) == "function" then
                runnable += 1
            elseif type(value) == "table" and (value.run or value.cases) then
                runnable += 1
            end
        end
    end

    if runnable > 0 then
        verdict = "runnable"
        detail = runnable .. " runnable vulnerability checks"
    else
        detail = "Vulnerabilities is a status-code legend only; no runnable checks"
    end
end

print(string.format("MYR_VULN_DONE verdict=%s detail=%s", verdict, detail))
print("SUNCIO-DONE")
