// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_SRC_UTIL_UTIL_H_
#define EDCC_SRC_UTIL_UTIL_H_

#define INVOKE_FUNC_WITH_STATUS(caller) \
  do {                                  \
    auto status = caller;               \
    if (!status.IsOk()) {               \
      return status;                    \
    }                                   \
  } while (0)

#endif  // EDCC_SRC_UTIL_UTIL_H_