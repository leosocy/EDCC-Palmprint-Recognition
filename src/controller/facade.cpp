// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "edcc/facade.h"
#include "config/config.h"
#include "config/reader.h"
#include "core/comparer.h"
#include "core/encoder.h"

namespace edcc {

EdccFacade* EdccFacade::Instance() {
  static EdccFacade* instance = new EdccFacade;
  return instance;
}

void EdccFacade::ClearEncoders() { encoders_.clear(); }

EdccFacade::eid EdccFacade::NewEncoderWithDefaultConfig(Status* s) {
  const EncoderConfig& cfg = kDefaultEncoderConfig;
  return NewEncoderWithConfig(cfg.image_size, cfg.gabor_kernel_size, cfg.laplace_kernel_size, cfg.gabor_directions, s);
}

EdccFacade::eid EdccFacade::NewEncoderWithConfig(uint8_t image_size, uint8_t gabor_kernel_size,
                                                 uint8_t laplace_kernel_size, uint8_t gabor_directions, Status* s) {
  EncoderConfig cfg;
  cfg.image_size = image_size;
  cfg.gabor_kernel_size = gabor_kernel_size;
  cfg.laplace_kernel_size = laplace_kernel_size;
  cfg.gabor_directions = gabor_directions;
  std::unique_ptr<ConfigReader> reader = std::unique_ptr<ConfigReader>(new SimpleConfigReader(cfg));
  *s = reader->LoadAndValidate();
  std::unique_ptr<Encoder> encoder = std::unique_ptr<Encoder>(new Encoder(reader->GetEncoderConfig()));
  encoders_.emplace_back(std::move(encoder));
  return encoders_.size() - 1;
}

size_t EdccFacade::GetSizeOfCodeBufferRequired(EdccFacade::eid id) { return encoders_.at(id)->GetCodeBufferSize(); }

void EdccFacade::EncodePalmprint(EdccFacade::eid id, const cv::Mat& palmprint, uint8_t* code_buffer, size_t buffer_size,
                                 Status* s) {
  PalmprintCode* code = reinterpret_cast<PalmprintCode*>(code_buffer);
  *s = encoders_.at(id)->Encode(palmprint, code, buffer_size);
}

void EdccFacade::EncodePalmprint(EdccFacade::eid id, const std::string& filename, uint8_t* code_buffer,
                                 size_t buffer_size, Status* s) {
  cv::Mat palmprint = cv::imread(filename);
  EncodePalmprint(id, palmprint, code_buffer, buffer_size, s);
}

double EdccFacade::CalcCodeSimilarity(const uint8_t* lhs_code_buffer, const uint8_t* rhs_code_buffer, Status* s) {
  const PalmprintCode* lhs_code = reinterpret_cast<const PalmprintCode*>(lhs_code_buffer);
  const PalmprintCode* rhs_code = reinterpret_cast<const PalmprintCode*>(rhs_code_buffer);
  double score = .0;
  *s = Comparer::Compare(*lhs_code, *rhs_code, &score);
  return score;
}

}  // namespace edcc