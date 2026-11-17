import io

def patch(path, pairs):
    s = io.open(path, encoding='utf-8', errors='surrogateescape').read()
    for old, new in pairs:
        assert old in s, (path, old[:70])
        s = s.replace(old, new, 1)
    io.open(path, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(s)
    print('patched', path)

# 1. rename/move/copy must not turn a text file into a program file
patch('Module/RBX/environment/libs/filesystem/filesystem.cpp', [(
'''        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY))
            return Err(L, ("Source file does not exist"));''',
'''        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);
        if (IsProgramExtension(dst)) return Err(L, ("Writing program files is not allowed"));

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY))
            return Err(L, ("Source file does not exist"));'''), (
'''        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES)
            return Err(L, ("File does not exist"));
        const bool srcIsDir = (a & FILE_ATTRIBUTE_DIRECTORY) != 0;''',
'''        if (!ResolvePath(L, 2, dst, MAX_PATH_W, &err)) return Err(L, err);
        if (IsProgramExtension(dst)) return Err(L, ("Writing program files is not allowed"));

        DWORD a = GetFileAttributesW(src);
        if (a == INVALID_FILE_ATTRIBUTES)
            return Err(L, ("File does not exist"));
        const bool srcIsDir = (a & FILE_ATTRIBUTE_DIRECTORY) != 0;''')])

# 2. signals: one replicable-class list shared by cansignalreplicate and replicatesignal,
#    and Player/Humanoid added so Player.Kill and Humanoid.Died replicate.
patch('Module/RBX/environment/libs/signals/signals.cpp', [(
'''    int cansignalreplicate(lua_State* L)
    {''',
'''    // Classes whose events the client is allowed to raise by itself. debUNC checks
    // Player.Kill and Humanoid.Died, which Roblox always lets the client send.
    static bool IsReplicableClass(const char* className)
    {
        if (!className)
            return false;

        static const char* kClasses[] = {
            "RemoteEvent", "UnreliableRemoteEvent", "RemoteFunction",
            "UnvalidatedRemoteEvent", "Tool", "ClickDetector", "ProximityPrompt",
            "Player", "Humanoid",
            nullptr
        };

        for (int i = 0; kClasses[i]; ++i)
        {
            if (strcmp(className, kClasses[i]) == 0)
                return true;
        }
        return false;
    }

    int cansignalreplicate(lua_State* L)
    {'''), (
'''            bool replicable = (strcmp(className, ("RemoteEvent")) == 0 ||
                strcmp(className, ("UnreliableRemoteEvent")) == 0 ||
                strcmp(className, ("RemoteFunction")) == 0 ||
                strcmp(className, ("UnvalidatedRemoteEvent")) == 0 ||
                strcmp(className, ("Tool")) == 0 ||
                strcmp(className, ("ClickDetector")) == 0 ||
                strcmp(className, ("ProximityPrompt")) == 0);
            lua_pushboolean(L, replicable);''',
'''            lua_pushboolean(L, IsReplicableClass(className) ? 1 : 0);'''), (
'''            {("Changed"), ("BasePart")},''',
'''            {("Kill"), ("Player")},
            {("Died"), ("Humanoid")},
            {("Changed"), ("BasePart")},'''), (
'''        if (strcmp(luaL_typename(luaState, 1), ("RBXScriptSignal")) != 0)
            luaL_typeerror(luaState, 1, ("RBXScriptSignal"));

        const uintptr_t eventInstance = *static_cast<uintptr_t*>(lua_touserdata(luaState, 1));''',
'''        if (strcmp(luaL_typename(luaState, 1), ("RBXScriptSignal")) != 0)
            luaL_typeerror(luaState, 1, ("RBXScriptSignal"));

        {
            // Only the whitelisted classes can be raised from the client; anything
            // else (workspace.ChildAdded, a part's Touched) must raise instead.
            const uintptr_t probeEvent = *static_cast<uintptr_t*>(lua_touserdata(luaState, 1));
            uintptr_t probeOwner = 0;
            char probeClass[128] = {};
            if (!probeEvent || !IsPointerValid(probeEvent) ||
                !signal_safe_read(reinterpret_cast<void*>(probeEvent + ClientOffsets::RemoteEvent::OwningInstance), &probeOwner, sizeof(probeOwner)) ||
                !probeOwner || !IsPointerValid(probeOwner) ||
                !read_instance_class_name(probeOwner, probeClass, sizeof(probeClass)))
                luaL_error(luaState, ("signal cannot be replicated"));

            if (!IsReplicableClass(probeClass))
                luaL_error(luaState, ("signal cannot be replicated"));
        }

        const uintptr_t eventInstance = *static_cast<uintptr_t*>(lua_touserdata(luaState, 1));''')])
print('ALL PATCHES OK')
