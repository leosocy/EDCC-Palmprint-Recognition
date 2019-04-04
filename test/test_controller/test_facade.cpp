// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "edcc/facade.h"
#include "test_base.h"

namespace {

using edcc::EdccFacade;
using edcc::Status;

class EdccFacadeTestFixture : public EdccTestFixtureBase {
 public:
  virtual void TearDown() { EdccFacade::Instance()->ClearEncoders(); }
};

TEST_F(EdccFacadeTestFixture, new_encoder_with_invalid_config) {
  Status s;
  EdccFacade::eid id = EdccFacade::Instance()->NewEncoderWithConfig(29, 5, 5, 66, &s);
  EXPECT_EQ(id, -1);
  EXPECT_EQ(s.code(), Status::kInvalidArgument);
}

TEST_F(EdccFacadeTestFixture, compare_two_palmprint_with_same_encoder) {
  Status s;
  EdccFacade::eid id = EdccFacade::Instance()->NewEncoderWithDefaultConfig(&s);
  ASSERT_TRUE(s.IsOk());
  size_t buffer_size = EdccFacade::Instance()->GetSizeOfCodeBufferRequired(id);
  char* code_a_01_buffer = new char[buffer_size];
  char* code_a_02_buffer = new char[buffer_size];
  EdccFacade::Instance()->EncodePalmprint(id, a_01_, code_a_01_buffer, buffer_size, &s);
  ASSERT_TRUE(s.IsOk());
  EdccFacade::Instance()->EncodePalmprint(id, TEST_A_02_PALMPRINT_IMAGE, code_a_02_buffer, buffer_size, &s);
  ASSERT_TRUE(s.IsOk());
  double score;
  score = EdccFacade::Instance()->CalcCodeSimilarity(code_a_01_buffer, code_a_02_buffer, &s);
  ASSERT_TRUE(s.IsOk());
  EXPECT_GT(score, 0.0);
}

TEST_F(EdccFacadeTestFixture, compare_two_palmprint_with_different_encoder) {
  Status s, s1, s2;
  EdccFacade::eid id1 = EdccFacade::Instance()->NewEncoderWithDefaultConfig(&s1);
  EdccFacade::eid id2 = EdccFacade::Instance()->NewEncoderWithConfig(29, 5, 5, 12, &s2);
  size_t buffer_size1 = EdccFacade::Instance()->GetSizeOfCodeBufferRequired(id1);
  char* code_a_01_buffer = new char[buffer_size1];
  size_t buffer_size2 = EdccFacade::Instance()->GetSizeOfCodeBufferRequired(id2);
  char* code_a_02_buffer = new char[buffer_size2];
  EdccFacade::Instance()->EncodePalmprint(id1, a_01_, code_a_01_buffer, buffer_size1, &s1);
  EdccFacade::Instance()->EncodePalmprint(id2, TEST_A_02_PALMPRINT_IMAGE, code_a_02_buffer, buffer_size2, &s2);
  double score;
  score = EdccFacade::Instance()->CalcCodeSimilarity(code_a_01_buffer, code_a_02_buffer, &s);
  EXPECT_EQ(s.code(), Status::kCodeCfgNEWhenComparing);
}

}  // namespace