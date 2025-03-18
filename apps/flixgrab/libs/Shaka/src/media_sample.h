// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef MEDIA_BASE_MEDIA_SAMPLE_H_
#define MEDIA_BASE_MEDIA_SAMPLE_H_

#include <deque>
#include <string>
#include <vector>
#include <memory>

#include "base/macros.h"
#include "base/logging.h"
#include <media\base\decrypt_config.h>

using namespace media;

namespace shaka {
namespace media {

/// Class to hold a media sample.
class MediaSample : public std::enable_shared_from_this<MediaSample> {
 public:
  /// Create a MediaSample object from input.
  /// @param data points to the buffer containing the sample data.
  ///        Must not be NULL.
  /// @param size indicates sample size in bytes. Must not be negative.
  /// @param is_key_frame indicates whether the sample is a key frame.
  static std::shared_ptr<MediaSample> CopyFrom(const uint8_t* data,
                                             size_t size,
                                             bool is_key_frame);

  /// Create a MediaSample object from input.
  /// @param data points to the buffer containing the sample data.
  ///        Must not be NULL.
  /// @param side_data points to the buffer containing the additional data.
  ///        Some containers allow additional data to be specified.
  ///        Must not be NULL.
  /// @param size indicates sample size in bytes. Must not be negative.
  /// @param side_data_size indicates additional sample data size in bytes.
  /// @param is_key_frame indicates whether the sample is a key frame.
  static std::shared_ptr<MediaSample> CopyFrom(const uint8_t* data,
                                             size_t size,
                                             const uint8_t* side_data,
                                             size_t side_data_size,
                                             bool is_key_frame);

  /// Create a MediaSample object from metadata.
  /// Unlike other factory methods, this cannot be a key frame. It must be only
  /// for metadata.
  /// @param metadata points to the buffer containing metadata.
  ///        Must not be NULL.
  /// @param metadata_size is the size of metadata in bytes.
  static std::shared_ptr<MediaSample> FromMetadata(const uint8_t* metadata,
                                                 size_t metadata_size);

  /// Create a MediaSample object with default members.
  static std::shared_ptr<MediaSample> CreateEmptyMediaSample();

  /// Create a MediaSample indicating we've reached end of stream.
  /// Calling any method other than end_of_stream() on the resulting buffer
  /// is disallowed.
  static std::shared_ptr<MediaSample> CreateEOSBuffer();

  void  append_extra_data(const std::vector<uint8_t>& extra);

  uint32_t  track_id() const { return track_id_; }
  void  set_track_id(uint32_t track_id)  { track_id_ = track_id; }


  int64_t dts() const {
    DCHECK(!end_of_stream());
    return dts_;
  }
    
  void set_dts(int64_t dts) { dts_ = dts; }

  int64_t pts() const {
    DCHECK(!end_of_stream());
    return pts_;
  }

  void set_pts(int64_t pts) { pts_ = pts; }

  int64_t duration() const {
    DCHECK(!end_of_stream());
    return duration_;
  }

  void set_duration(int64_t duration) {
    DCHECK(!end_of_stream());
    duration_ = duration;
  }

  int64_t timescale() const {
      DCHECK(!end_of_stream());
      return timescale_;
  }

  void set_timescale(int64_t ts) { timescale_ = ts; }


  bool is_key_frame() const {
    DCHECK(!end_of_stream());
    return is_key_frame_;
  }

  bool is_encrypted() const {
    DCHECK(!end_of_stream());
    return is_encrypted_;
  }
  const uint8_t* data() const {
    DCHECK(!end_of_stream());
    return &data_[0];
  }

  uint8_t* writable_data() {
    DCHECK(!end_of_stream());
    return &data_[0];
  }

  size_t data_size() const {
    DCHECK(!end_of_stream());
    return data_.size();
  }

  const uint8_t* side_data() const {
    return &side_data_[0];
  }

  size_t side_data_size() const {
    return side_data_.size();
  }

  void set_data(const uint8_t* data, const size_t data_size) {
    data_.assign(data, data + data_size);
  }

  void set_side_data(const uint8_t* data, const size_t data_size) {
      side_data_.assign(data, data + data_size);
  }
  
  void resize_data(const size_t data_size) {
    data_.resize(data_size);
  }

  void set_is_key_frame(bool value) {
    is_key_frame_ = value;
  }

  void set_is_encrypted(bool value) {
    is_encrypted_ = value;
  }

  void set_decrypt_config(std::unique_ptr<DecryptConfig> config)
  {
      decrypt_config_ = std::move(config);
  }

  DecryptConfig* decrypt_config() { return decrypt_config_.get(); }

  // If there's no data in this buffer, it represents end of stream.
  bool end_of_stream() const { return data_.size() == 0; }

  const std::string& config_id() const { return config_id_; }
  void set_config_id(const std::string& config_id) {
    config_id_ = config_id;
  }

  /// @return a human-readable string describing |*this|.
  std::string ToString() const;

 public:
	 virtual ~MediaSample();

 private:
     //friend class std::_Ref_count<MediaSample>;
     friend class std::shared_ptr<MediaSample>;
  //friend class base::RefCountedThreadSafe<MediaSample>;

  // Create a MediaSample. Buffer will be padded and aligned as necessary.
  // |data|,|side_data| can be NULL, which indicates an empty sample.
  // |size|,|side_data_size| should not be negative.
  MediaSample(const uint8_t* data,
              size_t size,
              const uint8_t* side_data,
              size_t side_data_size,
              bool is_key_frame);
  MediaSample();
  

  // Decoding time stamp.
  int64_t dts_;
  // Presentation time stamp.
  int64_t pts_;
  int64_t duration_;
  int64_t timescale_;
  bool is_key_frame_;
  // is sample encrypted ?
  bool is_encrypted_;

  uint32_t      track_id_;

  //Decrypt config if sample encrypted;
  std::unique_ptr<DecryptConfig> decrypt_config_;

  // Main buffer data.
  std::vector<uint8_t> data_;
  // Contain additional buffers to complete the main one. Needed by WebM
  // http://www.matroska.org/technical/specs/index.html BlockAdditional[A5].
  // Not used by mp4 and other containers.
  std::vector<uint8_t> side_data_;

  // Text specific fields.
  // For now this is the cue identifier for WebVTT.
  std::string config_id_;

  DISALLOW_COPY_AND_ASSIGN(MediaSample);
};

typedef std::deque<std::shared_ptr<MediaSample> > BufferQueue;

}  // namespace media
}  // namespace shaka

#endif  // MEDIA_BASE_MEDIA_SAMPLE_H_
