// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <stdlib.h>
#include "config/config.h"
#include "config/reader.h"
#include "core/code.h"
#include "core/comparer.h"
#include "core/encoder.h"
#include "edcc/status.h"
#include "test_base.h"

namespace {

using edcc::Comparer;
using edcc::ConfigReader;
using edcc::Encoder;
using edcc::kDefaultEncoderConfig;
using edcc::PalmprintCode;
using edcc::PalmprintCodeMetadata;
using edcc::Status;

class ComparerTestFixture : public EdccTestFixtureBase {
 public:
  virtual void SetUp() {
    EdccTestFixtureBase::SetUp();
    config_reader_ = std::unique_ptr<ConfigReader>(new edcc::SimpleConfigReader(kDefaultEncoderConfig));
    config_reader_->LoadAndValidate();
    Encoder encoder(config_reader_->GetEncoderConfig());
    size_t buffer_size = encoder.GetCodeBufferSize();
    a_01_code_ = (PalmprintCode*)malloc(buffer_size);
    a_02_code_ = (PalmprintCode*)malloc(buffer_size);
    b_01_code_ = (PalmprintCode*)malloc(buffer_size);
    b_02_code_ = (PalmprintCode*)malloc(buffer_size);
    encoder.Encode(a_01_, a_01_code_, buffer_size);
    encoder.Encode(a_02_, a_02_code_, buffer_size);
    encoder.Encode(b_01_, b_01_code_, buffer_size);
    config_reader_->SetGaborDirections(15);
    Encoder encoder1(config_reader_->GetEncoderConfig());
    encoder1.Encode(b_02_, b_02_code_, buffer_size);
  }
  virtual void TearDown() {
    free(a_01_code_);
    free(a_02_code_);
    free(b_01_code_);
    free(b_02_code_);
    EdccTestFixtureBase::TearDown();
  }

 protected:
  std::unique_ptr<ConfigReader> config_reader_;
  PalmprintCode* a_01_code_;
  PalmprintCode* a_02_code_;
  PalmprintCode* b_01_code_;
  PalmprintCode* b_02_code_;
};

TEST_F(ComparerTestFixture, two_codes_have_differect_cfg) {
  double score = 0;
  auto status = Comparer::Compare(*a_01_code_, *b_02_code_, &score);
  EXPECT_EQ(status.code(), Status::kCodeCfgNEWhenComparing);
}

TEST_F(ComparerTestFixture, correctly_compare) {
  double score_a1a1 = 0, score_a1a2 = 0, score_a2a1 = 0, score_a1b1 = 0;
  auto status = Comparer::Compare(*a_01_code_, *a_01_code_, &score_a1a1);
  EXPECT_EQ(status.code(), Status::kOk);
  status = Comparer::Compare(*a_01_code_, *a_02_code_, &score_a1a2);
  EXPECT_EQ(status.code(), Status::kOk);
  status = Comparer::Compare(*a_02_code_, *a_01_code_, &score_a2a1);
  EXPECT_EQ(status.code(), Status::kOk);
  status = Comparer::Compare(*a_01_code_, *b_01_code_, &score_a1b1);
  EXPECT_EQ(status.code(), Status::kOk);
  EXPECT_EQ(score_a1a1, 1.0);
  EXPECT_GT(score_a1b1, 0.0);
  EXPECT_LT(score_a1b1, score_a2a1);
  EXPECT_EQ(score_a1a2, score_a2a1);
}

}  // namespace