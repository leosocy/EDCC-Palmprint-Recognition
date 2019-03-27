// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <gtest/gtest.h>
#include "config/reader.h"

namespace {

using edcc::EdccConfig;
using edcc::kDefaultEdccConfig;
using edcc::SimpleConfigReader;
using edcc::Status;

TEST(SimpleConfigReaderTest, all_correct) {
  auto reader = SimpleConfigReader(edcc::kDefaultEdccConfig);
  auto status = reader.Load();
  EXPECT_TRUE(status.IsOk());
  auto config = reader.GetConfig();
  EXPECT_EQ(config.image_size, edcc::kDefaultEdccConfig.image_size);
  EXPECT_EQ(config.gabor_kernel_size,
            edcc::kDefaultEdccConfig.gabor_kernel_size);
  EXPECT_EQ(config.laplace_kernel_size,
            edcc::kDefaultEdccConfig.laplace_kernel_size);
  EXPECT_EQ(config.gabor_directions, edcc::kDefaultEdccConfig.gabor_directions);
}

TEST(SimpleConfigReaderTest, invalid_image_size) {
  EdccConfig config = kDefaultEdccConfig;
  config.image_size = edcc::limit::kMinImageSize - 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_gabor_kernel_size) {
  EdccConfig config = kDefaultEdccConfig;
  config.gabor_kernel_size = kDefaultEdccConfig.image_size + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.gabor_kernel_size = 4;
  reader = SimpleConfigReader(config);
  status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_laplace_kernel_size) {
  EdccConfig config = kDefaultEdccConfig;
  config.laplace_kernel_size = kDefaultEdccConfig.image_size + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.laplace_kernel_size = 4;
  reader = SimpleConfigReader(config);
  status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);

  config.laplace_kernel_size = edcc::limit::kMaxLaplaceKernelSize + 1;
  reader = SimpleConfigReader(config);
  status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

TEST(SimpleConfigReaderTest, invalid_gabor_directions) {
  EdccConfig config = kDefaultEdccConfig;
  config.gabor_directions = edcc::limit::kMaxGaborDirections + 1;
  auto reader = SimpleConfigReader(config);
  auto status = reader.Load();
  EXPECT_EQ(status.code(), Status::kInvalidArgument);
}

}  // namespace