#include "decompress_op.h"

#include <iostream>
#include <filesystem>

#include <utils/string.h>
#include <utils/zlib_utils.h>


using namespace recode;

DecompressOp::DecompressOp(const std::string& temp_dir){
    temp_dir_ = temp_dir;
}

DecompressOp::~DecompressOp(){}

Status DecompressOp::Do(OperationListener* listener) {
    //std::lock_guard<std::mutex> lock(mutex_);
    Status status;
    std::cout << "DecompressOp::Do() " << std::endl;

    if (is_cancelled()) {
        //case if we was cancelled when we was waiting by mutex
        std::cout << "DecompressOp Cancelled " << std::endl;
        return status;
    }

    for (auto& stream : streams_zip_) {
        if (zlib::zipExtract(stream.first, stream.second) != 0)
            return Status(error::Code::DECOMPRESS_SUBTITLES_ERROR, "Cannot decompress zip", DECOMPRESS_ERROR_CODE_ZIP);
        else if (save_original_zip_track_
            && !output_zip_.empty())
            std::filesystem::rename(stream.first, output_zip_ + utils::Utf8ToUtf16(stream.first.substr(stream.first.find_last_of("/") + 1, stream.first.size())));
        else
            std::filesystem::remove(stream.first);
    }

    for (auto& stream : streams_tar_gz_) {
        if (zlib::gzExtract(stream.first) != 0)
            return Status(error::Code::DECOMPRESS_SUBTITLES_ERROR, "Cannot decompress gz", DECOMPRESS_ERROR_CODE_GZ);
        else
            std::filesystem::remove(stream.first);
        
        std::string output = temp_dir_;
        if (!stream.second.empty())
            output = stream.second;

        if (zlib::tarExtract(stream.first.substr(0, stream.first.size() - 3), output) != 0)
            return Status(error::Code::DECOMPRESS_SUBTITLES_ERROR, "Cannot decompress tar", DECOMPRESS_ERROR_CODE_TAR);
        else
            std::filesystem::remove(stream.first.substr(0, stream.first.size() - 3));
    }

    return status;
}

void DecompressOp::Cancel() {
    Operation::Cancel();
}