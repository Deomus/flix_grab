#include "string.h"
#include <windows.h>
#include <string>

namespace utils {
    //std::wstring ASCIIToUTF16(const std::string& str) {
    //    std::wstring wstr(str.size() + 1, 0);

    //    wstr.resize(MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), &wstr[0], wstr.size()));

    //    return wstr;
    //}

    //std::string UTF16ToASCII(const std::wstring& wstr) {
    //    std::string str(wstr.size() * 4, 0);

    //    //int res = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size() * sizeof(wchar_t), &str[0], str.size(), NULL, NULL);
    //    //int res = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), &str[0], str.size(), NULL, NULL);
    //    //int res = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size() * sizeof(wchar_t), &str[0], 0, NULL, NULL);

    //    //HRESULT hr = GetLastError();

    //    str.resize(WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), &str[0], str.size(), NULL, NULL));

    //    return str;
    //}

    std::string Utf16ToUtf8(const std::wstring& utf16) {
        // Result of the conversion
        std::string utf8;

        // First, handle the special case of empty input string
        if (utf16.empty()) {
            _ASSERTE(utf8.empty());
            return utf8;
        }

        const int utf16Length = static_cast<int>(utf16.length());

        // Get the length, in chars, of the resulting UTF-8 string
        const int utf8Length = ::WideCharToMultiByte(
            CP_UTF8,            // convert to UTF-8
            0,             // conversion flags
            utf16.data(),       // source UTF-16 string
            utf16Length,        // length of source UTF-16 string, in wchar_ts
            nullptr,            // unused - no conversion required in this step
            0,                  // request size of destination buffer, in chars
            nullptr, nullptr    // unused
        );
        if (utf8Length == 0) {
            // Conversion error: capture error code and throw
            const DWORD error = ::GetLastError();
            _ASSERTE(utf8Length == 0);
            return utf8;
        }

        // Make room in the destination string for the converted bits
        utf8.resize(utf8Length);

        // Do the actual conversion from UTF-16 to UTF-8
        int result = ::WideCharToMultiByte(
            CP_UTF8,            // convert to UTF-8
            0,             // conversion flags
            utf16.data(),       // source UTF-16 string
            utf16Length,        // length of source UTF-16 string, in wchar_ts
            &utf8[0],           // pointer to destination buffer
            utf8Length,         // size of destination buffer, in chars
            nullptr, nullptr    // unused
        );
        if (result == 0) {
            // Conversion error: capture error code and throw
            const DWORD error = ::GetLastError();
            _ASSERTE(result == 0);
            return utf8;
        }

        return utf8;
    }

    std::wstring Utf8ToUtf16(const std::string& utf8) {
        // Result of the conversion
        std::wstring utf16;

        // First, handle the special case of empty input string
        if (utf8.empty()) {
            return utf16;
        }

        const int utf8Length = static_cast<int>(utf8.length());

        // Get the size of the destination UTF-16 string
        const int utf16Length = ::MultiByteToWideChar(
            CP_UTF8,       // source string is in UTF-8
            0,        // conversion flags
            utf8.data(),   // source UTF-8 string pointer
            utf8Length,    // length of the source UTF-8 string, in chars
            nullptr,       // unused - no conversion done in this step
            0              // request size of destination buffer, in wchar_ts
        );
        if (utf16Length == 0) {
            // Conversion error: capture error code and throw
            const DWORD error = ::GetLastError();
            _ASSERTE(utf16Length == 0);
            return utf16;

        }

        // Make room in the destination string for the converted bits
        utf16.resize(utf16Length);

        // Do the actual conversion from UTF-8 to UTF-16
        int result = ::MultiByteToWideChar(
            CP_UTF8,       // source string is in UTF-8
            0,        // conversion flags
            utf8.data(),   // source UTF-8 string pointer
            utf8Length,    // length of source UTF-8 string, in chars
            &utf16[0],     // pointer to destination buffer
            utf16Length    // size of destination buffer, in wchar_ts           
        );
        if (result == 0) {
            // Conversion error: capture error code and throw
            const DWORD error = ::GetLastError();
            //std::cout << "Error Utf8ToUtf16 " << error << std::endl;
            _ASSERTE(result == 0);
        }

        return utf16;
    }

}
