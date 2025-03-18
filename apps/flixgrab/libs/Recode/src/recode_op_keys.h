#pragma once

#include <product_config.h>

#include <map>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "operation.h"
#include "windows.h"

namespace recode {

    /// <summary>
    /// Uses mp4Decryptor with private keys
    /// </summary>
    class RecodeOpKeys : public Operation {

    public:
        RecodeOpKeys(const std::string& video_url, const std::string& output_file, const std::vector<std::string>& keys);

        virtual ~RecodeOpKeys();

        Status              Do(OperationListener* listener) override;
        

    public:
        //void                set_duration(long duration) { duration_ = duration; }

    private:
        void                runMp4Decrypt();
        bool                decryptWithKeys(const std::vector<std::string>& keys, const std::string& input_file, const std::string& output_file);

    private:
        Status                                          status_;

        const std::string                               output_file_;
        const std::string                               video_url_;
        const std::vector<std::string>                  keys_;

        OperationListener*                              progress_listener_;
        int64_t                                         progress_value_;

    private:
        uint64_t                                        duration_ = 0;

        HANDLE g_hChildStd_OUT_Rd = NULL;
        HANDLE g_hChildStd_OUT_Wr = NULL;

    };
}
