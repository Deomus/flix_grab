#pragma once

#include <string>

namespace zlib {
    int             zipExtract(const std::string& fileName, const std::string& destPath);
    int             tarExtract(const std::string& fileName, const std::string& destPath);
    int             gzExtract(const std::string& fileName);
    int             decompressData(void *pBytes, size_t cchSize, std::wstring& data);
};
