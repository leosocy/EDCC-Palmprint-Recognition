// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CORE_GABOR_FILTER_H_
#define EDCC_SRC_CORE_GABOR_FILTER_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "config/config.h"

namespace edcc {
class GaborFilter {
 public:
  GaborFilter(const EncoderConfig &config);
  void Handle(const cv::Mat &src, std::vector<cv::Mat> *result);

 private:
  // init gabor filter kernels in different directions.
  void InitKernels();
  void GetKernelReal(cv::Mat *kernel, int width, int height, int dimension, int direction, double kmax = CV_PI / 2,
                     double f = sqrt(2.0), double sigma = 2 * CV_PI);
  void PreProcessImage(const cv::Mat &src, cv::Mat *result);
  void EnhanceImage(const cv::Mat &src, cv::Mat *result);
  const EncoderConfig &cfg_;
  std::vector<cv::Mat> kernels_;
};
}  // namespace edcc

#endif  // EDCC_SRC_CORE_GABOR_FILTER_H_