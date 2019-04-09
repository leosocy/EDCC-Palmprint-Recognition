// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_INCLUDE_EDCC_FACADE_H_
#define EDCC_INCLUDE_EDCC_FACADE_H_

#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "edcc/status.h"

namespace edcc {

// Forward Declare
class Encoder;

class EdccFacade {
 public:
  typedef int eid;  // encoder id represents the position in the vector.

  EdccFacade(const EdccFacade&) = delete;
  EdccFacade& operator=(const EdccFacade&) = delete;
  EdccFacade(const EdccFacade&&) = delete;
  EdccFacade& operator=(const EdccFacade&&) = delete;

  static EdccFacade* Instance();
  void ClearEncoders();

  EdccFacade::eid NewEncoderWithDefaultConfig(Status* s);
  // will return -1 if config invalid.
  EdccFacade::eid NewEncoderWithConfig(uint8_t image_size, uint8_t gabor_kernel_size, uint8_t laplace_kernel_size,
                                       uint8_t gabor_directions, Status* s);

  size_t GetSizeOfCodeBufferRequired(EdccFacade::eid id);
  void EncodePalmprint(EdccFacade::eid id, const cv::Mat& palmprint, char* code_buffer, size_t buffer_size, Status* s);
  void EncodePalmprint(EdccFacade::eid id, const std::string& filename, char* code_buffer, size_t buffer_size,
                       Status* s);

  double CalcCodeSimilarity(const char* lhs_code_buffer, const char* rhs_code_buffer, Status* s);

 private:
  EdccFacade();

  std::vector<std::unique_ptr<Encoder>> encoders_;
};
}  // namespace edcc

#endif  // EDCC_INCLUDE_EDCC_FACADE_H_