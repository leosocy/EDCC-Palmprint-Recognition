// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include "config/reader.h"

namespace edcc {

SimpleConfigReader::SimpleConfigReader(const EdccConfig& config) {
  cfg_ = config;
}

Status SimpleConfigReader::Load() { return Status::Ok(); }

}  // namespace edcc