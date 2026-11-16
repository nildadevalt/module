print("[G2] start")
local ok, r = pcall(request, { Url = "https://httpbin.org/gzip", Method = "GET" })
print("[G2] ok=" .. tostring(ok) .. " err=" .. tostring(not ok and r) .. " type=" .. type(r))
if ok then print("[G2] status=" .. tostring(r.StatusCode) .. " bodylen=" .. tostring(r.Body and #r.Body)) end
print("SUNCIO-DONE")
