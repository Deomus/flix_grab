#pragma once 

#include <memory>
#include <atomic>
#include <string>

#include <utils/http_file.h>

#include "operation.h"

namespace recode {

    class DownloadOp : public Operation {

    public:
        DownloadOp(const std::string& url, const std::string& output_file, utils::HttpFile::Mode mode = utils::HttpFile::Mode::Truncate, long from = 0);
        virtual ~DownloadOp();

    public:
        Status                  Do(OperationListener* listener) override;

    public:
        const std::string&      output_file() const { return output_file_; }

        void                    set_chunk_size(size_t chunk) { chunk_size_ = chunk; }
        size_t                  chunk_size() const { return chunk_size_; }

        void                    set_block_size(size_t block) { block_size_ = block; }
        size_t                  block_size() const { return block_size_; }
        void                    set_limit_size(double limit_size) { limit_size_ = limit_size; }


    private:
        std::string             output_file_;
        std::string             url_;
        utils::HttpFile::Mode   mode_;
        long                    from_; // get range in case of Append mode

        size_t                  chunk_size_ = 1024 * 1024;
        size_t                  block_size_ = 100 * 1024;
        double                  limit_size_ = 1; // percent of file limit (0 ... 1)
    };

}

