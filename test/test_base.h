// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#pragma once

#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

#define TEST_PALMPRINT_DATA_DIR "../../test/palmprint_data"

#define TEST_A_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_01.bmp"
#define TEST_A_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_02.bmp"
#define TEST_B_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_01.bmp"
#define TEST_B_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_02.bmp"

class EdccTestFixtureBase : public testing::Test {
 public:
  EdccTestFixtureBase() {}
  virtual void SetUp() {
    a_01_ = cv::imread(TEST_A_01_PALMPRINT_IMAGE);
    a_02_ = cv::imread(TEST_A_02_PALMPRINT_IMAGE);
    b_01_ = cv::imread(TEST_B_01_PALMPRINT_IMAGE);
    b_02_ = cv::imread(TEST_B_02_PALMPRINT_IMAGE);
  }
  virtual void TearDown() {
    a_01_.release();
    a_02_.release();
    b_01_.release();
    b_02_.release();
  }

 protected:
  cv::Mat a_01_;
  cv::Mat a_02_;
  cv::Mat b_01_;
  cv::Mat b_02_;
};