local say = print
local GetInfo = debug.info
local function Inner(mode)
    if mode == "frames" then
        local out = {}
        for i = 1, 6 do
            local f = GetInfo(i, "f")
            if not f then break end
            out[#out+1] = f
        end
        return out
    end
    if mode == "traceback" then return debug.traceback() end
    if mode == "error" then error("debUNC") end
end
local function HiddenCaller(mode) return Inner(mode) end

say("[SSH] before hide: onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
say("[SSH] before hide: tb=" .. tostring(HiddenCaller("traceback"):find("HiddenCaller", 1, true)))
say("[SSH] before hide: err=" .. tostring(select(2, pcall(HiddenCaller, "error"))))

local ok, err = pcall(setstackhidden, HiddenCaller, true)
say("[SSH] set true ok=" .. tostring(ok) .. " err=" .. tostring(err))
say("[SSH] isstackhidden=" .. tostring(isstackhidden(HiddenCaller)))
say("[SSH] after hide: onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
say("[SSH] after hide: tb=" .. tostring(HiddenCaller("traceback"):find("HiddenCaller", 1, true)))
say("[SSH] after hide: err=" .. tostring(select(2, pcall(HiddenCaller, "error"))))
setstackhidden(HiddenCaller, false)
say("[SSH] after unhide: onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
say("SUNCIO-DONE")
