#pragma once
#include "operation.h"
#include <mutex>
#include <string>
#include <vector>
#include <map>

namespace recode {
    class TranscodeOp :
        public Operation
    {

    public:
        TranscodeOp(const std::string& output_file, bool use_h264_codec = false, bool copy_codec = false, bool audio_only = false);
        virtual ~TranscodeOp();

        Status              Do(OperationListener* listener) override;
        virtual void        Cancel() override;

        void                AddStream(const std::string& url, const std::pair<std::string, std::string>& metadata = std::pair("", ""));
        int                 StreamCount() { return streams_.size(); }
        void                SetOutputFile(const std::string& output_file) { output_file_ = output_file; }
        void                SetCopyCodec(bool copy_codec) { copy_codec_ = copy_codec; }
        void                UseH264Codec(const bool use_h264_codec) { use_h264_codec_ = use_h264_codec; }
        void                SetAudioOnly(bool audio_only) { audio_only_ = audio_only; }
        bool                CopyCodec() { return copy_codec_; }
        void                SetLimitDuration(int limit_duration) { limit_duration_ = limit_duration; }
    
    public:
        void                set_artwork_url(const std::string& artwork_url);
        void                set_metadata_title(const std::string& metadata_title);
        void                set_metadata_artist(const std::string& metadata_artist);
        void                set_metadata_album(const std::string& metadata_album);
        void                set_metadata_album_artist(const std::string& metadata_album_artist);
        void                set_metadata_track(const int metadata_track);
        void                set_metadata_year(const int metadata_year);
        void                set_metadata_comment(const std::string& metadata_comment);
        void                set_metadata_genre(const std::string& metadata_genre);

    private:
        std::vector<std::pair<std::string, std::pair<std::string, std::string>>> streams_;
        bool                                        use_h264_codec_ = false;
        std::string                                 output_file_;
        bool                                        copy_codec_;
        bool                                        audio_only_;
        int                                         limit_duration_ = 0; // seconds of limit duration for trial license
        static std::mutex                           mutex_;
        
        //metadata
        std::string                                 artwork_url_;
        std::string                                 metadata_title_;
        std::string                                 metadata_artist_;
        std::string                                 metadata_album_;
        std::string                                 metadata_album_artist_;
        std::string                                 metadata_track_;
        std::string                                 metadata_year_;
        std::string                                 metadata_comment_;
        std::string                                 metadata_genre_;
    };
}