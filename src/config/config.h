// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CONFIG_CONFIG_H_
#define EDCC_SRC_CONFIG_CONFIG_H_

#include <stdint.h>

namespace edcc {

typedef struct {
  uint8_t image_size;  // resize original palm image to (x, x)
  uint8_t gabor_kernel_size;
  uint8_t laplace_kernel_size;
  uint8_t gabor_directions;
} EdccConfig;

static const EdccConfig kDefaultEdccConfig = {29, 5, 5, 10};

}  // namespace edcc

#endif  //  EDCC_SRC_CONFIG_CONFIG_H_