#pragma once 

#include <memory>
#include <atomic>
#include <string>
#include "operation.h"


namespace recode {

    class SubtitleOp : public Operation {

    public:
        SubtitleOp(const std::string& subtitle_url, const std::string& output_file);
        virtual ~SubtitleOp();

    public:
        Status        Do(OperationListener* listener) override;

    public:
        const std::string&  output_file() const { return output_file_; }


    private:
        std::string                                     output_file_;
        std::string                                     subtitle_url_;
       
    };

}

