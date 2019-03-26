// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <gtest/gtest.h>
#include "config/reader.h"

namespace {

using edcc::SimpleConfigReader;
using edcc::Status;

TEST(ConfigReaderTest, test_simple_config_reader) {
  auto reader = SimpleConfigReader(edcc::kDefaultEdccConfig);
  auto status = reader.Load();
  EXPECT_TRUE(status.IsOk());
  auto config = reader.GetConfig();
  EXPECT_EQ(config.image_size, edcc::kDefaultEdccConfig.image_size);
}
}  // namespace