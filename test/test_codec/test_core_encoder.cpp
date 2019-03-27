// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <gtest/gtest.h>
#include "codec/code.h"

namespace {

TEST(PalmCodeMetadataTest, base_test) {
  printf("PalmprintCodeMetadata size:%d\n",
         sizeof(edcc::PalmprintCodeMetadata));
  printf("ReliablePalmprintCode size:%d\n",
         sizeof(edcc::ReliablePalmprintCode));
  printf("FastPalmprintCode size:%d\n", sizeof(edcc::FastPalmprintCode));
}

}  // namespace