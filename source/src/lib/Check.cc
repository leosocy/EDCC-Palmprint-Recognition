/*************************************************************************
	> File Name: IO.cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/18 22:00:10
 ************************************************************************/

#include <Check.h>
#include <Core.h>
using namespace EDCC;

bool Check::checkConfigValid(_IN const EDCC_CFG_T &config) const
{
    if(config.imageSizeW < CONFIG_IMAGE_SIZE_W_MIN 
       || config.imageSizeH < CONFIG_IMAGE_SIZE_H_MIN) {
        EDCC_Log("ImageSize(%d, %d) can't smaller than(%d, %d) %d\n", config.imageSizeW, config.imageSizeH, CONFIG_IMAGE_SIZE_W_MIN, CONFIG_IMAGE_SIZE_H_MIN);
        return false;
    }
    if(config.gaborSize > config.imageSizeW
       || config.gaborSize > config.imageSizeH
       || config.gaborSize % 2 == 0) {
        EDCC_Log("Gabor Kernel Size must be smaller than imageSize.And must be odd!\n");
        return false;
    }
    if(config.laplaceSize > config.imageSizeW
       || config.laplaceSize > config.imageSizeH
       || config.laplaceSize % 2 == 0
       || config.laplaceSize > CONFIG_VALID_LAPLACE_KERNEL_SIZE_MAX) {
        EDCC_Log("Laplace Kernel Size must be smaller than imageSize.And must be odd and samller than 31!\n");
        return false;
    }
    if(config.directions > CONFIG_VALID_GABOR_DIRECTIONS_MAX
       || config.directions < CONFIG_VALID_GABOR_DIRECTIONS_MIN) {
        EDCC_Log("Gabor Directions must in range [%d, %d]!\n", 
                 CONFIG_VALID_GABOR_DIRECTIONS_MIN, CONFIG_VALID_GABOR_DIRECTIONS_MAX);
        return false;
    }
        
    return true;
}

bool Check::checkPalmprintGroupValid(_IN const vector<PalmprintCode> &data) const
{
    
    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        for(vector<PalmprintCode>::const_iterator dataInnerIte = dataIte + 1;
            dataInnerIte != data.end(); ++dataInnerIte) {
            if((*dataIte) == (*dataInnerIte)) {
                EDCC_Log("Image Path: %s Conflict!\n", dataIte->getImagePath().c_str());
                return false;
            }
        }
    }
    
    return true;
}

bool Check::checkPalmprintFeatureData(_IN const vector<PalmprintCode> &data,
                                      _IN const EDCC_CFG_T &config) const
{
    CHECK_FALSE_RETURN(checkConfigValid(config), false);

    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        CHECK_FALSE_RETURN(checkTwoConfigEQAndValid(config, dataIte->m_ptCoding->cfg), false);
        if(dataIte->m_ptCoding == NULL
           || !checkCoding(*dataIte)) {
            EDCC_Log("EDCCoding format error!\n");
            return false;
        }
    }
    
    return true;
}

bool Check::checkCoding(_IN const EDCCoding &coding) const
{
    CHECK_POINTER_NULL_RETURN(coding.m_ptCoding, false);
    int actMagicKey = 0;
    memcpy(&actMagicKey, coding.m_ptCoding->pCodingBuff + coding.m_ptCoding->codingBuffLen - MAGIC_KEY_LEN, MAGIC_KEY_LEN);
    CHECK_NE_RETURN(actMagicKey, coding.magicKey, false);

    size_t codingCLen = (size_t)ceil(coding.m_ptCoding->cfg.imageSizeW*coding.m_ptCoding->cfg.imageSizeH/2.0);
    u_char gaborDirections = coding.m_ptCoding->cfg.directions;
    for(size_t i = 0; i < codingCLen; ++i) {
        u_char cTmp = *(coding.m_ptCoding->pCodingBuff + i);
        if((cTmp & 0x0f) >= gaborDirections
           || ((cTmp & 0xf0) >> 4) >= gaborDirections) {
            return false;
        }
    }

    return true;
}

bool Check::checkTwoPalmprintCodeConfigEQAndValid(_IN const PalmprintCode &firstPalmprintCode,
                                                  _IN const PalmprintCode &secondPalmprintCode) const
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintCode.m_ptCoding, false);
    CHECK_POINTER_NULL_RETURN(secondPalmprintCode.m_ptCoding, false);
    return checkTwoConfigEQAndValid(firstPalmprintCode.m_ptCoding->cfg, secondPalmprintCode.m_ptCoding->cfg);
}

bool Check::checkTwoConfigEQAndValid(_IN const EDCC_CFG_T &firstConfig,
                                     _IN const EDCC_CFG_T &secondConfig) const
{
    return !memcmp(&firstConfig, &secondConfig, sizeof(EDCC_CFG_T))
        && checkConfigValid(firstConfig)
        && checkConfigValid(secondConfig);
}



