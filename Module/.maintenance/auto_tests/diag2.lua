print("[D2] start")
local t0 = os.clock()

-- 1. big loadstring (triggers Yielding::YieldExecution path, >=16384 bytes)
task.spawn(function()
    local ok, f, err = pcall(function()
        return loadstring("return 1 --" .. string.rep("x", 17000))
    end)
    print("[D2] big-loadstring ok=" .. tostring(ok) .. " type=" .. type(f) .. " result=" .. tostring(f and f()) .. " err=" .. tostring(err))
end)

-- 2. small loadstring (sync path)
task.spawn(function()
    local f = loadstring("return 42")
    print("[D2] small-loadstring=" .. tostring(f and f()))
end)

-- 3. request (HTTP yield path)
task.spawn(function()
    local ok, res = pcall(request, { Url = "https://httpbin.org/user-agent", Method = "GET" })
    print("[D2] request ok=" .. tostring(ok) .. " status=" .. tostring(ok and res and res.StatusCode) .. " err=" .. tostring(not ok and res))
end)

-- 4. yield round-trip through task.wait
task.spawn(function()
    for i = 1, 3 do
        task.wait(0.5)
        print("[D2] wait tick " .. i)
    end
end)

task.delay(25, function()
    print("[D2] TIMEOUT-25 elapsed=" .. string.format("%.2f", os.clock() - t0))
    print("SUNCIO-DONE")
end)

print("[D2] spawned all")
