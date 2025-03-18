// Copyright 2015 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "io_cache.h"

#include <string.h>

#include <algorithm>

namespace shaka {

namespace media {

IoCache::IoCache(uint64_t cache_size)
    : cache_size_(cache_size),
     // read_event_(false, false),
      //write_event_(false, false),
      // Make the buffer one byte larger than the cache so that when the
      // condition r_ptr == w_ptr is unambiguous (buffer empty).
      circular_buffer_(cache_size + 1),
      end_ptr_(&circular_buffer_[0] + cache_size + 1),
      r_ptr_(circular_buffer_.data()),
      w_ptr_(circular_buffer_.data()),
      closed_(false) {}

IoCache::~IoCache() {
  Close();
}

uint64_t IoCache::Read(void* buffer, uint64_t size) {
  DCHECK(buffer);

  std::unique_lock<std::mutex> lock(lock_);
  //AutoLock lock(lock_);
  while (!closed_ && (BytesCachedInternal() == 0)) {
    //AutoUnlock unlock(lock_);
    //write_event_.Wait();
      write_event_.wait(lock);
  }

  size = std::min(size, BytesCachedInternal());
  uint64_t first_chunk_size(std::min(size, static_cast<uint64_t>(
      end_ptr_ - r_ptr_)));
  memcpy(buffer, r_ptr_, first_chunk_size);
  r_ptr_ += first_chunk_size;
  DCHECK_GE(end_ptr_, r_ptr_);
  if (r_ptr_ == end_ptr_)
    r_ptr_ = &circular_buffer_[0];
  uint64_t second_chunk_size(size - first_chunk_size);
  if (second_chunk_size) {
    memcpy(static_cast<uint8_t*>(buffer) + first_chunk_size, r_ptr_,
           second_chunk_size);
    r_ptr_ += second_chunk_size;
    DCHECK_GT(end_ptr_, r_ptr_);
  }
  read_event_.notify_all();
  //read_event_.Signal();
  return size;
}

uint64_t IoCache::Write(const void* buffer, uint64_t size) {
  DCHECK(buffer);

  const uint8_t* r_ptr(static_cast<const uint8_t*>(buffer));
  uint64_t bytes_left(size);
  while (bytes_left) {
    //AutoLock lock(lock_);
    std::unique_lock<std::mutex> lock(lock_);
    while (!closed_ && (BytesFreeInternal() == 0)) {
      //AutoUnlock unlock(lock_);
      //read_event_.Wait();
        read_event_.wait(lock);
    }
    if (closed_)
      return 0;

    uint64_t write_size(std::min(bytes_left, BytesFreeInternal()));
    uint64_t first_chunk_size(std::min(write_size, static_cast<uint64_t>(
        end_ptr_ - w_ptr_)));
    memcpy(w_ptr_, r_ptr, first_chunk_size);
    w_ptr_ += first_chunk_size;
    DCHECK_GE(end_ptr_, w_ptr_);
    if (w_ptr_ == end_ptr_)
      w_ptr_ = &circular_buffer_[0];
    r_ptr += first_chunk_size;
    uint64_t second_chunk_size(write_size - first_chunk_size);
    if (second_chunk_size) {
      memcpy(w_ptr_, r_ptr, second_chunk_size);
      w_ptr_ += second_chunk_size;
      DCHECK_GT(end_ptr_, w_ptr_);
      r_ptr += second_chunk_size;
    }
    bytes_left -= write_size;
    write_event_.notify_all();
    //write_event_.Signal();
  }
  return size;
}

void IoCache::Clear() {
  //AutoLock lock(lock_);
    std::lock_guard<std::mutex> lock(lock_);
  r_ptr_ = w_ptr_ = circular_buffer_.data();
  // Let any writers know that there is room in the cache.
  //read_event_.Signal();
  read_event_.notify_all();
}

void IoCache::Close() {
  //AutoLock lock(lock_);
  std::lock_guard<std::mutex> lock(lock_);
  closed_ = true;
  read_event_.notify_all();
  write_event_.notify_all();
  //read_event_.Signal();
  //write_event_.Signal();
}

void IoCache::Reopen() {
    std::lock_guard<std::mutex> lock(lock_);
  //AutoLock lock(lock_);
  CHECK(closed_);
  r_ptr_ = w_ptr_ = circular_buffer_.data();
  closed_ = false;
  read_event_.notify_all();
  write_event_.notify_all();
  //read_event_.Reset();
  //write_event_.Reset();
}

uint64_t IoCache::BytesCached() {
  //AutoLock lock(lock_);
  std::lock_guard<std::mutex> lock(lock_);
  return BytesCachedInternal();
}

uint64_t IoCache::BytesFree() {
  //AutoLock lock(lock_);
    std::lock_guard<std::mutex> lock(lock_);
  return BytesFreeInternal();
}

uint64_t IoCache::BytesCachedInternal() {
  return (r_ptr_ <= w_ptr_)
             ? w_ptr_ - r_ptr_
             : (end_ptr_ - r_ptr_) + (w_ptr_ - circular_buffer_.data());
}

uint64_t IoCache::BytesFreeInternal() {
  return cache_size_ - BytesCachedInternal();
}

void IoCache::WaitUntilEmptyOrClosed() {
  //AutoLock lock(lock_);
  std::unique_lock<std::mutex> lock(lock_);
  while (!closed_ && BytesCachedInternal()) {
    //AutoUnlock unlock(lock_);
    //read_event_.Wait();
      read_event_.wait(lock);
  }
}

}  // namespace media
}  // namespace shaka
