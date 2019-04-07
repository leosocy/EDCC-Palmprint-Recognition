// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "core/comparer.h"
#include "config/config.h"

namespace edcc {
Status Comparer::Compare(const PalmprintCode& lhs_code, const PalmprintCode& rhs_code, double* score) {
  *score = .0;
  if (memcmp(&lhs_code.cfg, &rhs_code.cfg, sizeof(EncoderConfig)) != 0 || lhs_code.len != rhs_code.len) {
    return Status::CodeCfgNEWhenComparing("The two palmprint codes have different config.");
  }
  uint32_t acc = 0;
  const PalmprintCodeMetadata* lhs_md_cur_ptr = lhs_code.data;
  const PalmprintCodeMetadata* lhs_md_end_ptr = lhs_code.data + lhs_code.len;
  const PalmprintCodeMetadata* rhs_md_cur_ptr = rhs_code.data;
  while (lhs_md_cur_ptr < lhs_md_end_ptr) {
    uint8_t distance = *(uint8_t*)lhs_md_cur_ptr ^ *(uint8_t*)rhs_md_cur_ptr;
    if (distance == 0x00) {  // same d, s.
      acc += 2;
    } else if (distance < 0x10) {  // same d, diff s.
      acc += 0;
    }
    ++lhs_md_cur_ptr, ++rhs_md_cur_ptr;
  }
  *score = acc / (2.0 * lhs_code.len);
  return Status::Ok();
}
}  // namespace edcc