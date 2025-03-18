// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef MEDIA_BASE_DEMUXER_ANNEX_H_
#define MEDIA_BASE_DEMUXER_ANNEX_H_

#include <deque>
#include <vector>
#include <map>
#include <functional>

//#include "packager/base/compiler_specific.h"
//#include "packager/base/memory/ref_counted.h"
//#include "packager/base/memory/scoped_ptr.h"
//#include "packager/media/base/container_names.h"
//#include "packager/media/base/status.h"


#include "media/base/container_names.h"
#include "./status.h"
#include "base/macros.h"
#include "media/base/stream_parser.h"
//#include "stream_info.h"
#include "media/base/media_tracks.h"


#include <memory>
#include <cstdint>

using namespace media::container_names;
using namespace media;

namespace media {
    class StreamParser;
    class DecodeTimestamp;
}
namespace shaka {
namespace media {

class Status;
class File;

class MediaSample;

class DemuxerAnnex;

typedef std::function<void(DemuxerAnnex*)> demuxer_init_fn;
typedef std::function<bool(DemuxerAnnex*, const std::shared_ptr<MediaSample>& sample)> demuxer_sample_fn;
typedef std::function<void(DemuxerAnnex*)> demuxer_finish_fn;

/// Demuxer is responsible for extracting elementary stream samples from a
/// media file, e.g. an ISO BMFF file.
class DemuxerAnnex /*: public MediaParserCallback*/ {
 public:
  /// @param file_name specifies the input source. It uses prefix matching to
  ///        create a proper File object. The user can extend File to support
  ///        a custom File object with its own prefix.
     explicit DemuxerAnnex(const std::string& file_name, const demuxer_init_fn& cb_init, const demuxer_sample_fn& cb_sample, const demuxer_finish_fn& cb_finish);
     ~DemuxerAnnex();
      
  /// Initialize the Demuxer. Calling other public methods of this class
  /// without this method returning OK, results in an undefined behavior.
  /// This method primes the demuxer by parsing portions of the media file to
  /// extract stream information.
  /// @return OK on success.
  Status Initialize();

  /// Drive the remuxing from demuxer side (push). Read the file and push
  /// the Data to Muxer until Eof.
  Status Run();

  /// Read from the source and send it to the parser.
  Status Parse();

  /// Cancel a demuxing job in progress. Will cause @a Run to exit with an error
  /// status of type CANCELLED.
  void Cancel();

   /// @return Container name (type). Value is CONTAINER_UNKNOWN if the demuxer
  ///         is not initialized.
  MediaContainerName container_name() { return container_name_; }

  const MediaTracks& tracks() const { return tracks_; }
  const std::vector<uint8_t>& GetInitData();
  double GetDuration() const { return duration_; }
  
  int GetSampleWidth(int track_id) const { return parser_->GetSampleWidth(track_id); }
  int GetSampleHeight(int track_id) const { return parser_->GetSampleHeight(track_id); }

 private:
  struct QueuedSample {
    QueuedSample(uint32_t track_id, std::shared_ptr<StreamParserBuffer> sample);
    ~QueuedSample();

    uint32_t track_id;
    std::shared_ptr<StreamParserBuffer> sample;
  };

private:
  // Parser init event.
  void InitF(const StreamParser::InitParameters& expected_params, const StreamParser::InitParameters& params);
  bool NewConfigF(std::unique_ptr<MediaTracks> tracks, const StreamParser::TextTrackConfigMap& tc);
  bool NewBuffersF(const StreamParser::BufferQueueMap& buffer_queue_map);
  void KeyNeededF(EmeInitDataType type, const std::vector<uint8_t>& init_data);
  void NewSegmentF();
  void EndOfSegmentF();

  void InitializeParserWithInitParametersExpectations(StreamParser::InitParameters params);
  StreamParser::InitParameters GetDefaultInitParametersExpectations();

  // Parser new sample event handler. Queues the samples if init event has not
  // been received, otherwise calls PushSample() to push the sample to
  // corresponding stream.
  /*virtual bool OnNewSample( const std::shared_ptr<StreamParserBuffer>& sample);*/
  // Helper function to push the sample to corresponding stream.
  bool PushSample(const scoped_refptr<StreamParserBuffer>& buf);

  std::string file_name_;
  File* media_file_;
  bool init_event_received_;
  Status init_parsing_status_;
  std::vector<uint8_t> init_data_; // if file is encrypted
  double   duration_ = 0; // in seconds

  std::unique_ptr<StreamParser> parser_;
  
  MediaTracks tracks_;
  MediaContainerName container_name_;
  std::unique_ptr<uint8_t[]> buffer_;
  
  bool cancelled_;

  std::unique_ptr<DecodeTimestamp> lower_bound_;

  std::unique_ptr<MediaTracks> media_tracks_;

  demuxer_init_fn                       init_callback_;
  demuxer_sample_fn                     sample_callback_;
  demuxer_finish_fn                     finish_callback_;

private:

    DISALLOW_COPY_AND_ASSIGN(DemuxerAnnex);
};

}  // namespace media
}  // namespace shaka

#endif  // MEDIA_BASE_DEMUXER_ANNEX_H_
