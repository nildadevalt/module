-- Luarmor Compatibility Tester (instrumented for automated capture)
local console
local tests
local function render_console()
    local lines = {}
    lines[#lines+1] = "Luarmor Compatibility Tester"
    lines[#lines+1] = ""
    local okId, ex = pcall(identifyexecutor)
    lines[#lines+1] = "Executor: " .. tostring(okId and ex or "unknown")
    for i, v in ipairs(tests) do
        local pass = v.success == false and "⛔" or v.success == true and "✅" or "-"
        lines[#lines+1] = tostring(v.name) .. " " .. pass .. " " .. tostring(v.result)
    end
    local text = table.concat(lines, "\n")
    if console and console.Text ~= nil then
        console.Text = text
    end
    return text
end
local function log_state(tag)
    local text = render_console()
    for line in string.gmatch(text, "[^\n]+") do
        print("[LUARMOR] " .. line)
    end
    pcall(function() writefile("luarmor_compat_result.txt", text) end)
end
local function json_decode(str)
    return game:GetService("HttpService"):JSONDecode(str)
end
local sr_resp, gg_resp
tests = {
    {
        name = "Has custom http func",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            self.result = "request()"
            self.success = true
            if syn and syn.request then
                self.result = "syn.request()"
            elseif not (request or http_request or (syn and syn.request)) then
                self.result = "func cant be found"
                self.success = false
            end
        end
    },
    {
        name = "Has custom WS func",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local ws =
                syn and syn.websocket and syn.websocket.connect or WebSocket and WebSocket.connect or WebsocketClient
            if ws then
                self.result = tostring(ws)
                self.success = true
            else
                self.result = "func cant be found"
                self.success = false
            end
        end
    },
    {
        name = "Custom httpfunc user-agent",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local req = request or http_request or (syn and syn.request)
            if not req then error("no request func") end
            sr_resp = req({Url = "https://httpbin.org/get", Method = "GET"}).Body
            self.result = json_decode(sr_resp).headers["User-Agent"]
            self.success = true
        end
    },
    {
        name = "game.httpget user-agent",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            gg_resp = game:HttpGet("https://httpbin.org/get")
            self.result = json_decode(gg_resp).headers["User-Agent"]
            self.success = true
        end
    },
    {
        name = "httpget roblox-session-id header",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local obj = json_decode(gg_resp).headers["Roblox-Session-Id"]
            if obj then
                self.result = "exists"
                self.success = true
            else
                self.result = "not present"
                self.success = false
            end
        end
    },
    {
        name = "roblox-session-id.gameid field",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local obj = json_decode(gg_resp).headers["Roblox-Session-Id"]
            if obj then
                obj = json_decode(obj)
                if obj.GameId then
                    self.result = tostring(obj.GameId)
                    self.success = true
                else
                    self.result = "not present"
                    self.success = false
                end
            else
                self.result = "not present"
                self.success = false
            end
        end
    },
    {
        name = "httpget roblox-game-id header",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local obj = json_decode(gg_resp).headers["Roblox-Game-Id"]
            if obj then
                self.result = obj
                self.success = true
            else
                self.result = "not present"
                self.success = false
            end
        end
    },
    {
        name = "raw headers",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local obj = game:GetService("HttpService"):JSONEncode(json_decode(sr_resp).headers)
            self.result = tostring(obj)
            self.success = true
        end
    },
    {
        name = "test luarmor script",
        result = "(pending...)",
        success = nil,
        run_task = function(self)
            local noclose = self
            getgenv().callback = function()
                noclose.success = true
                noclose.result = "Success (callback)"
                print("[LUARMOR] callback fired")
                render_console()
                pcall(function() writefile("luarmor_compat_result.txt", console and console.Text or "") end)
            end
            if lrm_load_script then
                lrm_load_script("f42f3746fb3eb60f837d3673581c14a5")
                self.result = "loaded via lrm_load_script (waiting callback)"
                return
            end
            local ok,err = pcall(function()
                loadstring(game:HttpGet("https://api.luarmor.net/files/v3/loaders/f42f3746fb3eb60f837d3673581c14a5.lua"))()
            end)
            if ok then
                self.result = "loader executed (waiting callback)"
            else
                self.result = "loader error: " .. tostring(err)
                self.success = false
            end
        end
    }
}

-- Try to create GUI (best-effort, like original)
pcall(function()
    local mouse = game:GetService("Players").LocalPlayer:GetMouse()
    local inputService = game:GetService("UserInputService")
    local ScreenGui = Instance.new("ScreenGui")
    local Frame = Instance.new("Frame")
    local ImageLabel = Instance.new("ImageLabel")
    local TextBox = Instance.new("TextBox")
    local UICorner = Instance.new("UICorner")

    ScreenGui.Parent = game:GetService("CoreGui")
    ScreenGui.Name = "LuarmorTestUtil"
    ScreenGui.ZIndexBehavior = Enum.ZIndexBehavior.Sibling

    Frame.Parent = ScreenGui
    Frame.BackgroundColor3 = Color3.fromRGB(62, 181, 255)
    Frame.BackgroundTransparency = 1.000
    Frame.BorderColor3 = Color3.fromRGB(27, 42, 53)
    Frame.BorderSizePixel = 0
    Frame.Position = UDim2.new(0.4093418935, 0, 0.405985038, 0)
    Frame.Size = UDim2.new(0, 414, 0, 489)

    ImageLabel.Parent = Frame
    ImageLabel.BackgroundColor3 = Color3.fromRGB(255, 255, 255)
    ImageLabel.BackgroundTransparency = 1.000
    ImageLabel.Position = UDim2.new(-0.0261243284, 0, -0.0164751261, 0)
    ImageLabel.Size = UDim2.new(0, 424, 0, 505)
    ImageLabel.Image = "rbxassetid://12648173604"

    TextBox.Parent = ImageLabel
    TextBox.BackgroundColor3 = Color3.fromRGB(255, 255, 255)
    TextBox.BackgroundTransparency = 0.900
    TextBox.Position = UDim2.new(0.0895741284, 0, 0.163428098, 0)
    TextBox.Size = UDim2.new(0, 352, 0, 373)
    TextBox.Font = Enum.Font.Roboto
    TextBox.Text = ""
    TextBox.TextColor3 = Color3.fromRGB(200, 200, 200)
    TextBox.TextSize = 14.000
    TextBox.TextWrapped = true
    TextBox.TextYAlignment = Enum.TextYAlignment.Top
    TextBox.TextXAlignment = Enum.TextXAlignment.Left
    TextBox.RichText = false
    TextBox.TextEditable = false
    TextBox.ClearTextOnFocus = false

    UICorner.CornerRadius = UDim.new(0.0299999993, 0)
    UICorner.Parent = TextBox
    console = TextBox
end)

render_console()
task.wait(1)
for i, v in ipairs(tests) do
    local ok, err = pcall(function() v:run_task() end)
    if not ok then
        v.success = false
        v.result = "error: " .. tostring(err)
    end
    task.wait(0.1)
    render_console()
end
print("[LUARMOR] TESTS_DONE")
log_state("final")
task.delay(10, function()
    print("[LUARMOR] POST_WAIT")
    log_state("post")
end)
