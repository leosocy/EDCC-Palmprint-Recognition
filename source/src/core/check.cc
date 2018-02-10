// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/check.h"

#include <iostream>
#include <map>

#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"
#include "core/config.h"
#include "util/pub.h"

namespace edcc
{

using std::vector;
using std::set;

namespace limit
{

static const u_short kMinImageWidth = 29;
static const u_short kMinImageHeight = 29;
static const u_char kMaxLaplaceKernelSize = 31;
static const u_char kMinGaborDirections = 4;
static const u_char kMaxGabotDirections = 16;
static const set<u_char> kSupportedCodingModes = {1, 2};
static const set<u_char> kSupportedMatchingModes = {1, 2};

} // namespace limit

bool Check::CheckConfig(const EDCC_CFG_T &config)
{
    if (config.imageSizeW < limit::kMinImageWidth
        || config.imageSizeH < limit::kMinImageHeight)
    {
        EDCC_Log("ImageSize(%d, %d) can't smaller than(%d, %d) %d",
                 config.imageSizeW, config.imageSizeH,
                 limit::kMinImageWidth, limit::kMinImageHeight);
        return false;
    }
    if (config.gaborSize > config.imageSizeW
        || config.gaborSize > config.imageSizeH
        || config.gaborSize % 2 == 0)
    {
        EDCC_Log("Gabor Kernel Size must be smaller than imageSize.And must be odd!");
        return false;
    }
    if (config.laplaceSize > config.imageSizeW
        || config.laplaceSize > config.imageSizeH
        || config.laplaceSize % 2 == 0
        || config.laplaceSize > limit::kMaxLaplaceKernelSize)
    {
        EDCC_Log("Laplace Kernel Size must be smaller than imageSize.And must be odd and samller than 31!");
        return false;
    }
    if (config.directions > limit::kMaxGabotDirections
        || config.directions < limit::kMinGaborDirections)
    {
        EDCC_Log("Gabor Directions must in range [%d, %d]!",
                 limit::kMinGaborDirections, limit::kMaxGabotDirections);
        return false;
    }
    if (limit::kSupportedCodingModes.find(config.codingMode) == limit::kSupportedCodingModes.end())
    {
        EDCC_Log("Coding Mode [%d] not supported!",
                 config.codingMode);
        return false;
    }
    if (limit::kSupportedMatchingModes.find(config.matchingMode) == limit::kSupportedMatchingModes.end())
    {
        EDCC_Log("Matching Mode [%d] not supported!",
                 config.matchingMode);
        return false;
    }

    return true;
}

bool Check::CheckTrainingSet(const vector<PalmprintCode> &data)
{
    for (vector<PalmprintCode>::const_iterator data_iter = data.begin();
         data_iter != data.end(); ++data_iter)
    {
        for (vector<PalmprintCode>::const_iterator data_inner_iter = data_iter + 1;
             data_inner_iter != data.end(); ++data_inner_iter)
        {
            if ((*data_iter->palmprint()) == (*data_inner_iter->palmprint()))
            {
                EDCC_Log("Image Path: %s Conflict!", data_iter->palmprint()->image_path().c_str());
                return false;
            }
        }
    }

    return true;
}

bool Check::CheckFeatureData(const vector<PalmprintCode> &data,
                             const EDCC_CFG_T &config)
{
    for (vector<PalmprintCode>::const_iterator data_iter = data.begin();
         data_iter != data.end(); ++data_iter)
    {
        const EDCC_CODING_T *coding_buffer = data_iter->coding()->buffer();
        if (coding_buffer == NULL
            || !CheckConfig(config)
            || !CheckTwoConfigEqual(config, coding_buffer->cfg)
            || !CheckCoding(*data_iter->coding()))
        {
            EDCC_Log("EDCCoding format error!");
            return false;
        }
    }

    return true;
}

bool Check::CheckCoding(const EDCCoding &coding)
{
    const EDCC_CODING_T *coding_buffer = coding.buffer();
    CHECK_POINTER_NULL_RETURN(coding_buffer, false);
    int actual_magic_key = 0;
    const void *src_addr = static_cast<const void*>(coding_buffer->data \
                                                    + coding_buffer->len - EDCCoding::kMagicKeyLen);
    memcpy(&actual_magic_key, src_addr, EDCCoding::kMagicKeyLen);
    CHECK_NE_RETURN(actual_magic_key, coding.magic_key(), false);
    size_t coding_c_len = static_cast<size_t>(ceil(coding_buffer->cfg.imageSizeW \
                                                   * coding_buffer->cfg.imageSizeH / 2.0));
    u_char gabor_directions = coding_buffer->cfg.directions;
    for (size_t i = 0; i < coding_c_len; ++i)
    {
        u_char c_tmp = *(coding_buffer->data + i);
        if ((c_tmp & 0x0f) >= gabor_directions
            || ((c_tmp & 0xf0) >> 4) >= gabor_directions)
        {
            return false;
        }
    }

    return true;
}

bool Check::CheckTwoPalmprintCodeConfigEqual(const PalmprintCode &first_palmprintcode,
                                             const PalmprintCode &second_palmprintcode)
{
    CHECK_POINTER_NULL_RETURN(first_palmprintcode.coding()->buffer(), false);
    CHECK_POINTER_NULL_RETURN(second_palmprintcode.coding()->buffer(), false);

    return CheckTwoConfigEqual(first_palmprintcode.coding()->buffer()->cfg,
                               second_palmprintcode.coding()->buffer()->cfg);
}

bool Check::CheckTwoConfigEqual(const EDCC_CFG_T &first_config,
                                const EDCC_CFG_T &second_config)
{
    return !memcmp(&first_config, &second_config, sizeof(EDCC_CFG_T));
}

} // namespace edcc
