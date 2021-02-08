// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "edcc/c_api.h"
#include "edcc/facade.h"

using edcc::EdccFacade;
using edcc::Status;

int new_encoder_with_config(unsigned char image_size, unsigned char gabor_kernel_size,
                            unsigned char laplace_kernel_size, unsigned char gabor_directions, char* status_ptr) {
  Status s;
  int eid = EdccFacade::Instance()->NewEncoderWithConfig(image_size, gabor_kernel_size, laplace_kernel_size,
                                                         gabor_directions, &s);
  s.CopyToBuffer(status_ptr);
  return eid;
}

unsigned long get_size_of_code_buffer_required(int eid) {
  return EdccFacade::Instance()->GetSizeOfCodeBufferRequired(eid);
}

void encode_palmprint_using_file(int eid, const char* filepath, char* code_bytes, unsigned long code_bytes_size,
                                 char* status_ptr) {
  Status s;
  EdccFacade::Instance()->EncodePalmprint(eid, filepath, code_bytes, code_bytes_size, &s);
  s.CopyToBuffer(status_ptr);
}

void encode_palmprint_using_bytes(int eid, const char* palmprint_bytes, unsigned long palmprint_bytes_size,
                                  char* code_bytes, unsigned long code_bytes_size, char* status_ptr) {
  Status s;
  std::vector<unsigned char> vp_bytes(palmprint_bytes_size);
  memcpy(vp_bytes.data(), palmprint_bytes, palmprint_bytes_size);
  cv::Mat palmprint(cv::imdecode(vp_bytes, 1));
  EdccFacade::Instance()->EncodePalmprint(eid, palmprint, code_bytes, code_bytes_size, &s);
  s.CopyToBuffer(status_ptr);
}

double calculate_codes_similarity(char* lhs_code_bytes, char* rhs_code_bytes, char* status_ptr) {
  Status s;
  double score;
  score = EdccFacade::CalcCodeSimilarity(lhs_code_bytes, rhs_code_bytes, &s);
  s.CopyToBuffer(status_ptr);
  return score;
}
