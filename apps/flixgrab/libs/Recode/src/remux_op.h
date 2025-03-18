#pragma once 

#include <string>

#include "transcode_op.h"

namespace recode {

    class RemuxOp : public TranscodeOp {
    public:
        RemuxOp(const std::string& output_file, bool audio_only = false);
        virtual ~RemuxOp();
    };
}
