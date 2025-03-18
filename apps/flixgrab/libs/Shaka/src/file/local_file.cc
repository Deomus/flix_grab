// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "local_file.h"

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif
#include <base\logging.h>

#include <base/strings/string_utils.h>


namespace shaka {
namespace media {

LocalFile::LocalFile(const char* file_name, const char* mode)
    : File(file_name), file_mode_(mode), internal_file_(NULL) {}

bool LocalFile::Close() {
  bool result = true;
  if (internal_file_) {

      result = (fclose(internal_file_) == 0);
    internal_file_ = NULL;
  }
  delete this;
  return result;
}

int64_t LocalFile::Read(void* buffer, uint64_t length) {
  DCHECK(buffer != NULL);
  DCHECK(internal_file_ != NULL);
  int err = ferror(internal_file_);
  return fread(buffer, sizeof(char), length, internal_file_);
}

int64_t LocalFile::Write(const void* buffer, uint64_t length) {
  DCHECK(buffer != NULL);
  DCHECK(internal_file_ != NULL);
  return fwrite(buffer, sizeof(char), length, internal_file_);
}

int64_t LocalFile::Size() {
  DCHECK(internal_file_ != NULL);

  // Flush any buffered data, so we get the true file size.
  if (!Flush()) {
    LOG(ERROR) << "Cannot flush file.";
    return -1;
  }

  int64_t file_size;

#ifdef WIN32
  WIN32_FILE_ATTRIBUTE_DATA attr;
  std::wstring  w_file = ASCIIToUTF16(file_name());
  if (!GetFileAttributesExW(w_file.c_str(),
      GetFileExInfoStandard, &attr)) {
      return -1;
  }

  ULARGE_INTEGER size;
  size.HighPart = attr.nFileSizeHigh;
  size.LowPart = attr.nFileSizeLow;
  file_size = size.QuadPart;
#endif

  return file_size;
}

bool LocalFile::Flush() {
  DCHECK(internal_file_ != NULL);
  return ((fflush(internal_file_) == 0) && !ferror(internal_file_));
}

bool LocalFile::Seek(uint64_t position) {
#if defined(WIN32)
    //return _fseeki64(internal_file_, position, SEEK_SET) >= 0;
    return (_fseeki64(internal_file_, position, SEEK_SET) == 0);
#else
  return fseeko(internal_file_, position, SEEK_SET) >= 0;
#endif
}

bool LocalFile::Tell(uint64_t* position) {
  
#if defined(WIN32)
    uint64_t offset = _ftelli64(internal_file_);
#else
    uint64_t offset = ftello(internal_file_);
#endif


  if (offset < 0)
    return false;
  *position = offset;
  return true;
}

LocalFile::~LocalFile() {}

bool LocalFile::Open() {

#ifdef WIN32
    std::wstring  w_mode;
    if (!strcmp(file_mode_.c_str(), "r"))
        w_mode = L"rb";
    else if (!strcmp(file_mode_.c_str(), "w"))
        w_mode = L"wb";
    
    std::wstring  w_file = ASCIIToUTF16(file_name());
    internal_file_ = _wfsopen(w_file.c_str(), w_mode.c_str(), _SH_DENYNO);

#endif
  /*internal_file_ =

      base::OpenFile(base::FilePath::FromUTF8Unsafe(file_name()), file_mode_.c_str());*/
  return (internal_file_ != NULL);
}

bool LocalFile::Delete(const char* file_name) {

#ifdef WIN32
    std::wstring fn = ASCIIToUTF16(file_name);
    if (::DeleteFileW(fn.c_str()) != 0)
        return true;
#endif 

    return false;

  //return base::DeleteFile(base::FilePath::FromUTF8Unsafe(file_name), false);
}

}  // namespace media
}  // namespace shaka
