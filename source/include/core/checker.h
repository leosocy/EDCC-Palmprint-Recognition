// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __CHECK_H__
#define __CHECK_H__

#include <vector>
#include "util/pub.h"

namespace edcc
{

class PalmprintCode;
class EDCCoding;
typedef struct tag_EDCC_CFG_T EDCC_CFG_T;
typedef struct tag_EDCC_CODING_T EDCC_CODING_T;

class Checker
{
public:
    static bool CheckConfig(const EDCC_CFG_T &config);
    static bool CheckTrainingSet(const std::vector<PalmprintCode> &data);
    static bool CheckFeatureData(const std::vector<PalmprintCode> &data,
                                 const EDCC_CFG_T &config);
    static bool CheckCoding(const EDCCoding &coding);
    static bool CheckCodingBuffer(const EDCC_CODING_T *coding_buffer);
private:
    static bool CheckTwoConfigEqual(const EDCC_CFG_T &first_config,
                                    const EDCC_CFG_T &second_config);
};

} // namespace edcc

#endif
