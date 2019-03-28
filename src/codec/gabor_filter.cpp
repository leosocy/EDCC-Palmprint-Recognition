// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "codec/gabor_filter.h"

namespace edcc {

GaborFilter::GaborFilter(const CoreEncoderConfig &config) : cfg_(config) { InitKernels(); }

void GaborFilter::Handle(const cv::Mat &src, std::vector<cv::Mat> *result) {
  assert(result);
  cv::Mat enhanced_image(src.clone());
  cv::resize(enhanced_image, enhanced_image, cv::Size(cfg_.image_size, cfg_.image_size));
  cvtColor(enhanced_image, enhanced_image, CV_BGR2GRAY);
  EnhanceImage(enhanced_image, &enhanced_image);
  cv::Mat directional_filter_result;
  for (uint8_t d = 0; d < cfg_.gabor_directions; ++d) {
    filter2D(enhanced_image, directional_filter_result, CV_64F, kernels_[d]);
    cv::normalize(directional_filter_result, directional_filter_result, 0, 1, CV_MINMAX);
    result->emplace_back(directional_filter_result);
  }
}

void GaborFilter::InitKernels() {
  cv::Mat directional_kernel;
  for (uint8_t d = 0; d < cfg_.gabor_directions; ++d) {
    GetKernelReal(&directional_kernel, cfg_.gabor_kernel_size, cfg_.gabor_kernel_size, 0, d);
    kernels_.emplace_back(directional_kernel);
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
      kernel->at<double>(i + half_height, j + half_width) = (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
    }
  }
}

void GaborFilter::EnhanceImage(const cv::Mat &src, cv::Mat *dst) {
  cv::Mat gaussian_result;
  GaussianBlur(src, gaussian_result, cv::Size(5, 5), 0, 0, cv::BORDER_DEFAULT);
  Laplacian(gaussian_result, *dst, CV_64F, cfg_.laplace_kernel_size);
}

}  // namespace edcc