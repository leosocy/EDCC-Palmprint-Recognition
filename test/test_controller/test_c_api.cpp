// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "edcc/c_api.h"
#include "test_base.h"

namespace {

class CApiTestFixture : public EdccTestFixtureBase {
 public:
  virtual void SetUp() {
    EdccTestFixtureBase::SetUp();
    cv::imencode(".png", a_01_, a_01_bytes_);
    cv::imencode(".png", b_01_, b_01_bytes_);
  }

 protected:
  std::vector<unsigned char> a_01_bytes_;
  std::vector<unsigned char> b_01_bytes_;
};

TEST_F(CApiTestFixture, compare_two_palmprint) {
  char status[128];
  int eid = new_encoder_with_config(29, 5, 5, 10, status);
  EXPECT_EQ(status[0], '\0');
  EXPECT_EQ(eid, 0);  // is first at encoders.
  status[0] = 1;
  unsigned long code_buffer_size = get_size_of_code_buffer_required(eid);
  char* a_01_code_bytes = new char[code_buffer_size];
  char* b_01_code_bytes = new char[code_buffer_size];
  encode_palmprint_bytes(eid, (char*)a_01_bytes_.data(), a_01_bytes_.size(), a_01_code_bytes, code_buffer_size, status);
  EXPECT_EQ(status[0], 0);
  status[0] = 1;
  encode_palmprint_bytes(eid, (char*)b_01_bytes_.data(), b_01_bytes_.size(), b_01_code_bytes, code_buffer_size, status);
  EXPECT_EQ(status[0], 0);
  status[0] = 1;
  double score = .0;
  score = calculate_codes_similarity(a_01_code_bytes, b_01_code_bytes, status);
  EXPECT_EQ(status[0], 0);
  EXPECT_GT(score, 0.0);
  delete[] a_01_code_bytes;
  delete[] b_01_code_bytes;
}

}  // namespace
