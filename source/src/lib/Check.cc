/*************************************************************************
	> File Name: IO.cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/18 22:00:10
 ************************************************************************/

#include <Check.h>
#include <Core.h>
using namespace EDCC;

bool Check::checkConfigValid(_IN const EDCC_CFG_T &config)
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

bool Check::checkPalmprintGroupValid(_IN const vector<PalmprintCode> &data)
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
                                      _IN const EDCC_CFG_T &config)
{
    CHECK_FALSE_RETURN(checkConfigValid(config), false);

    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        CHECK_FALSE_RETURN(checkTwoConfigEQAndValid(config, dataIte->cfg), false);
        if(dataIte->ptCoding == NULL
           || !checkCodingMagicKey(*dataIte)
           || !checkCoding(*dataIte)) {
            EDCC_Log("EDCCCoding format error!\n");
            return false;
        }
    }
    
    return true;
}

bool Check::checkCodingMagicKey(_IN const EDCCoding &coding)
{
    int actMagicKey = 0;
    memcpy(&actMagicKey, coding.ptCoding->pCodingBuff + coding.ptCoding->codingBuffLen - MAGIC_KEY_LEN, MAGIC_KEY_LEN);
    return actMagicKey == coding.magicKey;
}

bool Check::checkCoding(_IN const EDCCoding &coding)
{
    size_t codingCLen = (size_t)ceil(coding.ptCoding->cfg.imageSizeW*coding.ptCoding->cfg.imageSizeH/2.0);
    u_char gaborDirections = coding.ptCoding->cfg.directions;
    for(size_t i = 0; i < codingCLen; ++i) {
        u_char cTmp = *(coding.ptCoding->pCodingBuff + i);
        if((cTmp & 0x0f) >= gaborDirections
           || ((cTmp & 0xf0) >> 4) >= gaborDirections) {
            return false;
        }
    }
    return true;
}

bool Check::checkTwoPalmprintCodeConfigEQAndValid(_IN const PalmprintCode &firstPalmprintCode,
                                                  _IN const PalmprintCode &secondPalmprintCode)
{
    return checkTwoConfigEQAndValid(firstPalmprintCode.cfg, secondPalmprintCode.cfg);
}

bool Check::checkTwoConfigEQAndValid(_IN const EDCC_CFG_T &firstConfig,
                                     _IN const EDCC_CFG_T &secondConfig)
{
    return firstConfig.imageSizeW == secondConfig.imageSizeW
        && firstConfig.imageSizeH == secondConfig.imageSizeH
        && firstConfig.gaborSize == secondConfig.gaborSize
        && firstConfig.laplaceSize == secondConfig.laplaceSize
        && firstConfig.directions == secondConfig.directions
        && checkConfigValid(firstConfig)
        && checkConfigValid(secondConfig);
}



