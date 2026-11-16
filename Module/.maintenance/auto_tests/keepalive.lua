print("[KA] alive")
for i = 1, 24 do
    task.wait(2)
    if i % 5 == 0 then print("[KA] tick " .. i) end
end
print("SUNCIO-DONE")
