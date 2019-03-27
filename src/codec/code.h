// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CODEC_CODE_H_
#define EDCC_SRC_CODEC_CODE_H_

#include <stdint.h>
#include "config/config.h"

namespace edcc {

typedef struct {
  uint8_t c : 5;  // c is `gabor direction` which range is [4, 16], so pow(2, 6)
                  // - 1 = 31 can cover direction.
  uint8_t cs : 1;  // cs value is in range [0,1].
} PalmprintCodeMetadata;

typedef struct {
  EdccConfig cfg;
  uint32_t len;
  PalmprintCodeMetadata data[0];
} ReliablePalmprintCode;  // `Reliable` means that for each Palmprint code we
                          // store the corresponding encoder config for
                          // comparison between different codes.

typedef struct {
  uint32_t len;
  PalmprintCodeMetadata data[0];
} FastPalmprintCode;  // `Fast` means no encoder config is stored, and
                      // the caller guarantees that the two codes being compared
                      // are encoded by the same config, or an unexpected
                      // error will occur.

}  // namespace edcc

#endif  // EDCC_SRC_CODEC_CODE_H_