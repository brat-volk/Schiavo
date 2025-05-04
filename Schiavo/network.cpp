#include "functions.h"


namespace network{
    std::wstring MakeHttpRequest(const std::wstring& host, const std::wstring& path, const std::wstring& data, const std::wstring& method) {
        HINTERNET hSession = WinHttpOpen(0,
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return L"";

        HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(),
            INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (!hConnect) {
            WinHttpCloseHandle(hSession);
            return L"";
        }

        DWORD flags = WINHTTP_FLAG_SECURE | WINHTTP_FLAG_REFRESH;
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, method.c_str(), path.c_str(),
            nullptr, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            flags);
        LPCWSTR headers = L"Content-Type: application/json\r\n"
            L"Accept: application/json\r\n";

        if (WinHttpAddRequestHeaders(hRequest, headers, -1, WINHTTP_ADDREQ_FLAG_ADD) == FALSE) {
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return L"";
        }

        WinHttpAddRequestHeaders(hRequest,
            L"Content-Type: application/json\r\n"
            L"Cache-Control: no-cache\r\n",
            -1L, WINHTTP_ADDREQ_FLAG_ADD);

        std::wstring response;
        if (WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            (LPVOID)data.c_str(), data.size() * sizeof(wchar_t),
            data.size() * sizeof(wchar_t), 0)) {
            WinHttpReceiveResponse(hRequest, nullptr);

            DWORD size = 0;
            DWORD downloaded = 0;
            do {
                size = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &size)) break;

                if (size) {
                    std::vector<wchar_t> buffer(size / sizeof(wchar_t) + 1);
                    if (WinHttpReadData(hRequest, &buffer[0], size, &downloaded)) {
                        response.append(buffer.begin(), buffer.begin() + (downloaded / sizeof(wchar_t)));
                    }
                }
            } while (size > 0);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return response;
    }
}