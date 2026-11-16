print("[HID] start")
local raw_info, raw_err, raw_tb = debug.info, error, debug.traceback
local ishidden = isstackhidden
-- identity check only: ishidden takes a *function*, so it is immune to the
-- frame shifts that made level-based queries unreliable.
local function hid(fn) return fn ~= nil and ishidden(fn) == true end

local function info(level, opts)
    if typeof(level) ~= "number" or level < 0 then return raw_info(level, opts) end
    if level == 0 then return raw_info(0, opts) end
    local seen = 0
    for a = 1, 250 do
        local fn = raw_info(a, "f")
        if fn == nil then return nil end
        if not hid(fn) then
            seen = seen + 1
            if seen == level then return raw_info(a, opts) end
        end
    end
    return nil
end

local function error_fn(msg, level)
    if typeof(level) ~= "number" or level >= 1 then
        for a = 1, 250 do
            local fn = raw_info(a, "f")
            if fn == nil then break end
            if hid(fn) then return raw_err(msg, 0) end
        end
    end
    return raw_err(msg, level)
end

local function traceback(msg, level)
    local any = false
    for a = 1, 250 do
        local fn = raw_info(a, "f")
        if fn == nil then break end
        if hid(fn) then any = true break end
    end
    if not any then return raw_tb(msg, level) end
    local out = typeof(msg) == "string" and (msg .. "\n") or ""
    for a = 1, 250 do
        local fn = raw_info(a, "f")
        if fn == nil then break end
        if not hid(fn) then
            local s = raw_info(a, "sl")
            if s and s ~= "" then out = out .. s .. "\n" end
        end
    end
    return out
end

pcall(function() setreadonly(debug, false) end)
debug.info = info
debug.traceback = traceback
pcall(function() setreadonly(debug, true) end)
pcall(function() setreadonly(getfenv(0), false) end)
getfenv(0).error = error_fn
pcall(function() setreadonly(getfenv(0), true) end)
print("[HID] installed=" .. tostring(debug.info == info) .. " err=" .. tostring(getfenv(0).error == error_fn))

local function Inner(mode)
    if mode == "frames" then
        local out = {}
        for i = 1, 6 do local f = debug.info(i, "f"); if not f then break end; out[#out+1] = f end
        return out
    end
    if mode == "tb" then return debug.traceback("x") end
    if mode == "error" then error("debUNC") end
end
local function HiddenCaller(mode) return Inner(mode) end
print("[HID] before onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
setstackhidden(HiddenCaller, true)
print("[HID] after onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
print("[HID] after err=" .. tostring(select(2, pcall(HiddenCaller, "error"))))
print("[HID] after tb=" .. tostring(HiddenCaller("tb"):find("HiddenCaller", 1, true)))
setstackhidden(HiddenCaller, false)
print("[HID] unhide onstack=" .. tostring(table.find(HiddenCaller("frames"), HiddenCaller)))
print("[HID] unhide err=" .. tostring(select(2, pcall(HiddenCaller, "error"))))
print("SUNCIO-DONE")
