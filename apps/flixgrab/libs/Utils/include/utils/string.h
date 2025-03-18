#pragma once

#include <stddef.h>  // For size_t.
#include <string.h>  // For memcpy.
#include <iostream>
#include <assert.h>
#include <cstdint>
#include <stdarg.h>

#include <sstream>
#include <vector>

namespace utils {


    //////////////////////////////////////////////////////////////////////////
    //String Utils

    // Note: if you need to add String16ToDouble, first ask yourself if it's
    // really necessary. If it is, probably the best implementation here is to
    // convert to 8-bit and then use the 8-bit version.

    // Note: if you need to add an iterator range version of StringToDouble, first
    // ask yourself if it's really necessary. If it is, probably the best
    // implementation here is to instantiate a string and use the string version.

    inline std::string HexEncode(const void* bytes, size_t size) {
        static const char kHexChars[] = "0123456789ABCDEF";

        // Each input byte creates two output hex characters.
        std::string ret(size * 2, '\0');

        for (size_t i = 0; i < size; ++i) {
            char b = reinterpret_cast<const char*>(bytes)[i];
            ret[(i * 2)] = kHexChars[(b >> 4) & 0xf];
            ret[(i * 2) + 1] = kHexChars[b & 0xf];
        }
        return ret;
    }

    //
    //inline std::string hexStr(const unsigned char *data, int len)
    //{
    //    const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
    //        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    //
    //    std::string s(len * 2, ' ');
    //    for (int i = 0; i < len; ++i) {
    //        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
    //        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    //    }
    //    return s;
    //}
    //
    inline std::string HexEncodeDelim(const unsigned char *data, int len) {
        const char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        std::string s(len * 2 + len - 1, ' ');
        for (int i = 0; i < len; ++i) {
            s[3 * i] = hexmap[(data[i] & 0xF0) >> 4];
            s[3 * i + 1] = hexmap[data[i] & 0x0F];
        }
        return s;
    }


    // ASCII-specific tolower.  The standard library's tolower is locale sensitive,
    // so we don't want to use it here.
    template <class Char> inline Char ToLowerASCII(Char c) {
        return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
    }

    // ASCII-specific toupper.  The standard library's toupper is locale sensitive,
    // so we don't want to use it here.
    template <class Char> inline Char ToUpperASCII(Char c) {
        return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
    }
    // Converts the elements of the given string.  This version uses a pointer to
    // clearly differentiate it from the non-pointer variant.
    template <class str> inline void StringToLowerASCII(str* s) {
        for (typename str::iterator i = s->begin(); i != s->end(); ++i)
            *i = ToLowerASCII(*i);
    }

    template <class str> inline str StringToLowerASCII(const str& s) {
        // for std::string and std::wstring
        str output(s);
        StringToLowerASCII(&output);
        return output;
    }

    // Converts the elements of the given string.  This version uses a pointer to
    // clearly differentiate it from the non-pointer variant.
    template <class str> inline void StringToUpperASCII(str* s) {
        for (typename str::iterator i = s->begin(); i != s->end(); ++i)
            *i = ToUpperASCII(*i);
    }

    template <class str> inline str StringToUpperASCII(const str& s) {
        // for std::string and std::wstring
        str output(s);
        StringToUpperASCII(&output);
        return output;
    }

    inline std::string IntToString(int value) {
        std::stringstream str; str << value;
        return str.str();
    }

    inline std::string JoinString(const std::vector<std::string>& parts,
        const std::string& sep) {
        if (parts.empty())
            return std::string();

        std::stringstream result;
        auto iter = parts.begin();

        result << *iter;
        ++iter;

        for (; iter != parts.end(); ++iter) {
            result << sep << *iter;
        }

        return result.str();
    }

    inline void SplitString(const std::string& str,
        const char delimiter, std::vector<std::string>& tokens) {
        if (str.empty())
            return;

        size_t start = 0;
        while (start != std::string::npos) {
            size_t end = str.find_first_of(delimiter, start);

            std::string piece;
            if (end == std::string::npos) {
                piece = str.substr(start);
                start = std::string::npos;
            }
            else {
                piece = str.substr(start, end - start);
                start = end + 1;
            }
            tokens.push_back(piece);
            /*if (whitespace == TRIM_WHITESPACE)
            piece = TrimString(piece, WhitespaceForType<Str>(), TRIM_ALL);

            if (result_type == SPLIT_WANT_ALL || !piece.empty())
            result.push_back(PieceToOutputType<Str, OutputStringType>(piece));*/
        }
    }


    //inline std::string  StringPrintf(const std::string& format, ...)
    //{
    //    va_list ap;
    //    va_start(ap, format);
    //    std::string result;
    //    char    buffer[1024];
    //    int sz = vsnprintf(buffer, 1024, format.c_str(), ap);
    //    result.assign(buffer, buffer + sz);
    //    va_end(ap);
    //    return result;
    //}

    inline std::string  StringPrintf(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        std::string result;
        char    buffer[1024];
        int sz = vsnprintf(buffer, 1024, format, ap);
        result.assign(buffer, buffer + sz);
        va_end(ap);
        return result;
    }

   /* std::wstring ASCIIToUTF16(const std::string& str);


    std::string UTF16ToASCII(const std::wstring& wstr);

*/

    //------------------------------------------------------------------------------
    // Convert form UTF-8 to UTF-16.
    // Throws Utf8ConversionException on conversion errors
    // (e.g. invalid UTF-8 sequence found in input string).
    //------------------------------------------------------------------------------
    std::wstring Utf8ToUtf16(const std::string& utf8);

    //------------------------------------------------------------------------------
    // Convert form UTF-16 to UTF-8.
    // Throws Utf8ConversionException on conversion errors
    // (e.g. invalid UTF-16 sequence found in input string).
    //------------------------------------------------------------------------------
    std::string Utf16ToUtf8(const std::wstring& utf16);
}
