#include "subtitle_op.h"
#include <utils/http_file.h>
#include <utils/string.h>

#include <fstream>

using namespace std;

namespace recode {
    SubtitleOp::SubtitleOp(const string& subtitle_url, const string& output_file)
        : subtitle_url_(subtitle_url)
        , output_file_(output_file){

    }

    SubtitleOp::~SubtitleOp() {

    }

    Status SubtitleOp::Do(OperationListener * listener) {
        utils::HttpFile  file(output_file_.c_str());

        file.set_chunk_size(1024*1024);
        file.set_buffer_size(1024 * 100);

        utils::Status status = file.Open(subtitle_url_.c_str(), true, false);

        while (status.ok()) {
            status = file.Download();

            if (listener) {
                listener->OnProgress(file.total_received()/(double)file.total_length());
            }
            std::cout << "Received subtitles: " << file.total_received() << " Content Size: " << file.total_length() << std::endl;
        }
        //std::cout << "Finished!" << std::endl;

        /*if (status.error_code() == utils::Status::EndOfFile)
            status = utils::Status::SUCCESS;*/

        return (status.ok() || status.error_code() == utils::Status::EndOfFile)?
                Status():
                Status(error::Code::HTTP_ERROR, status.error_message(), status.error_code());
    }

}

