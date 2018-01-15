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
    int score = 0;
    const EDCC_CODING_T *coding_t_1 = instance1.m_ptCoding;
    const EDCC_CODING_T *coding_t_2 = instance2.m_ptCoding;
    if(coding_t_1 == NULL
       || coding_t_2 == NULL) {
        EDCC_Log("Two palmprint instance info error.");
        return score;
    }

    size_t codingCLen = (size_t)ceil(coding_t_1->cfg.imageSizeW*coding_t_1->cfg.imageSizeH/2.0);
    const u_char *cxStartPos = coding_t_1->pCodingBuff;
    const u_char *cyStartPos = coding_t_2->pCodingBuff;
    const u_char *cEnd = coding_t_1->pCodingBuff + codingCLen;
    const u_char *csxStartPos = coding_t_1->pCodingBuff+codingCLen;
    const u_char *csyStartPos = coding_t_2->pCodingBuff+codingCLen;

    size_t cpos = 0, cspos = 0;
    u_char cMask = 0xf0, csMask = 0x80;
    while(cxStartPos < cEnd) {
        if((*cxStartPos & cMask) == (*cyStartPos & cMask)) {
            score += 1;
            if((*csxStartPos & csMask) == (*csyStartPos & csMask)) {
                score += 1;
            }
        }
        if(cMask & 0x0f) {
            ++cxStartPos;
            ++cyStartPos;
            cMask = 0xf0;
        } else {
            cMask = 0x0f;
        }
        if(csMask & 0x01) {
            ++csxStartPos;
            ++csyStartPos;
            csMask = 0x80;
        } else {
            csMask >>= 1;
        }
        /*cxStartPos = cMask & 0x0f ? ++cxStartPos : cxStartPos;
        csxStartPos = csMask & 0x01 ? ++csxStartPos : csxStartPos;
        cyStartPos = cMask & 0x0f ? ++cyStartPos : cyStartPos;
        csyStartPos = csMask & 0x01 ? ++csyStartPos : csyStartPos;
        cMask = cMask & 0x0f ? 0xf0 : 0x0f;
        csMask = csMask & 0x01 ? 0x80 : (csMask >> 1);*/
    }

    return score / (2.0 * 2 * codingCLen);
}
