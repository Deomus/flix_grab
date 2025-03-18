// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "file.h"
#include "file_closer.h"

#include <memory>
#include <algorithm>


#include "local_file.h"
#include "http_file_s.h"
//#include "packager/media/file/memory_file.h"
#include "threaded_io_file.h"
//#include "packager/media/file/udp_file.h"
//#include "packager/base/strings/string_util.h"

#ifdef CopyFile
#undef CopyFile
#endif
// Each command-line flag has two variables associated with it: one
// with the current value, and one with the default value.  However,
// we have a third variable, which is where value is assigned; it's a
// constant.  This guarantees that FLAG_##value is initialized at
// static initialization time (e.g. before program-start) rather than
// than global construction time (which is after program-start but
// before main), at least when 'value' is a compile-time constant.  We
// use a small trick for the "default value" variable, and call it
// FLAGS_no<name>.  This serves the second purpose of assuring a
// compile error if someone tries to define a flag named no<name>
// which is illegal (--foo and --nofoo both affect the "foo" flag).
#define DEFINE_VARIABLE(type, shorttype, name, value, help)             \
  namespace fL##shorttype {                                             \
    static const type FLAGS_nono##name = value;                         \
    /* We always want to export defined variables, dll or no */         \
    type FLAGS_##name = FLAGS_nono##name;        \
    type FLAGS_no##name = FLAGS_nono##name;                             \
        }                                                                     \
  using fL##shorttype::FLAGS_##name 

#define DEFINE_uint64(name,val, txt) \
   DEFINE_VARIABLE(uint64_t, U64, \
                   name, val, txt)


DEFINE_uint64(io_cache_size,
              8ULL << 20,
              "Size of the threaded I/O cache, in bytes. Specify 0 to disable "
              "threaded I/O.");
DEFINE_uint64(io_block_size,
              2ULL << 20,
              "Size of the block size used for threaded I/O, in bytes.");

