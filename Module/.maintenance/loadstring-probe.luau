local tag = "NERO_LOADSTRING_PROBE"
print(tag, "begin")
local fn, err = loadstring("return 'compiled_ok'")
assert(fn, err)
print(tag, "local", fn())
local ok, source = pcall(function()
    return game:HttpGet("https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source")
end)
print(tag, "http", ok, type(source), type(source) == "string" and #source or 0)
if not ok then
    warn(tag, source)
    return
end
local iy, compileError = loadstring(source, "IYCompatibility")
print(tag, "remote_compile", type(iy), compileError)
