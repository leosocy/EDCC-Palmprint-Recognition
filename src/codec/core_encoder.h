// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CODEC_CORE_ENCODER_H_
#define EDCC_SRC_CODEC_CORE_ENCODER_H_

#include <memory>
#include <opencv2/opencv.hpp>
#include "codec/code.h"
#include "codec/gabor_filter.h"
#include "config/config.h"
#include "edcc/status.h"

namespace edcc {

class CoreEncoder {
 public:
  CoreEncoder(const CoreEncoderConfig& cfg);

  size_t GetCodeBufferSize() const;
  Status Encode(const cv::Mat& palmprint, PalmprintCode* code, size_t buffer_size) const;

 private:
  uint8_t GetDirectionOfMaxResponse(const std::vector<cv::Mat>& gabor_filter_result, uint8_t x, uint8_t y) const;

  const CoreEncoderConfig cfg_;
  std::unique_ptr<GaborFilter> gabor_filter_;
};

}  // namespace edcc

#endif  // EDCC_SRC_CODEC_CORE_ENCODER_H_