// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "config/reader.h"
#include "util/util.h"

namespace edcc {

Status ConfigReader::SetImageSize(uint8_t size) {
  if (size < limit::kMinImageSize) {
    return Status::InvalidArgument("Image size less than minimum limit.");
  }
  core_encoder_cfg_.image_size = size;
  return Status::Ok();
}

Status ConfigReader::SetGaborKernelSize(uint8_t size) {
  if (size > core_encoder_cfg_.image_size) {
    return Status::InvalidArgument("Gabor kernel size larger than image size.");
  }
  if ((size & 0x01) == 0) {
    return Status::InvalidArgument("Gabor kernel size not odd.");
  }
  core_encoder_cfg_.gabor_kernel_size = size;
  return Status::Ok();
}

Status ConfigReader::SetLaplaceKernelSize(uint8_t size) {
  if (size > core_encoder_cfg_.image_size || size > limit::kMaxLaplaceKernelSize) {
    return Status::InvalidArgument("Laplace kernel size larger than image size or %d.", limit::kMaxLaplaceKernelSize);
  }
  if ((size & 0x01) == 0) {
    return Status::InvalidArgument("Laplace kernel size not odd.");
  }
  core_encoder_cfg_.laplace_kernel_size = size;
  return Status::Ok();
}

Status ConfigReader::SetGaborDirecions(uint8_t num) {
  if (num > limit::kMaxGaborDirections || num < limit::kMinGaborDirections) {
    return Status::InvalidArgument("Gabor directions not in range [%d, %d].", limit::kMinGaborDirections,
                                   limit::kMaxGaborDirections);
  }
  core_encoder_cfg_.gabor_directions = num;
  return Status::Ok();
}

SimpleConfigReader::SimpleConfigReader(const CoreEncoderConfig& config) { core_encoder_cfg_ = config; }

Status SimpleConfigReader::LoadAndValidate() {
  INVOKE_FUNC_WITH_STATUS(SetImageSize(core_encoder_cfg_.image_size));
  INVOKE_FUNC_WITH_STATUS(SetGaborKernelSize(core_encoder_cfg_.gabor_kernel_size));
  INVOKE_FUNC_WITH_STATUS(SetLaplaceKernelSize(core_encoder_cfg_.laplace_kernel_size));
  INVOKE_FUNC_WITH_STATUS(SetGaborDirecions(core_encoder_cfg_.gabor_directions));
  return Status::Ok();
}

}  // namespace edcc