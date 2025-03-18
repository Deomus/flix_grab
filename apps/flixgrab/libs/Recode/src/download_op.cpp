#include "download_op.h"
#include <utils/string.h>

//#include <fstream>
#include <thread>
#include <filesystem>

using namespace std;

#define TIMEOUT_RETRY_COUNT               (3)

namespace recode {
    DownloadOp::DownloadOp(const string& url, const string& output_file, utils::HttpFile::Mode mode, long from)
        : url_(url)
        , output_file_(output_file)
        , mode_(mode)
        , from_(from) {
    }

    DownloadOp::~DownloadOp() {

    }

    Status DownloadOp::Do(OperationListener * listener) {
        utils::HttpFile  file(output_file_.c_str(), mode_);

        if (block_size_ > chunk_size_)
            return Status(error::Code::INVALID_ARGUMENT, "block_size_ > chunk_size_");

        file.set_chunk_size(chunk_size_);
        file.set_buffer_size(block_size_);

        // request_header=true is needed to get file length before download
        utils::Status status = file.Open(url_.c_str(), true, true, from_);

#ifdef _DEBUG    
        std::cout << "[" << std::this_thread::get_id() << "] DownloadOp: " << output_file_ << (limit_size_ < 1 ? " (limit " + std::to_string(limit_size_ * 100) + "%)" : "") << " << " << url_ << std::endl;
#endif
        int timeouts = 0;
        if (!this->is_cancelled()) {
            while (!this->is_cancelled()
                && status.error_code() != utils::Status::EndOfFile
                && status.error_code() != utils::Status::HttpError403Forbidden
                && status.error_code() != utils::Status::HttpError416WrongRange) {
                status = file.Download();
                std::cout << "status.error_code(): " << status.error_code() << " url_: " << url_ << std::endl;

                int okMul = 1;
                switch (status.error_code()) {
                case utils::Status::EndOfFile:
                    case utils::Status::HttpError416WrongRange:
                    case utils::Status::Success:
                        okMul = 1; // if progress in [0...1] then normal
                        break;
                    default:
                        okMul = -1; // if [-1...0) network error, keep retrying 
                        break;
                }
                // notify about issues
                if (listener) {
                    auto limit_mult = 0 < limit_size_ && limit_size_ < 1 ? limit_size_ : 1;
                    listener->OnProgress(okMul * file.total_received() / (double)file.total_length() / limit_mult);
                }

                if (okMul < 0) // some issues, wait for a while..
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

                if (0 < limit_size_ && limit_size_ < 1
                    && limit_size_ < file.total_received() / (double)file.total_length()
                    && status.error_code() == utils::Status::Success) {

                    //std::cout << "no more bytes for freebies " << file.total_received() << " / " << file.total_length() << std::endl;
                    
                    if (listener)
                        listener->OnProgress(1);

                    break;
                }
                //std::cout << "Received: " << file.total_received() << " Content Size: " << file.total_length() << std::endl;
            }
            (std::cout << "Finished: " << file.url() << std::endl).flush();

            if (!status.ok() &&
                file.headers()->HasHeader("X-Netflix-Geo-Check")) {
                std::string geo = file.headers()->RawHeader("X-Netflix-Geo-Check");
                if (geo.find("failed") != std::string::npos)
                    status = utils::Status(utils::Status::HttpError, "Please, check your proxy");
            }

            if (!status.ok() && status.error_code() != utils::Status::EndOfFile)
                (std::cout << "Error: " << status.error_message() << std::endl).flush();

        }

        file.Close();

        if (status.error_code() == utils::Status::HttpError416WrongRange)
            std::filesystem::remove(output_file_);

        return (status.ok()
            || status.error_code() == utils::Status::EndOfFile
            || status.error_code() == utils::Status::HttpError416WrongRange)?
                Status():
                Status(error::Code::HTTP_ERROR, status.error_message(), status.error_code());
    }

}

