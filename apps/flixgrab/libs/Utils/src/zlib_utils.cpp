#include "utils/zlib_utils.h"

#ifdef _DEBUG
    #pragma comment (lib, "zlibd.lib")
#else
    #pragma comment (lib, "zlib.lib")
#endif

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <zlib.h>
#include "utils/zip.h"


#define TAR_HEADER_SIZE             512
#define BUFFER_LEN                  TAR_HEADER_SIZE * 1024 * 1024

#define FILENAME_LEN_MIN            5
#define FILEEXT_LEN                 3

#define TAR_H_OFFS_FILENAME         0
#define TAR_H_OFFS_FILEMODE         100
#define TAR_H_OFFS_OUSER_ID         108
#define TAR_H_OFFS_GUSER_ID         116
#define TAR_H_OFFS_FILESIZE         124
#define TAR_H_OFFS_MODTIME          136
#define TAR_H_OFFS_CHECKSUM         148
#define TAR_H_OFFS_LINKTYPE         156
#define TAR_H_OFFS_LINK             157

#define TAR_H_LEN_FILENAME         100
#define TAR_H_LEN_FILEMODE         8
#define TAR_H_LEN_OUSER_ID         8
#define TAR_H_LEN_GUSER_ID         8
#define TAR_H_LEN_FILESIZE         12
#define TAR_H_LEN_MODTIME          12
#define TAR_H_LEN_CHECKSUM         8
#define TAR_H_LEN_LINKTYPE         1
#define TAR_H_LEN_LINK             100

#define CHUNK 0x4000
#define windowBits 15
#define ENABLE_ZLIB_GZIP 32

extern "C" int mkpath(const char *path);

namespace zlib
{

struct tar_header_t {
    std::string fileName = "";
    size_t fileSize = 0;

