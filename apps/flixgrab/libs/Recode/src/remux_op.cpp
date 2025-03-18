#include "remux_op.h"

using namespace recode;

RemuxOp::RemuxOp(const std::string& output_file, bool audio_only)
    : TranscodeOp(output_file, true, audio_only){
}

RemuxOp::~RemuxOp() {
}

