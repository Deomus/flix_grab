#include "remux_mp3_op.h"

#include <presets.h>

#include "mmconv.h"
#include "mmconvlame.h"

using namespace recode;

// input url
RemuxMP3Op::RemuxMP3Op() {
#ifdef _DEBUG
    // sample url:
    //url_ = "http://player.ooyala.com/player/ipad/J4NXB4djqiGBj6JAGO_YxRX6KRPdM2Vk_3500.m3u8";
    //url_ = "https://r1---sn-ug5onuxaxjvh-n8vs.googlevideo.com/videoplayback?expire=1569255676&ei=nJyIXa6IG4S17QSPv4XwAw&ip=95.79.32.153&id=o-AB-xUDf5A89hS5SKOoa40NS0Xv_vZsMJ8hGNl8rmAMMl&itag=18&source=youtube&requiressl=yes&mime=video/mp4&gir=yes&clen=23868988&ratebypass=yes&dur=519.941&lmt=1565962572730525&fvip=10&c=WEB&txp=2211222&sparams=expire,ei,ip,id,itag,source,requiressl,mime,gir,clen,ratebypass,dur,lmt&sig=ALgxI2wwRQIhAI9bvleEEWJ_uMlQipto6J8zjg3sHGstw5WTjZp3a7jKAiARHRQ64-AFj3mjSWgZGIxCesVOZGocmPa3PQlP22CWuA==&rm=sn-xguxaxjvh-bvws7e,sn-n8vrre7&req_id=2c070d95d31ea3ee&redirect_counter=2&cms_redirect=yes&ipbypass=yes&mip=176.104.129.214&mm=29&mn=sn-ug5onuxaxjvh-n8vs&ms=rdu&mt=1569234463&mv=m&mvi=0&pl=21&lsparams=ipbypass,mip,mm,mn,ms,mv,mvi,pl&lsig=AHylml4wRgIhAMaZTiYK5mRxByvd8-m5dPdhTK0FDw-v6aHHbbLrINbpAiEAyTpmanCI5gaE7UIXZsbrSgY_0iYgqBS-MRrEU51Nf2I=";
#endif
}

RemuxMP3Op::~RemuxMP3Op() {
    //
}

void RemuxMP3Op::AddStream(const std::string& url, const std::pair<std::wstring, int>& out) {
    std::cout << "RemuxMP3Op::AddStream " << url << std::endl;
    streams_.push_back({ url, out });
}

recode::Status RemuxMP3Op::Do(OperationListener * listener) {
    std::unique_lock<std::mutex> lock(mutex_);
    listener_ = listener;

    Status status;

    std::wcout << "RemuxMP3Op::Do() " << output_file_ << std::endl;
    if (is_cancelled()) {
        //case if we was cancelled when we was waiting by mutex
        std::wcout << "RemuxMP3Op Cancelled " << output_file_ << std::endl;
        return status;
    }
    
    int32_t res = MMConv::CLameTranscoder::createLameTranscoder(this, transcoder_);
    if (res != 0)
        return Status(error::Code::CONV_ERROR, "Lame transcoder error", res);

    transcoder_->set_metadata_title(id3tag_title_);
    transcoder_->set_metadata_artist(id3tag_artist_);
    transcoder_->set_metadata_album(id3tag_album_);
    transcoder_->set_metadata_album_artist(id3tag_album_artist_);
    transcoder_->set_metadata_year(id3tag_year_);
    transcoder_->set_metadata_comment(id3tag_comment_);
    transcoder_->set_metadata_genre(id3tag_genre_);
    transcoder_->set_metadata_track(id3tag_track_);
    transcoder_->set_metadata_image_path(id3tag_image_path_);

    int audioOutputSampleRate = 0; // Hz, 0 - keep original
    int audioOutputChannels = 0;    // 0 - keep original
    for (auto& s : streams_) {
        int audioOutputBitrate = lamePresetValue(s.second.second);

        transcoder_->start(s.first.c_str(), s.second.first, audioOutputSampleRate, audioOutputBitrate, audioOutputChannels);

        cv_.wait(lock);
        lock.unlock();

        res = transcoder_->getErrorCode();
        if (res != 0)
            return Status(error::Code::CONV_ERROR, "Lame transcoder error", res);
    }

    if (listener_)
        listener_->OnProgress(1);

    return status;
}

void RemuxMP3Op::Cancel() {
    if(transcoder_)
        transcoder_->stop();
    Operation::Cancel();
}

void RemuxMP3Op::set_metadata_title(const std::wstring& metadata_title) {
    id3tag_title_ = metadata_title;
}
void RemuxMP3Op::set_metadata_artist(const std::wstring& metadata_artist) {
    id3tag_artist_ = metadata_artist;
}
void RemuxMP3Op::set_metadata_album(const std::wstring& metadata_album) {
    id3tag_album_ = metadata_album;
}
void RemuxMP3Op::set_metadata_album_artist(const std::wstring& metadata_album_artist) {
    id3tag_album_artist_ = metadata_album_artist;
}
void RemuxMP3Op::set_metadata_year(const std::wstring& metadata_year) {
    id3tag_year_ = metadata_year;
}
void RemuxMP3Op::set_metadata_comment(const std::wstring& metadata_comment) {
    id3tag_comment_ = metadata_comment;
}
void RemuxMP3Op::set_metadata_genre(const std::wstring& metadata_genre) {
    id3tag_genre_ = metadata_genre;
}
void RemuxMP3Op::set_metadata_track(const std::wstring& metadata_track) {
    id3tag_track_ = metadata_track;
}
void RemuxMP3Op::set_metadata_image_path(const std::string& metadata_image_path) {
    id3tag_image_path_ = metadata_image_path;
}

int RemuxMP3Op::lamePresetValue(const int preset) {
    int value = kPreset_MP3Original;
    switch (preset) {
        case kPreset_MP3Original:
            value = 0;
            break;
        case kPreset_MP3LameR3mix:
            value = -4;
            break;
        case kPreset_MP3LameStandard:
            value = -1;
            break;
        case kPreset_MP3LameExtreme:
            value = -2;
            break;
        case kPreset_MP3LameInsane:
            value = -3;
            break;
        case kPreset_MP3LowQuality:
            value = -10;
            break;
        case kPreset_MP3FromVideoStream:
            value = -11;
            break;
    }
    
    return value;
}

//// output stream
//void recode::RemuxMP3Op::AddStream(const std::string& file, const std::map<std::string, std::string>& metadata /*= std::map<std::string, std::string>()*/)
//{
//  std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
//  if (lock.owns_lock()) {
//      streams_.push_back({ file, metadata });
//  }
//}
