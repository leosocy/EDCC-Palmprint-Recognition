// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "core/gabor_filter.h"

namespace edcc {

GaborFilter::GaborFilter(const EncoderConfig &config) : cfg_(config) { InitKernels(); }

void GaborFilter::Handle(const cv::Mat &src, std::vector<cv::Mat> *result) {
  assert(result);
  cv::Mat enhanced_image;
  PreProcessImage(src, &enhanced_image);
  EnhanceImage(enhanced_image, &enhanced_image);
  for (uint8_t d = 0; d < cfg_.gabor_directions; ++d) {
    cv::Mat directional_filter_result;
    filter2D(enhanced_image, directional_filter_result, CV_64F, kernels_[d]);
    result->push_back(directional_filter_result.clone());
  }
}

void GaborFilter::InitKernels() {
  for (uint8_t d = 0; d < cfg_.gabor_directions; ++d) {
    cv::Mat directional_kernel;
    GetKernelReal(&directional_kernel, cfg_.gabor_kernel_size, cfg_.gabor_kernel_size, 0, d);
    kernels_.push_back(directional_kernel.clone());
  }
}

void GaborFilter::GetKernelReal(cv::Mat *kernel, int width, int height, int dimension, int direction, double kmax,
                                double f, double sigma) {
  int half_width = width / 2;
  int half_height = height / 2;
  double Qu = CV_PI * direction / cfg_.gabor_directions;
  double sqsigma = sigma * sigma;
  double Kv = kmax / pow(f, dimension);
  double postmean = exp(-sqsigma / 2);
  kernel->create(width, height, CV_64F);
  double tmp1, tmp2;
  for (int i = -half_height; i <= half_height; ++i) {
    for (int j = -half_width; j <= half_width; ++j) {
      tmp1 = exp(-(Kv * Kv * (i * i + j * j)) / (2 * sqsigma));
      tmp2 = cos(Kv * cos(Qu) * j + Kv * sin(Qu) * i) - postmean;
      kernel->ptr<double>(i + half_height)[j + half_width] = Kv * Kv * tmp1 * tmp2 / sqsigma;
    }
  }
}

void GaborFilter::PreProcessImage(const cv::Mat &src, cv::Mat *result) {
  assert(result);
  cv::Mat resized;
  cv::resize(src, resized, cv::Size(cfg_.image_size, cfg_.image_size));
  cvtColor(resized, *result, CV_BGR2GRAY);
}

void GaborFilter::EnhanceImage(const cv::Mat &src, cv::Mat *result) {
  assert(result);
  cv::Mat gaussian;
  GaussianBlur(src, gaussian, cv::Size(5, 5), 0, 0, cv::BORDER_DEFAULT);
  Laplacian(gaussian, *result, CV_64F, cfg_.laplace_kernel_size);
}

}  // namespace edcc