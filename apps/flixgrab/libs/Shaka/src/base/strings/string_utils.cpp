#include "string_utils.h"
#include <windows.h>

std::wstring ASCIIToUTF16(const std::string& str) {    
    std::wstring wstr(str.size() + 1, 0);

    wstr.resize(MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), &wstr[0], wstr.size()));

    return wstr;
}

std::string UTF16ToASCII(const std::wstring& wstr) {
    std::string str(wstr.size() * 4, 0);
    str.resize(WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), &str[0], str.size(), NULL, NULL));
    
    return str;
}
