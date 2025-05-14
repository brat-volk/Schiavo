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
    std::wstring GetWinVersion() {
        std::wstring WindowsVersion = L"Unknown";
        PBYTE WKSTAPointer;
        WKSTA_INFO_100 WKSTABuf;
        NetWkstaGetInfoW(NULL, 100, &WKSTAPointer);
        memcpy(&WKSTABuf, WKSTAPointer, sizeof(WKSTABuf));
        WindowsVersion = IsWindowsXPOrGreater() ? "XP " : WindowsVersion;
        WindowsVersion = IsWindows7OrGreater() ? "7 " : WindowsVersion;
        WindowsVersion = IsWindows7SP1OrGreater() ? "7 SP1 " : WindowsVersion;
        WindowsVersion = IsWindows8OrGreater() ? "8 " : WindowsVersion;
        WindowsVersion = IsWindows8Point1OrGreater() ? "8.1 " : WindowsVersion;
        WindowsVersion = IsWindows10OrGreater() ? "10 " : WindowsVersion;
        WindowsVersion = IsWindowsServer() ? "Server " : WindowsVersion;
        WindowsVersion += WKSTABuf.wki100_ver_major;
        WindowsVersion += ".";
        WindowsVersion += WKSTABuf.wki100_ver_minor;
    }
}