// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __MATCH_H__
#define __MATCH_H__

#include "util/pub.h"

namespace edcc {

class PalmprintCode;
typedef struct tag_EDCC_CODING_T EDCC_CODING_T;

class Match
{
public:
    static Status MatchingProcess(const EDCC_CODING_T *lhs_coding,
                                  const EDCC_CODING_T *rhs_coding,
                                  double *score);
private:
    static Status ExcuteMatchingWhenCompressionCodingMode(const EDCC_CODING_T *lhs_coding,
                                                          const EDCC_CODING_T *rhs_coding,
                                                          double *score);
    static Status ExcuteMatchingWhenFastCodingMode(const EDCC_CODING_T *lhs_coding,
                                                   const EDCC_CODING_T *rhs_coding,
                                                   double *score);
};

} // namespace edcc

#endif 
