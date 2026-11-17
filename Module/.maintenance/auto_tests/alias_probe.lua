local say = print
local names = {"write_file","read_file","is_file","make_folder","do_file","get_signal_arguments",
  "get_signal_whitelist","mouse1_click","mouse_move_abs","key_press","is_cclosure","is_lclosure",
  "new_c_closure","hook_metamethod","hook_signal","get_script_bytecode","get_script_hash",
  "get_loaded_modules","get_tenv","set_clipboard","get_clipboard","set_readonly","make_readonly",
  "fire_touch_interest","get_connections","save_instance","replicate_signal","set_fps_cap","get_fps_cap",
  "check_caller","get_gc","get_hui","get_raw_metatable","set_raw_metatable","compare_instances","lz4_compress"}
local missing = {}
for _, n in ipairs(names) do
  if typeof(getgenv()[n]) ~= "function" then missing[#missing+1] = n end
end
say("[AL] aliases missing: " .. (#missing == 0 and "none" or table.concat(missing, ",")))
say("[AL] write_file works: " .. tostring(pcall(write_file, "alias_probe.txt", "ok")))
say("[AL] read_file -> " .. tostring(pcall(read_file, "alias_probe.txt")))
pcall(delfile, "alias_probe.txt")
print("SUNCIO-DONE")
