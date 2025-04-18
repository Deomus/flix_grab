// Copyright (c) 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_HASH_MD5_BORINGSSL_H_
#define BASE_HASH_MD5_BORINGSSL_H_

#include <stdint.h>

//#include "third_party/boringssl/src/include/openssl/md5.h"
// MD5_DIGEST_LENGTH is the length of an MD5 digest.
#define MD5_DIGEST_LENGTH 16

namespace base {


	typedef struct MD5Context {
		uint32_t buf[4];
		uint32_t bits[2];
		unsigned char in[64];
	} MD5_CTX;


// The output of an MD5 operation.
struct MD5Digest {
  uint8_t a[MD5_DIGEST_LENGTH];
};

// Used for storing intermediate data during an MD5 computation. Callers
// should not access the data.
typedef MD5_CTX MD5Context;

}  // namespace base

#endif  // BASE_HASH_MD5_BORINGSSL_H_
