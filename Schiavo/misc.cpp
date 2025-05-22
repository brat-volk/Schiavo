#include "functions.h"

namespace misc {
    // PROPERLY convert wide string to UTF-8 without buffer overflows
    std::string WideStringToUTF8(const std::wstring& wstr) {
        int input_len = static_cast<int>(wstr.length());
        int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), input_len, nullptr, 0, nullptr, nullptr);
        if (utf8_size == 0) return "";

        std::string utf8(utf8_size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), input_len, &utf8[0], utf8_size, nullptr, nullptr);
        return utf8;
    }

    // Safely convert UTF-8 to wide string
    std::wstring UTF8ToWideString(const std::string& utf8) {
        int input_len = static_cast<int>(utf8.length());
        int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), input_len, nullptr, 0);
        if (wide_size == 0) return L"";

        std::wstring wstr(wide_size, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), input_len, &wstr[0], wide_size);
        return wstr;
    }

    // Updated JSON converters using safe conversions
    nlohmann::json WStringToJson(const std::wstring& wstr) {
        std::string utf8 = WideStringToUTF8(wstr);
        return nlohmann::json::parse(utf8);
    }

    std::wstring JsonToWString(const nlohmann::json& j) {
        return UTF8ToWideString(j.dump());
    }
}