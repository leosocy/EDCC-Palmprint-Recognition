// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <stdlib.h>
#include "codec/code.h"
#include "codec/core_encoder.h"
#include "config/config.h"
#include "edcc/status.h"
#include "test_base.h"

namespace {

using edcc::CoreEncoder;
using edcc::kDefaultCoreEncoderConfig;
using edcc::PalmprintCode;
using edcc::PalmprintCodeMetadata;
using edcc::Status;

class CodecCoreEncoderTestFixture : public EdccTestFixtureBase {
 public:
  CodecCoreEncoderTestFixture() : encoder_(kDefaultCoreEncoderConfig) {}
  virtual void SetUp() { EdccTestFixtureBase::SetUp(); }

 protected:
  CoreEncoder encoder_;
};

TEST_F(CodecCoreEncoderTestFixture, get_code_buffer_size) {
  const CoreEncoder& encoder = encoder_;
  EXPECT_EQ(encoder.GetCodeBufferSize(), sizeof(PalmprintCode) + kDefaultCoreEncoderConfig.image_size *
                                                                     kDefaultCoreEncoderConfig.image_size *
                                                                     sizeof(PalmprintCodeMetadata));
}

TEST_F(CodecCoreEncoderTestFixture, lacking_code_buffer_size) {
  const CoreEncoder& encoder = encoder_;
  PalmprintCode* code = (PalmprintCode*)malloc(encoder.GetCodeBufferSize());
  auto status = encoder.Encode(a_01_, code, 1);
  EXPECT_EQ(status.code(), Status::kLackingCodeBuffer);
}

TEST_F(CodecCoreEncoderTestFixture, correctly_encode) {
  const CoreEncoder& encoder = encoder_;
  size_t buffer_size = encoder.GetCodeBufferSize();
  PalmprintCode* code = (PalmprintCode*)malloc(buffer_size);
  auto status = encoder.Encode(a_01_, code, buffer_size);
  EXPECT_EQ(status.code(), Status::kOk);
  EXPECT_TRUE(memcmp(&code->cfg, &kDefaultCoreEncoderConfig, sizeof(code->cfg)) == 0);
  EXPECT_EQ(code->len, kDefaultCoreEncoderConfig.image_size * kDefaultCoreEncoderConfig.image_size);
  for (uint32_t offset = 0; offset < code->len; ++offset) {
    PalmprintCodeMetadata* metadata = code->data + offset;
    EXPECT_TRUE(metadata->d >= 0 && metadata->d < kDefaultCoreEncoderConfig.gabor_directions);
    EXPECT_TRUE(metadata->s == 0 || metadata->s == 1);
    EXPECT_TRUE(metadata->_pad == 0);
  }
}

}  // namespace