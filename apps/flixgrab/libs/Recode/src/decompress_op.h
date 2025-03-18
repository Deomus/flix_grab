#pragma once

#include <product_config.h>

#include <string>
#include <vector>

#include "operation.h"

namespace recode {

    enum EDecompressErrorCode {
        DECOMPRESS_ERROR_CODE_UNKNOWN = 0,
        DECOMPRESS_ERROR_CODE_GZ = 0,
        DECOMPRESS_ERROR_CODE_TAR = 1,
        DECOMPRESS_ERROR_CODE_ZIP = 2
    };

class DecompressOp :
    public Operation {

public:
    DecompressOp(const std::string& temp_dir);
    virtual ~DecompressOp();

    void SaveOriginalZipTrack(bool save_original_zip, const std::wstring& output_zip) { 
        save_original_zip_track_ = save_original_zip;
        output_zip_ = output_zip;
    }

    void AddZip(const std::string& in_file, std::string output = "") {
        streams_zip_.push_back(make_pair(in_file, output));
    }

    void AddTarGz(const std::string& in_file, std::string output) {
        streams_tar_gz_.push_back(make_pair(in_file, output));
    }
    int StreamZipCount() { return streams_zip_.size(); }
    int StreamTarGzCount() { return streams_tar_gz_.size(); }

    Status                                              Do(OperationListener* listener) override;
    virtual void                                        Cancel() override;

private:
    std::vector<std::pair<std::string, std::string>>    streams_zip_;
    std::vector<std::pair<std::string, std::string>>    streams_tar_gz_;
    bool                                                save_original_zip_track_ = false;
    std::wstring                                        output_zip_;
    std::string                                         temp_dir_;

};
}
