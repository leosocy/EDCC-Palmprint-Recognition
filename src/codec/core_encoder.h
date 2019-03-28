// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CODEC_CORE_ENCODER_H_
#define EDCC_SRC_CODEC_CORE_ENCODER_H_

#include <memory>
#include <opencv2/opencv.hpp>
#include "codec/code.h"
#include "codec/gabor_filter.h"
#include "config/reader.h"

namespace edcc {

class CoreEncoder {
 public:
  CoreEncoder(std::unique_ptr<ConfigReader> reader);
  Status BuildAndInitEncoder();
  size_t GetCodeBufferSize();
  Status Encode(const cv::Mat& palmprint, PalmprintCode* code);
  Status Encode(const char* filename, PalmprintCode* code);

 private:
  std::unique_ptr<ConfigReader> config_reader_;
  std::unique_ptr<GaborFilter> gabor_filter_;
};
}  // namespace edcc

#endif  // EDCC_SRC_CODEC_CORE_ENCODER_H_