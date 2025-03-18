#include "transcode_op.h"
#include "media_sample.h"

extern "C" {
#include "ffmpeg.h"
}

#include <vector>
#include <stdio.h>
// ignore depricated warning
//#pragma warning(disable : 4996)

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avdevice.lib")

using namespace recode;

std::mutex   TranscodeOp::mutex_;

TranscodeOp::TranscodeOp(const std::string& output_file, bool use_h264_codec, bool copy_codec, bool audio_only):
    output_file_(output_file), use_h264_codec_(use_h264_codec), copy_codec_(copy_codec), audio_only_(audio_only) {
}

TranscodeOp::~TranscodeOp() {
}

void TranscodeOp::AddStream(const std::string& uri, const std::pair<std::string, std::string>& metadata) {
    streams_.push_back(std::pair(uri, metadata));
    std::cout << "TranscodeOp::AddStream " << uri << " size:" << streams_.size() << std::endl;
}

extern "C" {
    FILE* Win32Open(const char* filename_utf8);
}
bool checkSRTFile(char* fname) {
    int len;
    char str[30];

    if (fname == NULL) return true;
    len = strlen(fname);
    if (toupper(fname[len - 3]) == 'S' &&
        toupper(fname[len - 2]) == 'R' &&
        toupper(fname[len - 1]) == 'T') {
        FILE* fp = Win32Open(fname);
        if (!fp) {
            std::cout << "isSRTbad:File " << fname << " has problem" << std::endl;
            return true;
        }
        // SRT must has 3 line minimum
        if (fseek(fp, 0, SEEK_END) < 0) {
            std::cout << "isSRTbad:SRT is bad(seek)" << std::endl;
            fclose(fp);
            return true;
        }
        if (ftell(fp) > 10) {
            std::cout << "isSRTbad:SRT is good" << std::endl;
            fclose(fp);
            return false; // SRT is good
        }
        else {
            std::cout << "isSRTbad:SRT is bad" << std::endl;
            fclose(fp);
            return true;
        }
    }
    std::cout << "isSRTbad:file is not SRT" << std::endl;
    return false;
}

Status TranscodeOp::Do(OperationListener* listener) {
    //std::lock_guard<std::mutex> lock(mutex_);
    Status status;
    std::cout << "TranscodeOp::Do() " << output_file_ << ". " << metadata_title_ << ". " << metadata_artist_
        << ". " << metadata_album_ << ". " << metadata_album_artist_ << ". " << metadata_year_
        << ". " << metadata_comment_ << ". " << metadata_genre_ << ". " << metadata_track_ << ". " << artwork_url_.c_str() << std::endl;
    
    if (is_cancelled()) {
        //case if we was cancelled when we was waiting by mutex
        std::cout << "TranscodeOp Cancelled " << output_file_ << std::endl;
        return status;
    }

    // ffmpeg progress:
    progress_arg = listener;
    progress_arg2 = this;
    progress_callback = [](void* arg, void* arg2, int p) {
        if(arg2 != NULL && ((TranscodeOp*)arg2)->is_cancelled())
            sigterm_handler(1);
        if(arg != NULL)
            ((OperationListener*)arg)->OnProgress((double)p / 100.0);
    };
    
    std::vector<const char*> streams;
    std::vector<const char*> language;
    for (auto& s : streams_) {
        std::cout << "Fs:" << s.first.c_str() << std::endl;

        if (s.first.find(".srt") != std::string::npos
            && checkSRTFile((char*)s.first.c_str()))
            continue;

        streams.emplace_back(s.first.c_str());

        if (s.second.first == "language"        //Add metadata here
            && !s.second.second.empty())
            language.emplace_back(s.second.second.c_str());
        else {
            language.push_back("\0");
        }
    }

    // ffmpeg main:
    std::cout << "TranscodeOp Call ffmpeg " << std::endl;
    auto ret = main_0(streams.data(), language.data(), streams.size(), output_file_.c_str(), use_h264_codec_, copy_codec_, audio_only_,
        limit_duration_,
        metadata_title_.c_str(), metadata_artist_.c_str(), metadata_album_.c_str(), metadata_album_artist_.c_str(),
        metadata_year_.c_str(), metadata_comment_.c_str(), metadata_genre_.c_str(), metadata_track_.c_str()/*, artwork_url_.c_str()*/);

    if (ret != 0)
        status = Status(error::ENCODER_ERROR, "ffmpeg transcode err", -30);

    std::cout << "TranscodeOp end ffmpeg " << std::endl;
    progress_arg = NULL;
    progress_arg2 = NULL;
    progress_callback = NULL;

    return status;
}

void TranscodeOp::Cancel() {
    Operation::Cancel();
    sigterm_handler(1);
}

void TranscodeOp::set_artwork_url(const std::string& artwork_url) {
    artwork_url_ = artwork_url_;
}
void TranscodeOp::set_metadata_title(const std::string& metadata_title) {
    metadata_title_ = metadata_title;
}
void TranscodeOp::set_metadata_artist(const std::string& metadata_artist) {
    metadata_artist_ = metadata_artist;
}
void TranscodeOp::set_metadata_album(const std::string& metadata_album) {
    metadata_album_ = metadata_album;
}
void TranscodeOp::set_metadata_album_artist(const std::string& metadata_album_artist) {
    metadata_album_artist_ = metadata_album_artist;
}
void TranscodeOp::set_metadata_track(const int metadata_track) {
    metadata_track_ = metadata_track;
}
void TranscodeOp::set_metadata_year(const int metadata_year) {
    metadata_year_ = metadata_year;
}
void TranscodeOp::set_metadata_comment(const std::string& metadata_comment) {
    metadata_comment_ = metadata_comment;
}
void TranscodeOp::set_metadata_genre(const std::string& metadata_genre) {
    metadata_genre_ = metadata_genre;
}

