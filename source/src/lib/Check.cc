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
    /*if(configMap.find(IMAGE_SIZE_W) == configMap.end()
       || configMap.find(IMAGE_SIZE_H) == configMap.end()
       || configMap.find(GABOR_KERNEL_SIZE) == configMap.end()
       || configMap.find(GABOR_DIRECTIONS) == configMap.end()
       || configMap.find(LAPLACE_KERNEL_SIZE) == configMap.end()) {
        EDCC_Log("Load config fail, config params miss!\n");
        return false;
    }*/

    /*imageSizeW = configMap.at(IMAGE_SIZE_W);
    imageSizeH = configMap.at(IMAGE_SIZE_H);
    gaborKernelSize = configMap.at(GABOR_KERNEL_SIZE);
    gaborDirections = configMap.at(GABOR_DIRECTIONS);
    laplaceKernelSize = configMap.at(LAPLACE_KERNEL_SIZE);*/

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
    if(!checkConfigValid(config)) {
        return false;
    }

    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        if((dataIte->zipCodingC).length() != config.imageSizeW * config.imageSizeH
            || !checkCodingC(dataIte->zipCodingC, config.directions)) {
            EDCC_Log("EDCCoding C format error!\n");
            return false;
        }
        if((dataIte->zipCodingCs).length() != (config.imageSizeW * config.imageSizeH / 4 + 1)) {
            EDCC_Log("EDCCoding Cs format error!\n");
            return false;
        }
    }
    
    return true;
}

bool Check::checkCodingC(_IN const string &zipCodingC, int gaborDirections)
{
    for(size_t i = 0; i < zipCodingC.length(); ++i) {
        if(zipCodingC[i] >= hexArray[gaborDirections]) {
            return false;
        }
    }
    return true;
}



