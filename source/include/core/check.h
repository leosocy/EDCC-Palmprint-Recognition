// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __CHECK_H__
#define __CHECK_H__

#include <vector>

namespace edcc
{

class PalmprintCode;
class EDCCoding;
typedef struct tag_EDCC_CFG_T EDCC_CFG_T;

class Check
{
public:
    bool CheckConfig(const EDCC_CFG_T &config) const;
    bool CheckTrainingSet(const std::vector<PalmprintCode> &data) const;
    bool CheckFeatureData(const std::vector<PalmprintCode> &data,
                          const EDCC_CFG_T &config) const;
    bool CheckCoding(const EDCCoding &coding) const;
    bool CheckTwoPalmprintCodeConfigEqual(const PalmprintCode &first_palmprintcode,
                                                  const PalmprintCode &second_palmprintcode) const;
private:
    bool CheckTwoConfigEqual(const EDCC_CFG_T &first_config,
                                  const EDCC_CFG_T &second_config) const;
};

} // namespace edcc

#endif
