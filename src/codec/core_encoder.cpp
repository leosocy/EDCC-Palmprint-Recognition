// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "codec/core_encoder.h"
#include <cmath>
#include <vector>

namespace edcc {
namespace {
class GaborFilter {
 public:
  GaborFilter(uint8_t kernel_size, uint8_t directions) {
    kernel_size_ = kernel_size;
    directions_ = directions;
    InitKernels();
  }
  void Handle(const cv::Mat &src, cv::Mat *merge);

 private:
  uint8_t kernel_size_;
  uint8_t directions_;
  std::vector<cv::Mat> kernels_;
  void InitKernels();
  void GetKernelReal(cv::Mat *kernel, int width, int height, int dimension,
                     int direction, double kmax = CV_PI / 2,
                     double f = sqrt(2.0), double sigma = 2 * CV_PI,
                     int ktype = CV_64F);
};

void GaborFilter::Handle(const cv::Mat &src, cv::Mat *merge) {
  assert(merge);
  std::vector<cv::Mat> dest_vec;
  cv::Mat dst;
  for (uint8_t d = 0; d < directions_; ++d) {
    filter2D(src, dst, CV_64F, kernels_[d]);
    dest_vec.emplace_back(dst);
  }
  cv::merge(dest_vec, *merge);
  cv::normalize(*merge, *merge, 0, 1, CV_MINMAX);
}

void GaborFilter::InitKernels() {
  cv::Mat kernel;
  for (uint8_t d = 0; d < directions_; ++d) {
    GetKernelReal(&kernel, kernel_size_, kernel_size_, 0, d);
    kernels_.emplace_back(kernel);
  }
}

void GaborFilter::GetKernelReal(cv::Mat *kernel, int width, int height,
                                int dimension, int direction, double kmax,
                                double f, double sigma, int ktype) {
  assert(ktype == CV_32F || ktype == CV_64F);

  int half_width = width / 2;
  int half_height = height / 2;
  double Qu = CV_PI * direction / directions_;
  double sqsigma = sigma * sigma;
  double Kv = kmax / pow(f, dimension);
  double postmean = exp(-sqsigma / 2);
  kernel->create(width, height, ktype);
  double tmp1, tmp2;
  for (int i = -half_height; i <= half_height; ++i) {
    for (int j = -half_width; j <= half_width; ++j) {
      tmp1 = exp(-(Kv * Kv * (i * i + j * j)) / (2 * sqsigma));
      tmp2 = cos(Kv * cos(Qu) * j + Kv * sin(Qu) * i) - postmean;
      if (ktype == CV_32F) {
        kernel->at<float>(i + half_height, j + half_width) =
            (float)(Kv * Kv * tmp1 * tmp2 / sqsigma);
      } else {
        kernel->at<double>(i + half_height, j + half_width) =
            (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
      }
    }
  }
}
}  // namespace

CoreEncoder::CoreEncoder(std::shared_ptr<ConfigReader> reader)
    : config_reader_(reader) {}

size_t CoreEncoder::GetCodeBufferSize() {
  auto config = config_reader_->GetCoreEncoderConfig();
  uint32_t code_meta_len = config.image_size * config.image_size;
  return sizeof(PalmprintCode) + code_meta_len;
}

Status CoreEncoder::BuildAndInitEncoder() {}

Status CoreEncoder::Encode(const cv::Mat &palmprint, PalmprintCode *code) {}

}  // namespace edcc