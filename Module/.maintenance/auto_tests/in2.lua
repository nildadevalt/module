print("[I2] start")
local hwnd = nil
print("[I2] attempting focus")
local ok = pcall(function()
    local ffi = nil
end)
keypress(0x7C)
task.wait(0.5)
print("[I2] F13=" .. tostring(game:GetService("UserInputService"):IsKeyDown(Enum.KeyCode.F13)))
keyrelease(0x7C)
print("SUNCIO-DONE")
