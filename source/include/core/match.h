// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __MATCH_H__
#define __MATCH_H__

namespace edcc {

class PalmprintCode;
typedef struct tag_EDCC_CODING_T EDCC_CODING_T;

class Match
{
public:
    static double MatchPoint2Point(const PalmprintCode &lhs,
                                   const PalmprintCode &rhs);
    static double FastModeMatching(const unsigned char *lhs,
                                   const unsigned char *rhs);
private:
    static double ExcuteMatching(const EDCC_CODING_T *lhs,
                                 const EDCC_CODING_T *rhs);
};

} // namespace edcc

#endif 
