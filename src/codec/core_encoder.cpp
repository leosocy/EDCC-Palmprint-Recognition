// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "codec/core_encoder.h"
#include <cmath>
#include <vector>

namespace edcc {

CoreEncoder::CoreEncoder(const CoreEncoderConfig &cfg) : cfg_(cfg) {
  gabor_filter_ = std::unique_ptr<GaborFilter>(new GaborFilter(cfg_));
}

size_t CoreEncoder::GetCodeBufferSize() const {
  uint32_t metadata_len = cfg_.image_size * cfg_.image_size;
  return sizeof(PalmprintCode) + metadata_len * sizeof(PalmprintCodeMetadata);
}

Status CoreEncoder::Encode(const cv::Mat &palmprint, PalmprintCode *code, size_t buffer_size) const {
  assert(code != nullptr);
  if (buffer_size < GetCodeBufferSize()) {
    return Status::LackingCodeBuffer("Buffer size:%lu is lacking.", buffer_size);
  }
  memset(code, 0, buffer_size);
  code->cfg = cfg_;
  code->len = cfg_.image_size * cfg_.image_size;

  std::vector<cv::Mat> gabor_filter_result;
  gabor_filter_->Handle(palmprint, &gabor_filter_result);

  PalmprintCodeMetadata *metadata_ptr = code->data;
  for (uint8_t h = 0; h < cfg_.image_size; ++h) {
    for (uint8_t w = 0; w < cfg_.image_size; ++w) {
      uint8_t direction = GetDirectionOfMaxResponse(gabor_filter_result, h, w);
      metadata_ptr->d = direction;

      uint8_t s_left = direction == cfg_.gabor_directions - 1 ? 0 : direction + 1;
      uint8_t s_right = direction == 0 ? cfg_.gabor_directions - 1 : direction - 1;
      double left_direction_response = gabor_filter_result[s_left].ptr<double>(h)[w];
      double right_direction_response = gabor_filter_result[s_right].ptr<double>(h)[w];
      metadata_ptr->s = left_direction_response >= right_direction_response ? 1 : 0;
      ++metadata_ptr;
    }
  }
  return Status::Ok();
}

inline uint8_t CoreEncoder::GetDirectionOfMaxResponse(const std::vector<cv::Mat> &gabor_filter_result, uint8_t x,
                                                      uint8_t y) const {
  double max_response = -DBL_MAX;
  uint8_t direction_of = UINT8_MAX;
  for (uint8_t d = 0; d < cfg_.gabor_directions; ++d) {
    double response = gabor_filter_result[d].ptr<double>(x)[y];
    if (response > max_response) {
      max_response = response;
      direction_of = d;
    }
  }
  return direction_of;
}

}  // namespace edcc