namespace shaka {
namespace media {

const char* kLocalFilePrefix = "file://";
const char* kUdpFilePrefix = "udp://";
const char* kMemoryFilePrefix = "memory://";
const char* kHttpFilePrefix = "http://";
const char* kHttpsFilePrefix = "https://";

namespace {

typedef File* (*FileFactoryFunction)(const char* file_name, const char* mode);
typedef bool (*FileDeleteFunction)(const char* file_name);

struct SupportedTypeInfo {
  const char* type;
  size_t type_length;
  const FileFactoryFunction factory_function;
  const FileDeleteFunction delete_function;
};

File* CreateLocalFile(const char* file_name, const char* mode) {
  return new LocalFile(file_name, mode);
}

bool DeleteLocalFile(const char* file_name) {
  return LocalFile::Delete(file_name);
}

File* CreateHttpFile(const char* file_name, const char* mode) {
    return new HttpFileShaka(file_name, mode);
}
//
//File* CreateUdpFile(const char* file_name, const char* mode) {
//  if (base::strcasecmp(mode, "r")) {
//    NOTIMPLEMENTED() << "UdpFile only supports read (receive) mode.";
//    return NULL;
//  }
//  return new UdpFile(file_name);
//}

//File* CreateMemoryFile(const char* file_name, const char* mode) {
//  return new MemoryFile(file_name, mode);
//}
//
//bool DeleteMemoryFile(const char* file_name) {
//  MemoryFile::Delete(file_name);
//  return true;
//}
//
static const SupportedTypeInfo kSupportedTypeInfo[] = {
  {
    kLocalFilePrefix,
    strlen(kLocalFilePrefix),
    &CreateLocalFile,
    &DeleteLocalFile
  },
  {
      kHttpFilePrefix,
      strlen(kHttpFilePrefix),
    &CreateHttpFile,
    NULL
  },
  {
      kHttpsFilePrefix,
      strlen(kHttpsFilePrefix),
      &CreateHttpFile,
      NULL
  },
};

}  // namespace

File* File::Create(const char* file_name, const char* mode) {
  std::unique_ptr<File, FileCloser> internal_file(
      CreateInternalFile(file_name, mode));

  if (!strncmp(file_name, kMemoryFilePrefix, strlen(kMemoryFilePrefix))) {
    // Disable caching for memory files.
    return internal_file.release();
  }

  if (FLAGS_io_cache_size) {
    // Enable threaded I/O for "r", "w", and "a" modes only.
    if (!strcmp(mode, "r")) {
      return new ThreadedIoFile(std::move(internal_file),
                                ThreadedIoFile::kInputMode,
                                FLAGS_io_cache_size,
                                FLAGS_io_block_size);
    } else if (!strcmp(mode, "w") || !strcmp(mode, "a")) {
      return new ThreadedIoFile(std::move(internal_file),
                                ThreadedIoFile::kOutputMode,
                                FLAGS_io_cache_size,
                                FLAGS_io_block_size);
    }
  }

  // Threaded I/O is disabled.
  DLOG(WARNING) << "Threaded I/O is disabled. Performance may be decreased.";
  return internal_file.release();
}

File* File::CreateInternalFile(const char* file_name, const char* mode) {
    std::unique_ptr<File, FileCloser> internal_file;
  for (size_t i = 0; i < arraysize(kSupportedTypeInfo); ++i) {
    const SupportedTypeInfo& type_info = kSupportedTypeInfo[i];
    if (strncmp(type_info.type, file_name, type_info.type_length) == 0) {
      internal_file.reset(type_info.factory_function(
          file_name/* + type_info.type_length*/, mode));
    }
  }
  // Otherwise we assume it is a local file
  if (!internal_file)
    internal_file.reset(CreateLocalFile(file_name, mode));

  return internal_file.release();
}

File* File::Open(const char* file_name, const char* mode) {
  File* file = File::Create(file_name, mode);
  if (!file)
    return NULL;
  if (!file->Open()) {
    delete file;
    return NULL;
  }
  return file;
}

File* File::OpenWithNoBuffering(const char* file_name, const char* mode) {
  File* file = File::CreateInternalFile(file_name, mode);
  if (!file)
    return NULL;
  if (!file->Open()) {
    delete file;
    return NULL;
  }
  return file;
}

bool File::Delete(const char* file_name) {
  for (size_t i = 0; i < arraysize(kSupportedTypeInfo); ++i) {
    const SupportedTypeInfo& type_info = kSupportedTypeInfo[i];
    if (strncmp(type_info.type, file_name, type_info.type_length) == 0) {
      return type_info.delete_function ?
          type_info.delete_function(file_name + type_info.type_length) :
          false;
    }
  }
  // Otherwise we assume it is a local file
  return DeleteLocalFile(file_name);
}

int64_t File::GetFileSize(const char* file_name) {
  File* file = File::Open(file_name, "r");
  if (!file)
    return -1;
  int64_t res = file->Size();
  file->Close();
  return res;
}

bool File::ReadFileToString(const char* file_name, std::string* contents) {
  DCHECK(contents);

  File* file = File::Open(file_name, "r");
  if (!file)
    return false;

  const size_t kBufferSize = 0x40000;  // 256KB.
  std::unique_ptr<char[]> buf(new char[kBufferSize]);

  int64_t len;
  while ((len = file->Read(buf.get(), kBufferSize)) > 0)
    contents->append(buf.get(), len);

  file->Close();
  return len == 0;
}

bool File::Copy(const char* from_file_name, const char* to_file_name) {
  std::string content;
  if (!ReadFileToString(from_file_name, &content)) {
    LOG(ERROR) << "Failed to open file " << from_file_name;
    return false;
  }

  std::unique_ptr<File, FileCloser> output_file(File::Open(to_file_name, "w"));
  if (!output_file) {
    LOG(ERROR) << "Failed to write to " << to_file_name;
    return false;
  }

  uint64_t bytes_left = content.size();
  uint64_t total_bytes_written = 0;
  const char* content_cstr = content.c_str();
  while (bytes_left > total_bytes_written) {
    const int64_t bytes_written =
        output_file->Write(content_cstr + total_bytes_written, bytes_left);
    if (bytes_written < 0) {
      LOG(ERROR) << "Failure while writing to " << to_file_name;
      return false;
    }

    total_bytes_written += bytes_written;
  }
  return true;
}

int64_t File::CopyFile(File* source, File* destination) {
  return CopyFile(source, destination, kWholeFile);
}

int64_t File::CopyFile(File* source, File* destination, int64_t max_copy) {
  DCHECK(source);
  DCHECK(destination);
  if (max_copy < 0)
    max_copy = std::numeric_limits<int64_t>::max();

  const int64_t kBufferSize = 0x40000;  // 256KB.
  std::unique_ptr<uint8_t[]> buffer(new uint8_t[kBufferSize]);
  int64_t bytes_copied = 0;
  while (bytes_copied < max_copy) {
    const int64_t size = std::min(kBufferSize, max_copy - bytes_copied);
    const int64_t bytes_read = source->Read(buffer.get(), size);
    if (bytes_read < 0)
      return bytes_read;
    if (bytes_read == 0)
      break;

    int64_t total_bytes_written = 0;
    while (total_bytes_written < bytes_read) {
      const int64_t bytes_written = destination->Write(
          buffer.get() + total_bytes_written, bytes_read - total_bytes_written);
      if (bytes_written < 0)
        return bytes_written;

      total_bytes_written += bytes_written;
    }

    DCHECK_EQ(total_bytes_written, bytes_read);
    bytes_copied += bytes_read;
  }

  return bytes_copied;
}

//#include <windows.h>


//
//bool CreateTemporaryFile(FilePath* path) {
//    ThreadRestrictions::AssertIOAllowed();
//
//    FilePath temp_file;
//
//    if (!GetTempDir(path))
//        return false;
//
//    if (CreateTemporaryFileInDir(*path, &temp_file)) {
//        *path = temp_file;
//        return true;
//    }
//
//    return false;
//}
//
//
//bool CreateTemporaryFileInDir(const FilePath& dir, FilePath* temp_file) {
//    ThreadRestrictions::AssertIOAllowed();
//
//    wchar_t temp_name[MAX_PATH + 1];
//
//    if (!GetTempFileName(dir.value().c_str(), L"", 0, temp_name)) {
//        DPLOG(WARNING) << "Failed to get temporary file name in "
//            << UTF16ToUTF8(dir.value());
//        return false;
//    }
//
//    wchar_t long_temp_name[MAX_PATH + 1];
//    DWORD long_name_len = GetLongPathName(temp_name, long_temp_name, MAX_PATH);
//    if (long_name_len > MAX_PATH || long_name_len == 0) {
//        // GetLongPathName() failed, but we still have a temporary file.
//        *temp_file = FilePath(temp_name);
//        return true;
//    }
//
//    FilePath::StringType long_temp_name_str;
//    long_temp_name_str.assign(long_temp_name, long_name_len);
//    *temp_file = FilePath(long_temp_name_str);
//    return true;
//}

//
//
//std::string GetTempDir() {
//    wchar_t temp_path[MAX_PATH + 1];
//    DWORD path_len = ::GetTempPathW(MAX_PATH, temp_path);
//    if (path_len >= MAX_PATH || path_len <= 0)
//        return false;
//
//    return UTF16ToASCII(temp_path);
//    // TODO(evanm): the old behavior of this function was to always strip the
//    // trailing slash.  We duplicate this here, but it shouldn't be necessary
//    // when everyone is using the appropriate FilePath APIs.
//    //*path = FilePath(temp_path).StripTrailingSeparators();
//    //return true;
//}
//
//std::string File::CreateTemporaryFile()
//{
//
//}

}  // namespace media
}  // namespace shaka
