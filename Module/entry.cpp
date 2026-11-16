#include <Utilitys/Utils.hpp>
#include <Utilitys/Globals.hpp>
#include <Utilitys/Log.hpp>
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

        Raknet::InitializeHook(RakNet);

        while (task_scheduler::GetJobByTypeName("Net Peer Send") != 0) {

        }

        Raknet::ResetHook();
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

    // Disabled at startup to test whether RakPeer vtable hook / busy-wait triggers Error 268.
    // std::thread(raknetwatcher).detach();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // â”€â”€ Anti-debug: immediate check + watchdog thread â”€â”€
   

        DisableThreadLibraryCalls(hModule);

        // creates Logs.log next to Module.dll
  

        CrashHandler::Initialize();

        std::thread(MainThread, hModule).detach();
    }

    return TRUE;
}
