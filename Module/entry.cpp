#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>
#include <Utilitys/Log.hpp>
#include <Utilitys/ModuleLocation.hpp>
#include <Utilitys/Communication/Communication.hpp>
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <Utilitys/TPHandler.hpp>
#include <Renderer/graphics.h>
#include <RBX/environment/libs/raknet/raknet.hpp>
#include <CrashHandler.hpp>

#include <filesystem>
#include <thread>
#include <windows.h>

void raknetwatcher() {
    while (true) {
        uintptr_t RakNet = 0;
        while (!(RakNet = task_scheduler::GetJobByTypeName("Net Peer Send"))) {
            Sleep(250);
        }

        // The peer pointer is filled in shortly after the job shows up, so give
        // the client a moment before reading it.
        Sleep(1000);
        Raknet::InitializeHook(RakNet);

        // Poll instead of spinning: a tight loop here burns a whole core and
        // races the client's own job list updates.
        while (task_scheduler::GetJobByTypeName("Net Peer Send") == RakNet) {
            Sleep(500);
        }
        Raknet::ResetHook();
        Sleep(500);
    }
}


void MainThread(HMODULE hModule)
{
    LOGF("Module", "Execution diagnostics build started; pid=%lu", GetCurrentProcessId());
    char appdata[MAX_PATH] = {};
    if (GetEnvironmentVariableA(("APPDATA"), appdata, MAX_PATH) && appdata[0] != '\0')
        Shared::workspace_folder = std::filesystem::path(appdata) / GenerateRandomName(10) / ("workspace");
    else
        Shared::workspace_folder = std::filesystem::path(("workspace"));

    {
        std::error_code ec;
        if (!std::filesystem::exists(Shared::workspace_folder, ec))
            std::filesystem::create_directories(Shared::workspace_folder, ec);
    }

    Communication::Initialize();

    TpHandler::Initialize();

    module::core::graphics::graphics->initialize();

    // The RakPeer hook now restores the original vtable before it gives up and
    // polls instead of spinning, so it can run again.
    std::thread(raknetwatcher).detach();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // â”€â”€ Anti-debug: immediate check + watchdog thread â”€â”€

        // Remember where this module lives before anything can hide it from the
        // loader lists (the emulator does that right after loading). Without
        // this the module cannot find its own folder, so Logs.log and the
        // bundled decompiler.exe are never located.
        ModuleLocation::Capture(hModule);

        DisableThreadLibraryCalls(hModule);

        // creates Logs.log next to Module.dll
  

        CrashHandler::Initialize();

        std::thread(MainThread, hModule).detach();
    }

    return TRUE;
}
