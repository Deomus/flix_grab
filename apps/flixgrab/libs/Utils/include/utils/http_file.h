#pragma once
#include "http.h"
#include "status.h"

#include <fstream>

namespace utils {

    class HttpFile {
    public:
        enum Mode {
            Truncate,
            Append,     // append multiple web files to local file
            Continue,   // continue interrupted download
        };
    public:
        //COMMENT: file_name can be UTF8 string;
        HttpFile(const char* file_name, Mode mode = Truncate);
        ~HttpFile();

    public:
        Status          Open(const char* url, bool chunked_requests = false, bool request_header = false, long remote_file_pos = -1);
        //Download block
        Status          Download();

        void            Close();

        const char*     url() const;

    public:
        void            set_buffer_size(size_t size) { buffer_size_ = size; }
        size_t          buffer_size() const { return buffer_size_; }

        void            set_chunk_size(size_t chunk) { chunk_size_ = chunk; }
        size_t          chunk_size() const { return chunk_size_; }
        //Received bytes;
        int64_t         total_received() const { return total_received_; }
        int64_t         total_length() const { return total_length_; }

        bool            chunked_requests() const { return chunked_requests_; }

        const HttpHeaders*   headers() const {return &headers_;}

    private:    
        Status          ParseHeaders();

    private:
        std::ofstream           file_;
        int64_t                 file_pos_ = 0;      // local file position
        Mode                    mode_;
        std::vector<char>       buffer_;

    private:
        Http                    http_;
        HttpHeaders             headers_;
        size_t                  chunk_size_ = 1024 * 1024; //1mb
        size_t                  buffer_size_ = 1024 * 200; //200kb
        int64_t                 total_received_ = 0;        // remote file position
        int64_t                 total_length_ = 0;
        int64_t                 content_received_ = 0;
        int64_t                 content_length_ = 0;
        bool                    chunked_requests_ = false;

    private:
        //DISABLE_COPY
        HttpFile(const HttpFile&);
        void operator=(const HttpFile&);
    };

}
