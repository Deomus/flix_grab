//#pragma once
//#define BUILDFLAG(x) false

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BUILD_BUILDFLAG_H_
#define BUILD_BUILDFLAG_H_

// These macros un-mangle the names of the build flags in a way that looks
// natural, and gives errors if the flag is not defined. Normally in the
// preprocessor it's easy to make mistakes that interpret "you haven't done
// the setup to know what the flag is" as "flag is off". Normally you would
// include the generated header rather than include this file directly.
//
// This is for use with generated headers. See build/buildflag_header.gni.

// This dance of two macros does a concatenation of two preprocessor args using
// ## doubly indirectly because using ## directly prevents macros in that
// parameter from being expanded.
#define BUILDFLAG_CAT_INDIRECT(a, b) a ## b
#define BUILDFLAG_CAT(a, b) BUILDFLAG_CAT_INDIRECT(a, b)

// Accessor for build flags.
//
// To test for a value, if the build file specifies:
//
//   ENABLE_FOO=true
//
// Then you would check at build-time in source code with:
//
//   #include "foo_flags.h"  // The header the build file specified.
//
//   #if BUILDFLAG(ENABLE_FOO)
//     ...
//   #endif
//
// There will no #define called ENABLE_FOO so if you accidentally test for
// whether that is defined, it will always be negative. You can also use
// the value in expressions:
//
//   const char kSpamServerName[] = BUILDFLAG(SPAM_SERVER_NAME);
//
// Because the flag is accessed as a preprocessor macro with (), an error
// will be thrown if the proper header defining the internal flag value has
// not been included.
#define BUILDFLAG(flag) (BUILDFLAG_CAT(BUILDFLAG_INTERNAL_, flag)())



#define BUILDFLAG_INTERNAL_ALTERNATE_CDM_STORAGE_ID_KEY() ("")
#define BUILDFLAG_INTERNAL_CDM_PLATFORM_SPECIFIC_PATH() ("_platform_specific/win_x64")
#define BUILDFLAG_INTERNAL_ENABLE_PLATFORM_AC3_EAC3_AUDIO() (1)
#define BUILDFLAG_INTERNAL_ENABLE_CDM_HOST_VERIFICATION() (0)
#define BUILDFLAG_INTERNAL_ENABLE_CDM_STORAGE_ID() (0)
#define BUILDFLAG_INTERNAL_ENABLE_DAV1D_DECODER() (1)
#define BUILDFLAG_INTERNAL_ENABLE_AV1_DECODER() (1)
#define BUILDFLAG_INTERNAL_ENABLE_PLATFORM_DOLBY_VISION() (0)
#define BUILDFLAG_INTERNAL_ENABLE_FFMPEG() (1)
#define BUILDFLAG_INTERNAL_ENABLE_FFMPEG_VIDEO_DECODERS() (1)
#define BUILDFLAG_INTERNAL_ENABLE_PLATFORM_HEVC() (0)
#define BUILDFLAG_INTERNAL_ENABLE_HLS_SAMPLE_AES() (0)
#define BUILDFLAG_INTERNAL_ENABLE_LIBGAV1_DECODER() (0)
#define BUILDFLAG_INTERNAL_ENABLE_LIBRARY_CDMS() (1)
#define BUILDFLAG_INTERNAL_ENABLE_LIBVPX() (1)
#define BUILDFLAG_INTERNAL_ENABLE_LOGGING_OVERRIDE() (0)
#define BUILDFLAG_INTERNAL_ENABLE_MEDIA_DRM_STORAGE() (0)
#define BUILDFLAG_INTERNAL_ENABLE_MEDIA_REMOTING() (1)
#define BUILDFLAG_INTERNAL_ENABLE_MEDIA_REMOTING_RPC() (1)
#define BUILDFLAG_INTERNAL_ENABLE_OPENH264() (0)
#define BUILDFLAG_INTERNAL_ENABLE_PLATFORM_MPEG_H_AUDIO() (0)
#define BUILDFLAG_INTERNAL_ENABLE_MSE_MPEG2TS_STREAM_PARSER() (0)
#define BUILDFLAG_INTERNAL_USE_CHROMEOS_MEDIA_ACCELERATION() (false||false)
#define BUILDFLAG_INTERNAL_USE_CHROMEOS_PROTECTED_MEDIA() (0)
#define BUILDFLAG_INTERNAL_IS_CHROMECAST() (0)
#define BUILDFLAG_INTERNAL_USE_PROPRIETARY_CODECS() (1)


#endif  // BUILD_BUILDFLAG_H_
