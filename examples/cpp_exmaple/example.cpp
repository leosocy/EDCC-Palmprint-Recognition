// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <edcc/facade.h>
#include <edcc/status.h>

using edcc::EdccFacade;
using edcc::Status;

#define TEST_PALMPRINT_DATA_DIR "../../../palmprint_data"

#define TEST_A_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_01.bmp"
#define TEST_A_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_02.bmp"
#define TEST_B_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_01.bmp"
#define TEST_B_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_02.bmp"

#define ASSERT_STATUS_OK(s) \
  do {                      \
    if (!s.IsOk()) {        \
      perror(s.msg());      \
      return -1;            \
    }                       \
  } while (0)

int main() {
  Status s;
  // create a new encoder.
  auto inst = EdccFacade::Instance();
  auto encoder_id = inst->NewEncoderWithConfig(29, 5, 5, 10, &s);
  ASSERT_STATUS_OK(s);
  // encode palmprints to code buffer.
  size_t buffer_size = inst->GetSizeOfCodeBufferRequired(encoder_id);
  char* code_buffer_one = new char[buffer_size];
  char* code_buffer_another = new char[buffer_size];
  inst->EncodePalmprint(encoder_id, TEST_A_01_PALMPRINT_IMAGE, code_buffer_one, buffer_size, &s);
  ASSERT_STATUS_OK(s);
  inst->EncodePalmprint(encoder_id, TEST_B_01_PALMPRINT_IMAGE, code_buffer_another, buffer_size, &s);
  ASSERT_STATUS_OK(s);
  // calculate the similarity score of two codes.
  double score = inst->CalcCodeSimilarity(code_buffer_one, code_buffer_another, &s);
  ASSERT_STATUS_OK(s);
  printf("%s <-> %s similarity score:%lf\n", TEST_A_01_PALMPRINT_IMAGE, TEST_B_01_PALMPRINT_IMAGE, score);
  return 0;
}