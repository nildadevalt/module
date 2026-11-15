local tag = "NERO_IY_PROBE"
print(tag, "begin")
task.delay(12, function()
    local ok, result = pcall(function()
        local root = gethui()
        local titles = 0
        for index, item in ipairs(root:GetDescendants()) do
            if index > 100000 then break end
            if item:IsA("TextLabel") and string.find(item.Text, "Infinite Yield FE v", 1, true) then
                titles += 1
                local screen = item:FindFirstAncestorWhichIsA("ScreenGui")
                print(tag, "title", item.Text, "visible", item.Visible, "enabled", screen and screen.Enabled)
            end
        end
        return titles
    end)
    print(tag, "gui_check", ok, result)
end)
local ok, err = xpcall(function()
    loadstring(game:HttpGet("https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source"))()
end, function(err)
    return tostring(err) .. "\n" .. debug.traceback()
end)
print(tag, "command_returned", ok, err)
