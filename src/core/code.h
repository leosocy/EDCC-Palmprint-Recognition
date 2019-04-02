// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CORE_CODE_H_
#define EDCC_SRC_CORE_CODE_H_

#include <stdint.h>
#include "config/config.h"

namespace edcc {

typedef struct {
  // `d` means `gabor direction` which range is [0, 15], so pow(2, 4) - 1 = 15
  // can cover direction.
  uint8_t d : 4;
  // `s` means `direction side` which value is left(1) or right(0), so range
  // [0,1] can cover side.
  uint8_t s : 1;
  uint8_t _pad : 3;
} PalmprintCodeMetadata;

typedef struct {
  EncoderConfig cfg;
  uint32_t len;  // num of metadata
  PalmprintCodeMetadata data[0];
} PalmprintCode;

}  // namespace edcc

#endif  // EDCC_SRC_CORE_CODE_H_