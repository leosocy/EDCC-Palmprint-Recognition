// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/match.h"
#include "core/palmprint_code.h"
#include "core/edccoding.h"
#include "core/check.h"
#include "EDCC.h"

namespace edcc
{

Status Match::MatchingProcess(const EDCC_CODING_T *lhs_coding,
                              const EDCC_CODING_T *rhs_coding,
                              double *score)
{
    assert(lhs_coding && rhs_coding);
    if (memcmp(&lhs_coding->cfg, &rhs_coding->cfg, sizeof(EDCC_CFG_T)))
    {
        *score = .0;
        EDCC_Log("Two palmprint instance coding buffer config difference.");
        return EDCC_CODINGS_DIFF_CONFIG;
    }

    switch (lhs_coding->cfg.codingMode)
    {
        case COMPRESSION_CODING_MODE:
        {
            return ExcuteMatchingWhenCompressionCodingMode(lhs_coding, rhs_coding, score);
        }
        case FAST_CODING_MODE:
        {
            return ExcuteMatchingWhenFastCodingMode(lhs_coding, rhs_coding, score);
        }
        default:
        {
            *score = .0;
            EDCC_Log("Coding Mode Error.");
            return EDCC_CODING_INVALID;
        }
    }
}

Status Match::ExcuteMatchingWhenCompressionCodingMode(const EDCC_CODING_T *lhs_coding,
                                                      const EDCC_CODING_T *rhs_coding,
                                                      double *score)
{
    if (lhs_coding->cfg.matchingMode == RELIABLE_MATCHING_MODE)
    {
        if (!Check::CheckCodingBuffer(lhs_coding)
            || !Check::CheckCodingBuffer(rhs_coding))
    {
            *score = .0;
            EDCC_Log("Coding Invalid!");
            return EDCC_CODING_INVALID;
        }
    }

    int match_score = 0;
    size_t image_size = lhs_coding->cfg.imageSizeW * lhs_coding->cfg.imageSizeH;
    size_t coding_c_len = (size_t)ceil(image_size / 2.0);
    const u_char *c_x_start_pos = lhs_coding->data;
    const u_char *c_y_start_pos = rhs_coding->data;
    const u_char *c_end = lhs_coding->data + coding_c_len;
    const u_char *cs_x_start_pos = lhs_coding->data + coding_c_len;
    const u_char *cs_y_start_pos = rhs_coding->data + coding_c_len;

    u_char c_mask = 0xf0;
    u_char cs_mask = 0x80;
    while (c_x_start_pos < c_end)
    {
        if ((*c_x_start_pos & c_mask) == (*c_y_start_pos & c_mask))
        {
            match_score += 1;
            if ((*cs_x_start_pos & cs_mask) == (*cs_y_start_pos & cs_mask))
            {
                match_score += 1;
            }
        }
        if (c_mask & 0x0f)
        {
            ++c_x_start_pos;
            ++c_y_start_pos;
            c_mask = 0xf0;
        }
        else
        {
            c_mask = 0x0f;
        }
        if (cs_mask & 0x01)
        {
            ++cs_x_start_pos;
            ++cs_y_start_pos;
            cs_mask = 0x80;
        }
        else
        {
            cs_mask >>= 1;
        }
    }

    if (image_size % 2 == 0)
    {
        *score = match_score / (2.0 * image_size);
    }
    else
    {
        *score = (match_score - 2) / (2.0 * image_size);
    }
    return EDCC_SUCCESS;
}

Status Match::ExcuteMatchingWhenFastCodingMode(const EDCC_CODING_T *lhs_coding,
                                               const EDCC_CODING_T *rhs_coding,
                                               double *score)
{
    if (lhs_coding->cfg.matchingMode == RELIABLE_MATCHING_MODE)
    {
        if (!Check::CheckCodingBuffer(lhs_coding)
            || !Check::CheckCodingBuffer(rhs_coding))
        {
            *score = .0;
            EDCC_Log("Coding Invalid!");
            return EDCC_CODING_INVALID;
        }
    }

    int match_score = 0;
    size_t image_size = lhs_coding->cfg.imageSizeW * lhs_coding->cfg.imageSizeH;
    const u_char *lhs_coding_buffer_cur = lhs_coding->data;
    const u_char *lhs_coding_buffer_end = lhs_coding_buffer_cur + image_size;
    const u_char *rhs_coding_buffer_cur = rhs_coding->data;
    while (lhs_coding_buffer_cur < lhs_coding_buffer_end)
    {
        u_char cmp_value = *lhs_coding_buffer_cur ^ *rhs_coding_buffer_cur;
        if (cmp_value == 0x00)
        {
            match_score += 2;
        }
        else if (cmp_value < 0x10)
        {
            match_score += 1;
        }
        else
        {
            match_score += 0;
        }
        ++lhs_coding_buffer_cur;
        ++rhs_coding_buffer_cur;
    }

    *score = match_score / (2.0 * image_size);
    return EDCC_SUCCESS;
}

} // namespace edcc
