// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/checker.h"

#include <iostream>
#include <map>

#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprintcode.h"
#include "core/config.h"

namespace edcc
{
using std::vector;
using std::set;

bool IsValueInArray(const u_char *elem_array, int array_size, u_char value)
{
    for (int i = 0; i < array_size; ++i)
    {
        if (*(elem_array + i) == value)
        {
            return true;
        }
    }
    return false;
}

bool Checker::CheckConfig(const EDCC_CFG_T &config)
{
    if (config.imageSizeW < limit::kMinImageWidth
        || config.imageSizeH < limit::kMinImageHeight)
    {
        EDCC_Log("ImageSize(%d, %d) can't smaller than(%d, %d).",
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
    if (!IsValueInArray(limit::kSupportedCodingModes,
                       sizeof(limit::kSupportedCodingModes)/sizeof(limit::kSupportedCodingModes[0]),
                       config.codingMode))
    {
        EDCC_Log("Coding Mode [%d] not supported!",
                 config.codingMode);
        return false;
    }
    if (!IsValueInArray(limit::kSupportedMatchingModes,
                        sizeof(limit::kSupportedMatchingModes) / sizeof(limit::kSupportedMatchingModes[0]),
                        config.matchingMode))
    {
        EDCC_Log("Matching Mode [%d] not supported!",
                 config.matchingMode);
        return false;
    }

    return true;
}

bool Checker::CheckTrainingSet(const vector<PalmprintCode> &data)
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

bool Checker::CheckFeatureData(const vector<PalmprintCode> &data,
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

bool Checker::CheckCoding(const EDCCoding &coding)
{
    const EDCC_CODING_T *coding_buffer = coding.buffer();
    return CheckCodingBuffer(coding_buffer);
}

bool Checker::CheckCodingBuffer(const EDCC_CODING_T *coding_buffer)
{
    CHECK_POINTER_NULL_RETURN(coding_buffer, false);
    int actual_magic_key = 0;
    const void *src_addr = static_cast<const void*>(coding_buffer->data \
                                                    + coding_buffer->len - EDCCoding::kMagicKeyLen);
    memcpy(&actual_magic_key, src_addr, EDCCoding::kMagicKeyLen);
    CHECK_NE_RETURN(actual_magic_key, EDCCoding::kMagicKey, false);
    u_char gabor_directions = coding_buffer->cfg.directions;
    if (coding_buffer->cfg.codingMode == COMPRESSION_CODING_MODE)
    {
        size_t coding_c_len = static_cast<size_t>(ceil(coding_buffer->cfg.imageSizeW \
                                                       * coding_buffer->cfg.imageSizeH / 2.0));
        const u_char *coding_c_cur_addr = coding_buffer->data;
        const u_char *coding_c_end_addr = coding_buffer->data + coding_c_len;
        for (; coding_c_cur_addr < coding_c_end_addr; ++coding_c_cur_addr)
        {
            u_char c_tmp = *coding_c_cur_addr;
            if ((c_tmp & 0x0f) >= gabor_directions
                || ((c_tmp & 0xf0) >> 4) >= gabor_directions)
            {
                EDCC_Log("Coding Buffer Invalid!");
                return false;
            }
        }
    }
    else if (coding_buffer->cfg.codingMode == FAST_CODING_MODE)
    {
        size_t coding_len = coding_buffer->cfg.imageSizeW * coding_buffer->cfg.imageSizeH;
        const u_char *coding_cur_addr = coding_buffer->data;
        const u_char *coding_end_addr = coding_buffer->data + coding_len;
        for (; coding_cur_addr < coding_end_addr; ++coding_cur_addr)
        {
            u_char c_tmp = *coding_cur_addr;
            if (((c_tmp & 0xf0) >> 4) >= gabor_directions
                || (c_tmp & 0x0e) != 0x00)
            {
                EDCC_Log("Coding Buffer Invalid!");
                return false;
            }
        }
    }
    else
    {
        EDCC_Log("Coding Mode [%d] not supported!", coding_buffer->cfg.codingMode);
        return false;
    }
    
    return true;
}

bool Checker::CheckTwoConfigEqual(const EDCC_CFG_T &first_config,
                                const EDCC_CFG_T &second_config)
{
    return !memcmp(&first_config, &second_config, sizeof(EDCC_CFG_T));
}

} // namespace edcc
