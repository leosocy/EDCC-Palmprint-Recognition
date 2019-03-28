// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "codec/core_encoder.h"
#include <cmath>
#include <vector>

namespace edcc {

CoreEncoder::CoreEncoder(std::unique_ptr<ConfigReader> reader)
    : config_reader_(std::move(reader)), gabor_filter_(nullptr) {}

Status CoreEncoder::BuildAndInitEncoder() {
  auto status = config_reader_->LoadAndValidate();
  if (status.IsOk()) {
    gabor_filter_ = std::unique_ptr<GaborFilter>(new GaborFilter(config_reader_->GetCoreEncoderConfig()));
  }
  return status;
}

size_t CoreEncoder::GetCodeBufferSize() {
  auto config = config_reader_->GetCoreEncoderConfig();
  uint32_t code_meta_len = config.image_size * config.image_size;
  return sizeof(PalmprintCode) + code_meta_len;
}

Status CoreEncoder::Encode(const cv::Mat &palmprint, PalmprintCode *code) {
  assert(code != nullptr);
  auto cfg = config_reader_->GetCoreEncoderConfig();
  std::vector<cv::Mat> gabor_filter_result;
  gabor_filter_->Handle(palmprint, &gabor_filter_result);

  code->cfg = cfg;
  code->len = GetCodeBufferSize() - sizeof(PalmprintCode);
  memset(code->data, 0, code->len);

  PalmprintCodeMetadata *metadata_ptr = code->data;
  for (int h = 0; h < cfg.image_size; ++h) {
    for (int w = 0; w < cfg.image_size; ++w) {
      double max_response = -DBL_MAX;
      int max_direction = -1;
      int c_left = -1, c_right = -1;
      for (uint8_t d = 0; d < cfg.gabor_directions; ++d) {
        double response = gabor_filter_result[d].at<double>(h, w);
        if (response > max_response) {
          max_response = response;
          max_direction = d;
        }
      }
      metadata_ptr->d = max_direction;
      if (max_direction == cfg.gabor_directions - 1) {
        c_left = 0;
      } else {
        c_left = max_direction + 1;
      }
      if (max_direction == 0) {
        c_right = cfg.gabor_directions - 1;
      } else {
        c_right = max_direction - 1;
      }
      metadata_ptr->s =
          gabor_filter_result[c_left].at<double>(h, w) >= gabor_filter_result[c_right].at<double>(h, w) ? 1 : 0;
      ++metadata_ptr;
    }
  }
}

}  // namespace edcc