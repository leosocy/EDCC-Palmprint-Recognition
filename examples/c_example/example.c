// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <edcc/c_api.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_PALMPRINT_DATA_DIR "../../../palmprint_data"

#define TEST_A_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_01.bmp"
#define TEST_A_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/a_02.bmp"
#define TEST_B_01_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_01.bmp"
#define TEST_B_02_PALMPRINT_IMAGE TEST_PALMPRINT_DATA_DIR "/b_02.bmp"

#define ASSERT_STATUS_OK(s) \
  do {                      \
    if (s[0] != '\0') {     \
      perror(s + 1);        \
      return -1;            \
    }                       \
  } while (0)

int main() {
  // create a new encoder.
  char status[128];
  int encoder_id = new_encoder_with_config(29, 5, 5, 10, status);
  ASSERT_STATUS_OK(status);
  // encode palmprints to code buffer.
  unsigned long buffer_size = get_size_of_code_buffer_required(encoder_id);
  char* code_buffer_one = (char*)malloc(buffer_size);
  char* code_buffer_another = (char*)malloc(buffer_size);
  encode_palmprint_using_file(encoder_id, TEST_A_01_PALMPRINT_IMAGE, code_buffer_one, buffer_size, status);
  ASSERT_STATUS_OK(status);
  encode_palmprint_using_file(encoder_id, TEST_B_01_PALMPRINT_IMAGE, code_buffer_another, buffer_size, status);
  ASSERT_STATUS_OK(status);
  // calculate the similarity score of two codes.
  double score = calculate_codes_similarity(code_buffer_one, code_buffer_another, status);
  ASSERT_STATUS_OK(status);
  printf("%s <-> %s similarity score:%lf\n", TEST_A_01_PALMPRINT_IMAGE, TEST_B_01_PALMPRINT_IMAGE, score);
  free(code_buffer_one);
  free(code_buffer_another);
  return 0;
}