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
  typedef size_t eid;  // encoder id represents the position in the vector.

  static EdccFacade* Instance();
  void ClearEncoders();

  EdccFacade::eid NewEncoderWithDefaultConfig(Status* s);
  EdccFacade::eid NewEncoderWithConfig(uint8_t image_size, uint8_t gabor_kernel_size, uint8_t laplace_kernel_size,
                                       uint8_t gabor_directions, Status* s);

  size_t GetSizeOfCodeBufferRequired(EdccFacade::eid id);
  void EncodePalmprint(EdccFacade::eid id, const cv::Mat& palmprint, uint8_t* code_buffer, size_t buffer_size,
                       Status* s);
  void EncodePalmprint(EdccFacade::eid id, const std::string& filename, uint8_t* code_buffer, size_t buffer_size,
                       Status* s);

  double CalcCodeSimilarity(const uint8_t* lhs_code_buffer, const uint8_t* rhs_code_buffer, Status* s);

 private:
  std::vector<std::unique_ptr<Encoder>> encoders_;
};
}  // namespace edcc

#endif  // EDCC_INCLUDE_EDCC_FACADE_H_