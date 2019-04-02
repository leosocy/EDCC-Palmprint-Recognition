// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <stdlib.h>
#include "config/config.h"
#include "core/code.h"
#include "core/encoder.h"
#include "edcc/status.h"
#include "test_base.h"

namespace {

using edcc::Encoder;
using edcc::kDefaultEncoderConfig;
using edcc::PalmprintCode;
using edcc::PalmprintCodeMetadata;
using edcc::Status;

class EncoderTestFixture : public EdccTestFixtureBase {
 public:
  EncoderTestFixture() : encoder_(kDefaultEncoderConfig) {}
  virtual void SetUp() { EdccTestFixtureBase::SetUp(); }

 protected:
  Encoder encoder_;
};

TEST_F(EncoderTestFixture, get_code_buffer_size) {
  const Encoder& encoder = encoder_;
  EXPECT_EQ(encoder.GetCodeBufferSize(), sizeof(PalmprintCode) + kDefaultEncoderConfig.image_size *
                                                                     kDefaultEncoderConfig.image_size *
                                                                     sizeof(PalmprintCodeMetadata));
}

TEST_F(EncoderTestFixture, lacking_code_buffer_size) {
  const Encoder& encoder = encoder_;
  PalmprintCode* code = (PalmprintCode*)malloc(encoder.GetCodeBufferSize());
  auto status = encoder.Encode(a_01_, code, 1);
  EXPECT_EQ(status.code(), Status::kLackingCodeBuffer);
}

TEST_F(EncoderTestFixture, correctly_encode) {
  const Encoder& encoder = encoder_;
  size_t buffer_size = encoder.GetCodeBufferSize();
  PalmprintCode* code = (PalmprintCode*)malloc(buffer_size);
  auto status = encoder.Encode(a_01_, code, buffer_size);
  EXPECT_EQ(status.code(), Status::kOk);
  EXPECT_TRUE(memcmp(&code->cfg, &kDefaultEncoderConfig, sizeof(code->cfg)) == 0);
  EXPECT_EQ(code->len, kDefaultEncoderConfig.image_size * kDefaultEncoderConfig.image_size);
  for (uint32_t offset = 0; offset < code->len; ++offset) {
    PalmprintCodeMetadata* metadata = code->data + offset;
    EXPECT_TRUE(metadata->d >= 0 && metadata->d < kDefaultEncoderConfig.gabor_directions);
    EXPECT_TRUE(metadata->s == 0 || metadata->s == 1);
    EXPECT_TRUE(metadata->_pad == 0);
  }
}

}  // namespace