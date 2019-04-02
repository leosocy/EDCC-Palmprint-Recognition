// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <gtest/gtest.h>
#include "config/reader.h"

namespace {

using edcc::EncoderConfig;
using edcc::kDefaultEncoderConfig;
using edcc::SimpleConfigReader;
using edcc::Status;

TEST(SimpleConfigReaderTest, all_correct) {
  auto reader = SimpleConfigReader(edcc::kDefaultEncoderConfig);
  auto status = reader.LoadAndValidate();
  EXPECT_TRUE(status.IsOk());
  auto config = reader.GetEncoderConfig();
  EXPECT_EQ(config.image_size, edcc::kDefaultEncoderConfig.image_size);
  EXPECT_EQ(config.gabor_kernel_size, edcc::kDefaultEncoderConfig.gabor_kernel_size);
  EXPECT_EQ(config.laplace_kernel_size, edcc::kDefaultEncoderConfig.laplace_kernel_size);
  EXPECT_EQ(config.gabor_directions, edcc::kDefaultEncoderConfig.gabor_directions);
}

TEST(SimpleConfigReaderTest, invalid_image_size) {
  EncoderConfig config = kDefaultEncoderConfig;
  config.image_size = edcc::limit::kMinImageSize - 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_gabor_kernel_size) {
  EncoderConfig config = kDefaultEncoderConfig;
  config.gabor_kernel_size = kDefaultEncoderConfig.image_size + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.gabor_kernel_size = 4;
  reader = SimpleConfigReader(config);
  status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_laplace_kernel_size) {
  EncoderConfig config = kDefaultEncoderConfig;
  config.laplace_kernel_size = kDefaultEncoderConfig.image_size + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.laplace_kernel_size = 4;
  reader = SimpleConfigReader(config);
  status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.laplace_kernel_size = edcc::limit::kMaxLaplaceKernelSize + 1;
  reader = SimpleConfigReader(config);
  status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_gabor_directions) {
  EncoderConfig config = kDefaultEncoderConfig;
  config.gabor_directions = edcc::limit::kMaxGaborDirections + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.LoadAndValidate();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

}  // namespace