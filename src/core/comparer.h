// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_CORE_COMPARER_H_
#define EDCC_SRC_CORE_COMPARER_H_

#include "core/code.h"
#include "edcc/status.h"

namespace edcc {
class Comparer {
 public:
  static Status Compare(const PalmprintCode& lhs_code, const PalmprintCode& rhs_code, double* score);
};
}  // namespace edcc

#endif  // EDCC_SRC_CORE_COMPARER_H_