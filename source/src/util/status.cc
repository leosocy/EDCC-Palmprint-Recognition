// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "util/status.h"
#include "edcc.h"
#include <assert.h>
#include <string.h>

namespace edcc
{

static struct StatusCode2ExternCode
{
    Status::Code code;
    int ext_code;
} transform_table[] = {
    {Status::kOk,                       EDCC_SUCCESS},
    {Status::kNullPtrError,             EDCC_NULL_POINTER_ERROR},
    {Status::kLoadConfigError,          EDCC_LOAD_CONFIG_FAIL},
    {Status::kLoadTrainingsetError,     EDCC_LOAD_TAINING_SET_FAIL},
    {Status::kLoadFeaturesError,        EDCC_LOAD_FEATURES_FAIL},
    {Status::kSaveFeaturesError,        EDCC_SAVE_FEATURES_FAIL},
    {Status::kLoadPalmprintError,       EDCC_LOAD_PALMPRINT_IMAGE_FAIL},
    {Status::kCodingBufferLenNotEnough, EDCC_CODING_BUFF_LEN_NOT_ENOUGH},
    {Status::kCodingInvalid,            EDCC_CODING_INVALID},
    {Status::kCodingsConfigDiff,        EDCC_CODINGS_DIFF_CONFIG}
};

Status::Status(Code code)
{
    char *result = new char[1];
    result[0] = static_cast<char>(code);
    state_ = result;
}

int Status::ToExtCode()
{
    return transform_table[state_[0]].ext_code;
}

const char* Status::CopyState(const char *state)
{
    assert(state != NULL);
    char *result = new char[1];
    memcpy(result, state, 1);
    return result;
}

} // namespace edcc