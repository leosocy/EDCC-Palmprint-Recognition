// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "util/status.h"
#include <assert.h>
#include <string.h>

namespace edcc
{

Status::Status(Code code)
{
    char *result = new char[1];
    result[0] = static_cast<char>(code);
    state_ = result;
}

const char* Status::CopyState(const char *state)
{
    assert(state != NULL);
    char *result = new char[1];
    memcpy(result, state, 1);
    return result;
}

} // namespace edcc