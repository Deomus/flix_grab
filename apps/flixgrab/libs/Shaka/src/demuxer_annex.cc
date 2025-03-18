#include "demuxer_annex.h"

#include "file/file.h"

#include "media/formats/mp4/mp4_stream_parser.h"
#include "base/sys_byteorder.h"
#include "media/base/media_tracks.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/video_decoder_config.h"
#include "media/base/text_track_config.h"
#include "media_sample.h"

#include <fstream>
#include <base\bind.h>
#include <media\formats\mp4\es_descriptor.h>

// TODO:
#define EXPECT_EQ(x,y) x; y;
#define EXPECT_NE(x,y) x; y;
#define EXPECT_FALSE(x) x;
#define EXPECT_TRUE(x) x;


namespace {
// 65KB, sufficient to determine the container and likely all init data.
const size_t kInitBufSize = 0x10000;
const size_t kBufSize = 0x200000;  // 2MB
//const size_t kBufSize = 0x100000;  // 1MB
// Maximum number of allowed queued samples. If we are receiving a lot of
// samples before seeing init_event, something is not right. The number
// set here is arbitrary though.
const size_t kQueuedSamplesLimit = 10000;
}
using namespace media;
using namespace shaka::media;

namespace shaka {
namespace media {

DemuxerAnnex::DemuxerAnnex(const std::string& file_name, const demuxer_init_fn& cb_init, const demuxer_sample_fn& cb_sample, const demuxer_finish_fn& cb_finish)
    : file_name_(file_name),
      media_file_(NULL),
      init_event_received_(false),
      container_name_(CONTAINER_UNKNOWN),
      buffer_(new uint8_t[kBufSize]),
      cancelled_(false),
      init_callback_(cb_init),
      sample_callback_(cb_sample),
      finish_callback_(cb_finish)
{
    lower_bound_.reset(new DecodeTimestamp(DecodeTimestamp::FromPresentationTime(base::TimeDelta::Max())));
}

DemuxerAnnex::~DemuxerAnnex() {
  if (media_file_)
    media_file_->Close();
 // STLDeleteElements(&streams_);
}
/*
void DemuxerAnnex::SetKeySource(std::unique_ptr<KeySource>& key_source) {
    key_source_ = std::move( key_source);// .Pass();
}
*/
Status DemuxerAnnex::Initialize() {
  DCHECK(!media_file_);
  DCHECK(!init_event_received_);

  LOG(INFO) << "Initialize Demuxer for file '" << file_name_ << "'.";

  media_file_ = File::Open(file_name_.c_str(), "r");
  if (!media_file_) {
    return Status(error::FILE_FAILURE,
                  "Cannot open file for reading " + file_name_);
  }
   
  // Read enough bytes before detecting the container.
  size_t bytes_read = 0;
  while (bytes_read < kInitBufSize) {
    int64_t read_result =
        media_file_->Read(buffer_.get() + bytes_read, kInitBufSize);
    if (read_result < 0)
      return Status(error::FILE_FAILURE, "Cannot read file " + file_name_);
    if (read_result == 0)
      break;
    bytes_read += static_cast<size_t>(read_result);
  }
  container_name_ = DetermineContainer(buffer_.get(), bytes_read);

  // Initialize media parser.
  switch (container_name_) {
    case CONTAINER_MOV:
        parser_.reset(new mp4::MP4StreamParser({::media::mp4::ObjectType::kForbidden, ::media::mp4::ObjectType::kISO_14496_3, ::media::mp4::ObjectType::kISO_13818_7_AAC_LC, ::media::mp4::ObjectType::kAC3, ::media::mp4::ObjectType::kEAC3 }, false, false));
      break;
  /*  case CONTAINER_MPEG2TS:
      parser_.reset(new mp2t::Mp2tMediaParser());
      break;
    case CONTAINER_MPEG2PS:
      parser_.reset(new wvm::WvmMediaParser());
      break;
    case CONTAINER_WEBM:
      parser_.reset(new WebMMediaParser());
      break;
    case CONTAINER_WEBVTT:
      parser_.reset(new WebVttMediaParser());
      break;*/
    default:
      //NOTIMPLEMENTED();
      return Status(error::UNIMPLEMENTED, "Container not supported.");
  }

  InitializeParserWithInitParametersExpectations(
      GetDefaultInitParametersExpectations());

  // Handle trailing 'moov'.

  //if (container_name_ == CONTAINER_MOV){
  //    if (!static_cast<mp4::MP4StreamParser*>(parser_.get())->LoadMoov(buffer_.get(), bytes_read))
  //        static_cast<mp4::MP4StreamParser*>(parser_.get())->LoadMoov(file_name_);
  //}

  if (!parser_->Parse(buffer_.get(), bytes_read)) {
    init_parsing_status_ =
        Status(error::PARSER_FAILURE, "Cannot parse media file " + file_name_);
  }

  // Parse until init event received or on error.
  while (!init_event_received_ && init_parsing_status_.ok())
    init_parsing_status_ = Parse();
  // Defer error reporting if init completed successfully.
  return init_event_received_ ? Status::OK : init_parsing_status_;
}

void DemuxerAnnex::InitializeParserWithInitParametersExpectations(
    StreamParser::InitParameters params) {
    parser_->Init(base::BindOnce(&DemuxerAnnex::InitF,
        base::Unretained(this), params),
        base::BindRepeating(&DemuxerAnnex::NewConfigF,
            base::Unretained(this)),
        base::BindRepeating(&DemuxerAnnex::NewBuffersF,
            base::Unretained(this)),
        true,
        base::BindRepeating(&DemuxerAnnex::KeyNeededF,
            base::Unretained(this)),
        base::BindRepeating(&DemuxerAnnex::NewSegmentF,
            base::Unretained(this)),
        base::BindRepeating(&DemuxerAnnex::EndOfSegmentF,
            base::Unretained(this))
    );
}

StreamParser::InitParameters DemuxerAnnex::GetDefaultInitParametersExpectations() {
    // Most unencrypted test mp4 files have zero duration and are treated as
    // live streams.
    StreamParser::InitParameters params(kInfiniteDuration);
    params.liveness = DemuxerStream::LIVENESS_RECORDED;
    params.detected_audio_track_count = 1;
    params.detected_video_track_count = 1;
    params.detected_text_track_count = 0;
    params.duration = base::TimeDelta::FromInternalValue(0);
    return params;
}

void DemuxerAnnex::InitF(const StreamParser::InitParameters& expected_params,
    const StreamParser::InitParameters& params) {
    DVLOG(1) << "InitF: dur=" << params.duration.InMicroseconds();
    duration_ = params.duration.InSecondsF();
    if (std::isnan(duration_))
        duration_ = 0;

    if (init_callback_)
        init_callback_(this);
}

bool DemuxerAnnex::NewConfigF(std::unique_ptr<MediaTracks> tracks,
    const StreamParser::TextTrackConfigMap& tc) {
    
    init_event_received_ = true;

    CHECK(tracks.get());
    DVLOG(1) << "NewConfigF: got " << tracks->tracks().size() << " tracks";

    for (const auto& track : tracks->tracks()) {
        const auto& track_id = track->bytestream_track_id();
        if (track->type() == MediaTrack::Audio) {
            auto audio_track_id_ = track_id;
            auto audio_decoder_config_ = tracks->getAudioConfig(track_id);
            tracks_.AddAudioTrack(audio_decoder_config_, track_id, track->kind(), track->label(), track->language());
            DVLOG(1) << "track_id=" << track_id << " audio config="
                << (audio_decoder_config_.IsValidConfig()
                    ? audio_decoder_config_.AsHumanReadableString()
                    : "INVALID");
        }
        else if (track->type() == MediaTrack::Video) {
            auto video_track_id_ = track_id;
            auto video_decoder_config_ = tracks->getVideoConfig(track_id);
            DVLOG(1) << "track_id=" << track_id << " video config="
                << (video_decoder_config_.IsValidConfig()
                    ? video_decoder_config_.AsHumanReadableString()
                    : "INVALID");
            if(tracks_.getVideoConfig(track_id).codec() == VideoCodec::kUnknownVideoCodec)
                tracks_.AddVideoTrack(video_decoder_config_, track->bytestream_track_id(), track->kind(), track->label(), track->language());
        }
    }
    media_tracks_ = std::move(tracks);
    return true;
}

bool DemuxerAnnex::NewBuffersF(const StreamParser::BufferQueueMap& buffer_queue_map) {
    DecodeTimestamp lowest_end_dts = kNoDecodeTimestamp();
    for (const auto& it : buffer_queue_map) {
        DVLOG(3) << "Buffers for track_id=" << it.first;
        DCHECK(!it.second.empty());

        if (lowest_end_dts == kNoDecodeTimestamp() ||
            lowest_end_dts > it.second.back()->GetDecodeTimestamp())
            lowest_end_dts = it.second.back()->GetDecodeTimestamp();

        for (const auto& buf : it.second) {
            /*DVLOG(3) << "  track_id=" << buf->track_id()
                << ", size=" << buf->data_size()
                << ", pts=" << buf->timestamp().InSecondsF()
                << ", dts=" << buf->GetDecodeTimestamp().InSecondsF()
                << ", dur=" << buf->duration().InSecondsF()
               ;*/
            // Ensure that track ids are properly assigned on all emitted buffers.
            //EXPECT_EQ(it.first, buf->track_id());

            // Let single-track tests verify the sequence of keyframes/nonkeyframes.
            //if (verifying_keyframeness_sequence_) {
            //    keyframeness_cb_.Run(buf->is_key_frame()
            //        ? Keyframeness::kKeyframe
            //        : Keyframeness::kNonKeyframe);
            //}
            
            if (!PushSample(buf))
                return false;
        }
    }

    EXPECT_NE(lowest_end_dts, kNoDecodeTimestamp());

    if (*lower_bound_ != kNoDecodeTimestamp() && lowest_end_dts < *lower_bound_) {
        return false;
    }

    *lower_bound_ = lowest_end_dts;
    return true;
}

void DemuxerAnnex::KeyNeededF(EmeInitDataType type, const std::vector<uint8_t>& init_data) {
    DVLOG(1) << "KeyNeededF: " << init_data.size();
    EXPECT_EQ(EmeInitDataType::CENC, type);
    EXPECT_FALSE(init_data.empty());
    init_data_ = init_data; // make a copy
}

void DemuxerAnnex::NewSegmentF() {
    DVLOG(1) << "NewSegmentF";
    *lower_bound_ = kNoDecodeTimestamp();
}

void DemuxerAnnex::EndOfSegmentF() {
    DVLOG(1) << "EndOfSegmentF()";
    *lower_bound_ =
        DecodeTimestamp::FromPresentationTime(base::TimeDelta::Max());
}


DemuxerAnnex::QueuedSample::QueuedSample(uint32_t local_track_id,
                                    std::shared_ptr<StreamParserBuffer> local_sample)
: track_id(local_track_id), sample(local_sample) {}
DemuxerAnnex::QueuedSample::~QueuedSample() {}


bool DemuxerAnnex::PushSample(const scoped_refptr<StreamParserBuffer>& buf) {
    if (sample_callback_) {
        auto cpyBuf = MediaSample::CopyFrom(buf->data(), buf->data_size(),
            buf->is_key_frame()
        );
        cpyBuf->set_timescale(1000000); // microseconds
        cpyBuf->set_dts(buf->GetDecodeTimestamp().InMicroseconds());
        cpyBuf->set_pts(buf->timestamp().InMicroseconds());
        cpyBuf->set_duration(buf->duration().InMicroseconds());
        cpyBuf->set_track_id(buf->track_id());
        const auto decryp = buf->decrypt_config();
        if (decryp != NULL) {
            cpyBuf->set_is_encrypted(true);
            cpyBuf->set_decrypt_config(decryp->Clone());
        }
        if (!sample_callback_(this, cpyBuf))
            return false; // some errors
    }
    return true;
}

Status DemuxerAnnex::Run() {
  Status status;
  LOG(INFO) << "Demuxer::Run() on file '" << file_name_ << "'.";

  while (!cancelled_ && (status = Parse()).ok())
    continue;

  if (cancelled_ && status.ok())
    return Status(error::CANCELLED, "Demuxer run cancelled");

  if (status.error_code() == error::END_OF_STREAM) {
    // Push EOS sample to muxer to indicate end of stream.
      //const auto& sample = MediaSample::CreateEOSBuffer();
    /*for (std::vector<MediaStream*>::iterator it = streams_.begin();
         it != streams_.end();
         ++it) {
      status = (*it)->PushSample(sample);
      if (!status.ok())
        return status;
    }*/
  }
  return status;
}

Status DemuxerAnnex::Parse() {
  DCHECK(media_file_);
  DCHECK(parser_);
  DCHECK(buffer_);

  // Return early and avoid call Parse(...) again if it has already failed at
  // the initialization.
  if (!init_parsing_status_.ok())
    return init_parsing_status_;

  int64_t bytes_read = media_file_->Read(buffer_.get(), kBufSize);
  if (bytes_read == 0) {
      parser_->Flush();
    //if (!parser_->Flush())
      //return Status(error::PARSER_FAILURE, "Failed to flush.");
    if (finish_callback_)
        finish_callback_(this);
    return Status(error::END_OF_STREAM, "");
  } else if (bytes_read < 0) {
    return Status(error::FILE_FAILURE, "Cannot read file " + file_name_);
  }

  return parser_->Parse(buffer_.get(), static_cast<size_t>(bytes_read))
             ? Status::OK
             : Status(error::PARSER_FAILURE,
                      "Cannot parse media file " + file_name_);   
}

void DemuxerAnnex::Cancel() {
  cancelled_ = true;
}

const std::vector<uint8_t>& DemuxerAnnex::GetInitData()
{
    return init_data_;
}

}  // namespace media
}  // namespace shaka