    void copy(const char* const headerPtr) {
        memcpy(&_header, headerPtr, TAR_HEADER_SIZE);
        fileName.assign(&_header[TAR_H_OFFS_FILENAME]);

        fileSize = 0;
        memcpy(_fileSizeInOct, &_header[TAR_H_OFFS_FILESIZE], TAR_H_LEN_FILESIZE);
        sscanf(_fileSizeInOct, "%o", &fileSize);
    }

private:
    char _header[TAR_HEADER_SIZE] = { 0 };
    char _fileSizeInOct[TAR_H_LEN_FILESIZE] = { 0 };
};

int tarExtract(const std::string& fileName, const std::string &destPath) {
    size_t bytesRead = 0;
    size_t bytesWritten = 0;
    size_t fileDataOffset = TAR_HEADER_SIZE;
    size_t tarHeaderOffset = 0;

    bool fileRecording = false;
    bool parsingFinished = true;

    char* buffer = new char[BUFFER_LEN];

    tar_header_t tarH;

    gzFile gfile = gzopen(fileName.c_str(), "rb");

    do {
        if (parsingFinished)
            bytesRead = gzread(gfile, buffer, BUFFER_LEN);

        if (!fileRecording) {
            tarH.copy(&buffer[tarHeaderOffset]);

            if ((tarH.fileName == "") || (tarH.fileSize == 0))
                break;

            //std::experimental::filesystem::create_directory((destPath + "\\" + tarH.fileName.c_str()).c_str());
            std::string file = tarH.fileName.c_str();
            std::string file_name = file.substr(file.find_last_of("/") + 1, file.size());
            mkpath((destPath + file_name).c_str());
        }

        FILE* outFile;
        std::string file = tarH.fileName.c_str();
        std::string file_name = file.substr(file.find_last_of("/") + 1, file.size());

        if ((fileDataOffset + tarH.fileSize - bytesWritten) <= BUFFER_LEN) {
            outFile = fopen((destPath + file_name).c_str(), "a+b");
            size_t _bytesWritten = fwrite(&buffer[fileDataOffset], 1, tarH.fileSize - bytesWritten, outFile);
            fclose(outFile);

            if (_bytesWritten != (tarH.fileSize - bytesWritten)) {
                return -1;
            }

            fileRecording = false;

            tarHeaderOffset = ((fileDataOffset + tarH.fileSize - bytesWritten) / TAR_HEADER_SIZE) * TAR_HEADER_SIZE;
            if (tarHeaderOffset != (tarH.fileSize - bytesWritten))
                tarHeaderOffset += TAR_HEADER_SIZE;

            if (tarHeaderOffset >= bytesRead) {
                tarHeaderOffset = 0;
                parsingFinished = true;
            }
            else {
                parsingFinished = false;
            }

            fileDataOffset = tarHeaderOffset + TAR_HEADER_SIZE;
            if (fileDataOffset > BUFFER_LEN) {
                tarH.copy(&buffer[tarHeaderOffset]);
                parsingFinished = true;
                fileRecording = true;
            }

            bytesWritten = 0;

        }
        else {
            outFile = fopen((destPath + file_name).c_str(), "a+b");
            size_t _bytesWritten = fwrite(&buffer[fileDataOffset], 1, BUFFER_LEN - fileDataOffset, outFile);
            bytesWritten += _bytesWritten;
            fclose(outFile);

            fileDataOffset = 0;
            fileRecording = true;

            if (_bytesWritten != (BUFFER_LEN - fileDataOffset))
                return -1;

            parsingFinished = true;
        }

    } 
    while ((bytesRead == BUFFER_LEN) || !parsingFinished || fileRecording);
    gzclose(gfile);

    delete[] buffer;

    return 0;
}


static int on_extract_entry(const char *cFileName, void *arg) {
    std::string fileName;
    fileName.assign(cFileName);

    if (fileName.size() >= FILENAME_LEN_MIN) {
        char cFileExt[FILEEXT_LEN + 1] = { 0 };
        std::string fileExt;

        memcpy(cFileExt, &cFileName[strlen(cFileName) - FILEEXT_LEN], FILEEXT_LEN);
        fileExt.assign(cFileExt);

        if ((fileExt == "zip") || (fileExt == ".gz")) {
            std::string subfolder = fileName + ".FOLDER\\";
            //std::experimental::filesystem::create_directory(subfolder.c_str());
            mkpath(subfolder.c_str());

            if (fileExt == "zip") {
                int _arg = 0;
                zip_extract(fileName.c_str(), subfolder.c_str(), on_extract_entry, &_arg);
            }
            else {
                tarExtract(fileName.c_str(), subfolder.c_str());
            }
        }
    }

    return 0;
}

int zipExtract(const std::string& fileName, const std::string& destPath) {
    int arg = 0;
    int res = 0;

    res = zip_extract(fileName.c_str(), destPath.c_str(), on_extract_entry, &arg);
    return res;
}

int gzExtract(const std::string& fileName) {
    int bytesRead = 0;
    std::string tarFileName;

    if (fileName.size() >= FILENAME_LEN_MIN)
        std::copy(fileName.begin(), fileName.end() - 3, std::back_inserter(tarFileName));

    FILE* outFile;
    outFile = fopen(tarFileName.c_str(), "wb");

    char* buffer = new char[BUFFER_LEN];

    gzFile _gzFile = gzopen(fileName.c_str(), "rb");

    do {
        bytesRead = gzread(_gzFile, buffer, BUFFER_LEN);
        fwrite(buffer, 1, bytesRead, outFile);
    } while (bytesRead == BUFFER_LEN);

    fclose(outFile);

    gzclose(_gzFile);
    delete[] buffer;

    return 0;
}

int decompressData(void *pBytes, size_t cchSize, std::wstring& data) {
    z_stream strm = { 0 };
    unsigned char out[CHUNK + 1];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (unsigned char*)pBytes;
    strm.avail_in = cchSize;
    strm.next_out = out;

    int res = inflateInit2(&strm, windowBits | ENABLE_ZLIB_GZIP);
    if (res != Z_OK)
        return res;

    size_t cchWritten = CHUNK;
    std::wostringstream os;
    do
    {
        memset(out, 0, CHUNK + 1);
        strm.avail_out = CHUNK;
        strm.next_out = out;
        res = inflate(&strm, Z_NO_FLUSH);
        cchWritten += strm.total_out;
        os << (char*)out;
    } 
    while (res == Z_OK);

    inflateEnd(&strm);

    if (res == Z_STREAM_END) {
        res = Z_OK;
        data = os.str();
    }

    return res;
}
}