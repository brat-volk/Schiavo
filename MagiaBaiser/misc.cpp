#include "functions.h"


namespace misc {
    std::wstring JsonToWString(const nlohmann::json& j) {
        std::string utf8 = j.dump();
        int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
        std::wstring wstr(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], size);
        return wstr;
    }

    nlohmann::json WStringToJson(const std::wstring& wstr) {
        int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string utf8(size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], size, nullptr, nullptr);
        return nlohmann::json::parse(utf8);
    }
}