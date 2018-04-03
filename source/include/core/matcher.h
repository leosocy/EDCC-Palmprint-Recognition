// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __MATCH_H__
#define __MATCH_H__

namespace edcc {

class Status;
typedef struct tag_EDCC_CODING_T EDCC_CODING_T;

class Matcher
{
public:
    static Status MatchingProcess(const EDCC_CODING_T *lhs_coding,
                                  const EDCC_CODING_T *rhs_coding,
                                  double *score);
private:
    static Status ExecuteMatchingWhenCompressionCodingMode(const EDCC_CODING_T *lhs_coding,
                                                          const EDCC_CODING_T *rhs_coding,
                                                          double *score);
    static Status ExecuteMatchingWhenFastCodingMode(const EDCC_CODING_T *lhs_coding,
                                                   const EDCC_CODING_T *rhs_coding,
                                                   double *score);
};

} // namespace edcc


#endif 
