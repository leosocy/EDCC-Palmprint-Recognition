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
                       _IN const PalmprintCode &instance2) const
{
    double score = 0.0;
    const EDCC_CODING_T *coding_t_1 = instance1.ptCoding;
    const EDCC_CODING_T *coding_t_2 = instance2.ptCoding;
    if(coding_t_1 == NULL
       || coding_t_2 == NULL) {
        EDCC_Log("Two palmprint instance info error.\n");
        return score;
    }

    size_t codingCLen = (size_t)ceil(coding_t_1->cfg.imageSizeW*coding_t_1->cfg.imageSizeH / 2.0);
    const u_char *cxStartPos = coding_t_1->pCodingBuff;
    const u_char *cyStartPos = coding_t_2->pCodingBuff;
    const u_char *csxStartPos = coding_t_1->pCodingBuff+codingCLen;
    const u_char *csyStartPos = coding_t_2->pCodingBuff+codingCLen;

    size_t cpos = 0;
    size_t cspos = 0;
    u_char cMask = 0xf0;
    u_char csMask = 0x80;
    while(cpos < codingCLen) {
        if((cxStartPos[cpos] & cMask) == (cyStartPos[cpos] & cMask)) {
            score += 1.0;
            if((csxStartPos[cspos] & csMask) == (csyStartPos[cspos] & csMask)) {
                score += 1.0;
            }
        }
        cpos = cMask & 0x0f ? ++cpos : cpos;
        cspos = csMask & 0x01 ? ++cspos : cspos;
        cMask = cMask & 0x0f ? 0xf0 : 0x0f;
        csMask = csMask & 0x01 ? 0x80 : (csMask >> 1);
    }

    return score / (2 * 2 * codingCLen);
}
