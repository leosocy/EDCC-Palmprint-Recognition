// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __MATCH_H__
#define __MATCH_H__

namespace edcc {

class PalmprintCode;

class Match 
{
public:
    double MatchPoint2Point(const PalmprintCode &lhs,
                            const PalmprintCode &rhs) const;
};

} // namespace edcc

#endif 
