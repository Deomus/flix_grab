#include "utils/http_file.h"
#include "utils/string.h"


utils::HttpFile::HttpFile(const char* file_name, Mode mode /*= Truncate*/)
    : mode_(mode) {
    int file_mode =
        mode == Truncate ? std::ios_base::trunc : std::ios_base::ate | std::ios_base::app;

    file_mode = file_mode | std::ios_base::in | std::ios_base::out | std::ios_base::binary;

    std::wstring wfile_name = utils::Utf8ToUtf16(file_name);//utils::ASCIIToUTF16(file_name);

    file_.open(wfile_name, file_mode);

    if (file_.is_open()) {
        file_pos_ = mode == Continue || mode == Append ? file_.tellp() : 0; // local file position
        total_received_ = mode == Continue ? file_.tellp() : 0;             // remote file position
    }
    //(std::cout << "http file_pos_: " << file_pos_ << ", " << file_name << std::endl).flush();
}

utils::HttpFile::~HttpFile() {
    Close();
}

utils::Status utils::HttpFile::Open(const char* url, bool chunked_requests /*= false*/, bool request_header, long remote_file_pos) {
    chunked_requests_ = chunked_requests;
    (std::cout << "Http open url: " << url << std::endl).flush();
    
    Status status = http_.Open(url);

    if (status.ok()
        && request_header) {

        status = http_.Headers(&headers_);
    
        if (status.ok())
            status = ParseHeaders();
        else {
            (std::cout << "ERROR HEADER: " << headers_.original() << std::endl).flush();
        }

    }
    content_received_ = content_length_ = 0;
    if (remote_file_pos >= 0)
        total_received_ = remote_file_pos; // set remote file position

    return status;
}

utils::Status utils::HttpFile::ParseHeaders() {
    Status status;
    if (headers_.status_code() == 200 ||
        headers_.status_code() == 206) {
        total_length_ = content_length_ = headers_.ContentLength();
        bool accept_ranges = headers_.HasHeader("Accept-Ranges") ?
            strstr(headers_.RawHeader("Accept-Ranges"), "bytes") : false;

        if (headers_.HasHeader("Content-Range"))
            total_length_ = headers_.ContentRangeLength();
        else if (!accept_ranges) {
            //Ranges skiped by server;
            total_received_ = 0;
        }
        chunked_requests_ = accept_ranges;
        //std::cout << " http_.Headers accept_ranges:" << accept_ranges << ", total_received:" << total_received_ << ", total_length: " << total_length_ << std::endl;
    }
    else {
        std::cout << "HttpFile ParseHeaders: " << headers_.status_code();
        auto code = Status::HttpError;
        if (headers_.status_code() == 403)
            code = Status::HttpError403Forbidden;
        else if (headers_.status_code() == 416)
            code = Status::HttpError416WrongRange;
        status.Update({ code, "Http Status code " + std::to_string(headers_.status_code()) });
    }
    return status;
}


utils::Status utils::HttpFile::Download() {
    Status    status;
    
    if (total_received_ >= total_length_ && total_length_ > 0)
        return status.Update(Status::EndOfFile); // already complete

    // 1. Send Get-request
    if (content_received_ >= content_length_) {
        int64_t last_pos = chunked_requests_ ? total_received_ + chunk_size_ : -1;
        //std::cout << " http_.Get " << (chunked_requests_ ? "chunked " : "") << total_received_ << "+" << chunk_size_ << ". content_length_:" << content_length_ << ". content_received_:" << content_received_ << std::endl;
        status = http_.Get(total_received_, last_pos, &headers_);
    
        if (status.ok() &&
            (status = ParseHeaders()).ok()) {
            content_received_ = 0;
            if (!chunked_requests_ && mode_ == Continue)
                file_.seekp(file_pos_);
        }
    }

    // 2. Read content
    if (status.ok()) {
        uint64_t length = buffer_size_;
        buffer_.resize(buffer_size_);

        status = http_.Read(buffer_.data(), &length);
        if (status.ok()) {
            file_.write(buffer_.data(), length);
            if (file_.good()) {
                total_received_ += length;
                content_received_ += length;
                // length < buffer_.size() && 
                if (total_received_ >= total_length_)
                    status.Update(Status::EndOfFile);
                //std::cout << " file write buflen: " << length << ",content: " << content_received_ << ", total:" << total_received_ << ", total_length_" << total_length_ << ". s:" << status.error_code() << std::endl;
            }
            else
                status.Update({ Status::FileFailure, "IO Error " + std::to_string(errno) + ": " + std::strerror(errno) });
        }
        else {
            //Drop current request;
            content_length_ = 0;
        }
    }

    return status;
}

void utils::HttpFile::Close() {
    http_.Close();
    file_.close();
    (std::cout << "Http close " << http_.url() << std::endl).flush();
}

const char* utils::HttpFile::url() const {
    return http_.url();
}
