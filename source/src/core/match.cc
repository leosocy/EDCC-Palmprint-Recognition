// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/match.h"

#include "core/palmprint_code.h"
#include "core/edccoding.h"

namespace edcc
{

double Match::MatchPoint2Point(const PalmprintCode &lhs,
                               const PalmprintCode &rhs) const
{
    int score = 0;
    const EDCC_CODING_T *l_coding = lhs.coding()->coding_buffer();
    const EDCC_CODING_T *r_coding = rhs.coding()->coding_buffer();
    if (l_coding == NULL
        || r_coding == NULL)
    {
        EDCC_Log("Two palmprint instance info error.");
        return score;
    }

    size_t coding_c_len = (size_t)ceil(l_coding->cfg.imageSizeW*l_coding->cfg.imageSizeH / 2.0);
    const u_char *c_x_start_pos = l_coding->data;
    const u_char *c_y_start_pos = r_coding->data;
    const u_char *c_end = l_coding->data + coding_c_len;
    const u_char *cs_x_start_pos = l_coding->data + coding_c_len;
    const u_char *cs_y_start_pos = r_coding->data + coding_c_len;

    u_char c_mask = 0xf0;
    u_char cs_mask = 0x80;
    while (c_x_start_pos < c_end)
    {
        if ((*c_x_start_pos & c_mask) == (*c_y_start_pos & c_mask))
        {
            score += 1;
            if ((*cs_x_start_pos & cs_mask) == (*cs_y_start_pos & cs_mask))
            {
                score += 1;
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
        /*c_x_start_pos = c_mask & 0x0f ? ++c_x_start_pos : c_x_start_pos;
        cs_x_start_pos = cs_mask & 0x01 ? ++cs_x_start_pos : cs_x_start_pos;
        c_y_start_pos = c_mask & 0x0f ? ++c_y_start_pos : c_y_start_pos;
        cs_y_start_pos = cs_mask & 0x01 ? ++cs_y_start_pos : cs_y_start_pos;
        c_mask = c_mask & 0x0f ? 0xf0 : 0x0f;
        cs_mask = cs_mask & 0x01 ? 0x80 : (cs_mask >> 1);*/
    }

    return score / (2.0 * 2 * coding_c_len);
}

} // namespace edcc
