#pragma once

// Shim: provides the old Renderer::AddConsoleLog signature
// that TPHandler.hpp and task_scheduler.cpp still reference.
// Routes output to OutputDebugStringA + the new graphics log_mgr.

#include <cstdio>
#include <managers/logging/logging.h>


namespace Renderer
{
    inline void AddConsoleLog(const char* level, const char* source, const char* message)
    {
        char buf[1024];
        snprintf(buf, sizeof(buf), ("[%s] %s: %s"), level, source, message);
#ifdef _DEBUG
        OutputDebugStringA(buf);
        OutputDebugStringA(("\n"));
#endif
        log_mgr.add_log(buf);
    }

    inline void Initialize() {}
}
