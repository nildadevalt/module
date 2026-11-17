local say = print
say("[SP] start")
for _, name in ipairs({"debUNC.exe", "debUNC.EXE", "debUNC.exe.", "debUNC.exe ", "debUNC.exe::$DATA", "debUNC.txt.exe"}) do
    local ok, err = pcall(writefile, name, "x")
    task.wait(0.05)
    local exists = select(2, pcall(isfile, name))
    say("[SP] " .. name .. " writeOk=" .. tostring(ok) .. " isFile=" .. tostring(exists))
    pcall(delfile, name)
end
for _, pair in ipairs({{"a.txt", "b.exe"}, {"c.txt", "d.exe."}}) do
    pcall(writefile, pair[1], "x")
    local ok, err = pcall(renamefile, pair[1], pair[2])
    say("[SP] rename " .. pair[2] .. " ok=" .. tostring(ok))
    pcall(delfile, pair[1]); pcall(delfile, pair[2])
end
local ok, res = pcall(game.HttpGet, game, "https://awqbcduwendewbnzd.aaaaaa")
say("[SP] httpget ok=" .. tostring(ok) .. " type=" .. typeof(res) .. " val=" .. tostring(res):sub(1, 90))
print("SUNCIO-DONE")
