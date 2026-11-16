print("[SX] start")
local Value = math.random()
local CreatedSignal = Signal.new()
local Received
local OnceCount = 0
local Connection = CreatedSignal:Connect(function(...) Received = table.pack(...) end)
CreatedSignal:Once(function() OnceCount += 1 end)
CreatedSignal:Fire(Value, "debUNC")
Connection:Disconnect()
local ReceivedAfterFire = Received
Received = nil
CreatedSignal:Fire(Value)
task.delay(0.1, function() CreatedSignal:Fire(Value) end)
print("[SX] checks1=" .. tostring(ReceivedAfterFire and ReceivedAfterFire[1] == Value and ReceivedAfterFire[2] == "debUNC"))
print("[SX] once=" .. tostring(OnceCount))
local t0 = os.clock()
local w = CreatedSignal:Wait()
print("[SX] wait=" .. tostring(w) .. " t=" .. string.format("%.2f", os.clock()-t0))
print("SUNCIO-DONE")
