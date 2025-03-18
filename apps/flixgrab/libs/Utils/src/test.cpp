
#include <utils/http.h>
#include <utils/http_file.h>
#include <utils/string.h>

#include <fstream>
#include <iostream>

//QUrl url("http://video.playground.ru/178893/md/178893.mp4");
//QUrl url("https://www.youtube.com/watch?v=G6WGUbnBLWw");

void test_continuous_download(const char* url, const char* file_name) {
    utils::Http http;

    utils::Status status = http.Open(url);

    utils::HttpHeaders headers;

    status = http.Get(0, -1, &headers);

    std::ofstream file(file_name, std::ios_base::binary);

    std::vector<char>  buffer;
    buffer.resize(1024 * 1024);
    uint32_t length = buffer.size();
    uint64_t stored_bytes = 0;
    while ((status = http.Read(buffer.data(), &length)).ok() && length > 0) {
        file.write(buffer.data(), length);
        stored_bytes += length;
        
        std::cout << "Readed " << stored_bytes << " Content " << headers.ContentLength() << " Content-Range " << headers.ContentRangeLength() << std::endl;
        std::cout << "Status Code " << headers.status_code()  << std::endl;
    }
}

void test_chunked_download(const char* url, const char* file_name) {
    utils::Http http;

    utils::Status status = http.Open(url);

    utils::HttpHeaders headers;
      
    std::ofstream file(file_name, std::ios_base::binary);

    std::vector<char>  buffer;
    buffer.resize(1024 * 1024);
    uint32_t length = buffer.size();
    uint64_t stored_bytes = 0;
    while ((status = http.Get(stored_bytes, buffer.size(), &headers) ).ok() && length > 0) {
        std::cout << "Content-Length" << headers.RawHeader("Content-Length") << std::endl;
        length = buffer.size();
        status = http.Read(buffer.data(), &length);
        if (status.ok()) {
            file.write(buffer.data(), length);
            stored_bytes += length;
            std::cout << "Readed " << stored_bytes << " Content " << headers.ContentLength() << " Content-Range " << headers.ContentRangeLength() << std::endl;

            std::cout << "Status Code " << headers.status_code() << " Response " << headers.response() << " Content-Range " << headers.RawHeader("Content-Range") << std::endl;
            if (buffer.size() > length)
                break;
        }
        
    }
}


void test_http_file(const char* url, const char* file_name, bool partial = false) {

    //std::ofstream fs(file_name, std::ios_base::trunc | std::ios_base::binary);
       
    utils::HttpFile  file(file_name, utils::HttpFile::Truncate);

    utils::Status status = file.Open(url, partial, false);

    while (status.ok()) {
        status = file.Download();
        std::cout << "Received: " << file.total_received() << " Content Size: " << file.total_length() << std::endl;

        if (!status) {
            int a = 0;
        }
    }
    std::cout << "Finished!" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string fn = utils::Utf16ToUtf8(L"русский.mp4");

    std::wstring wfn = utils::Utf8ToUtf16(fn);

    //test_http_file("http://10.255.255.255", fn.c_str(), true);
    test_http_file("http://video.playground.ru/178893/md/178893.mp4", fn.c_str(), true);

    //test_http_file("https://www.youtube.com/watch?v=G6WGUbnBLWw", "yt.htm", true);
    //test_chunked_download("http://video.playground.ru/178893/md/178893.mp4", "chunked.mp4");
    //test_continuous_download("http://video.playground.ru/178893/md/178893.mp4", "chunked.mp4");
    return 0;
}
