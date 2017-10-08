/*************************************************************************
> File Name: Match.cc
> Author: Leosocy
> Mail: 513887568@qq.com
> Created Time: 2017/10/1 23:50:15
************************************************************************/
#include <Match.h>
#include <Core.h>
using namespace EDCC;

double Match::matchP2P(_IN const PalmprintCode &instance1,
                       _IN const PalmprintCode &instance2)
{
    double score = 0.0;
    string Cx = instance1.zipCodingC;
    string Cy = instance2.zipCodingC;
    string Csx = instance1.zipCodingCs;
    string Csy = instance2.zipCodingCs;
    if(Cx.length() != Cy.length()
       || Csx.length() != Csy.length()
       || Cx.empty()
       || Cy.empty()
       || Csx.empty()
       || Csy.empty()) {
        cerr << "Two palmprint instance info error." << endl;
        return score;
    }

    size_t codingLen = Cx.length();
    for(size_t i = 0, csIndex = 0; i < codingLen; ++i, csIndex = i / 4) {
        if(Cx.at(i) == Cy.at(i)) {
            score += 1.0;
            unsigned char mask = 1 << (3 - (i % 4));
            if((Csx.at(csIndex) & mask) == (Csy.at(csIndex) & mask)) {
                score += 1.0;
            }
        }
    }

    return score / (2 * codingLen);
}
