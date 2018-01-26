// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/check.h"

#include <iostream>
#include <map>

#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"
#include "util/pub.h"

namespace edcc
{

namespace
{

const u_short kMinImageWidth = 29;
const u_short kMinImageHeight = 29;
const u_char kMaxLaplaceKernelSize = 31;
const u_char kMinGaborDirections = 4;
const u_char kMaxGabotDirections = 16;

} // namespace

bool Check::CheckConfig(const EDCC_CFG_T &config) const
{
    if (config.imageSizeW < kMinImageWidth
        || config.imageSizeH < kMinImageHeight)
    {
        EDCC_Log("ImageSize(%d, %d) can't smaller than(%d, %d) %d", config.imageSizeW, config.imageSizeH, kMinImageWidth, kMinImageHeight);
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
        || config.laplaceSize > kMaxLaplaceKernelSize)
    {
        EDCC_Log("Laplace Kernel Size must be smaller than imageSize.And must be odd and samller than 31!");
        return false;
    }
    if (config.directions > kMaxGabotDirections
        || config.directions < kMinGaborDirections)
    {
        EDCC_Log("Gabor Directions must in range [%d, %d]!",
                 kMinGaborDirections, kMaxGabotDirections);
        return false;
    }

    return true;
}

bool Check::CheckTrainingSet(const vector<PalmprintCode> &data) const
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
                             const EDCC_CFG_T &config) const
{
    for (vector<PalmprintCode>::const_iterator data_iter = data.begin();
         data_iter != data.end(); ++data_iter)
    {
        const EDCC_CODING_T *coding_buffer = data_iter->coding()->coding_buffer();
        if (coding_buffer == NULL
            || !CheckTwoConfigEQAndValid(config, coding_buffer->cfg)
            || !CheckCoding(*data_iter->coding()))
        {
            EDCC_Log("EDCCoding format error!");
            return false;
        }
    }

    return true;
}

bool Check::CheckCoding(const EDCCoding &coding) const
{
    CHECK_POINTER_NULL_RETURN(coding.coding_buffer(), false);
    int actual_magic_key = 0;
    const void *src_addr = static_cast<const void*>(coding.coding_buffer()->data \
                                                    + coding.coding_buffer()->len - EDCCoding::kMagicKeyLen);
    void *dst_addr = static_cast<void*>(&actual_magic_key);
    memcpy(dst_addr, src_addr, EDCCoding::kMagicKeyLen);
    CHECK_NE_RETURN(actual_magic_key, coding.magic_key(), false);

    size_t coding_c_len = static_cast<size_t>(ceil(coding.coding_buffer()->cfg.imageSizeW \
                                                   * coding.coding_buffer()->cfg.imageSizeH / 2.0));
    u_char gabor_directions = coding.coding_buffer()->cfg.directions;
    for (size_t i = 0; i < coding_c_len; ++i)
    {
        u_char cTmp = *(coding.coding_buffer()->data + i);
        if ((cTmp & 0x0f) >= gabor_directions
            || ((cTmp & 0xf0) >> 4) >= gabor_directions)
        {
            return false;
        }
    }
    return true;
}

bool Check::CheckTwoPalmprintCodeConfigEqualAndValid(const PalmprintCode &first_palmprintcode,
                                                     const PalmprintCode &second_palmprintcode) const
{
    CHECK_POINTER_NULL_RETURN(first_palmprintcode.coding()->coding_buffer(), false);
    CHECK_POINTER_NULL_RETURN(second_palmprintcode.coding()->coding_buffer(), false);
    return CheckTwoConfigEQAndValid(first_palmprintcode.coding()->coding_buffer()->cfg,
                                    second_palmprintcode.coding()->coding_buffer()->cfg);
}

bool Check::CheckTwoConfigEQAndValid(const EDCC_CFG_T &first_config,
                                     const EDCC_CFG_T &second_config) const
{
    return !memcmp(&first_config, &second_config, sizeof(EDCC_CFG_T))
        && CheckConfig(first_config)
        && CheckConfig(second_config);
}

} // namespace edcc
