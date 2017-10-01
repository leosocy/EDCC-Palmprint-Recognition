/*************************************************************************
	> File Name: IO..cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/18 22:00:10
 ************************************************************************/

#include <Check.h>
using namespace EDCC;

bool Check::checkValid(const map<string, int> &configMap, const vector<PalmprintCode> &data)
{
    bool bRet = true;
    bRet = bRet && checkConfigValid(configMap);
    bRet = bRet && checkPalmprintGroupValid(data);
    bRet = bRet && checkPalmprintFeatureData(data);

    return bRet;
}

bool Check::checkConfigValid(const map<string, int> &configMap)
{
    if(configMap.find("imageSize") == configMap.end() 
        || configMap.find("laplaceKernelSize") == configMap.end()
        || configMap.find("gaborKernelSize") == configMap.end()
        || configMap.find("gaborDirections") == configMap.end()) {
        cerr << "Config error!" << endl;
        return false;
    }
    
    imageSize = configMap.at("imageSize");
    gaborKernelSize = configMap.at("gaborKernelSize");
    gaborDirections = configMap.at("gaborDirections");
    laplaceKernelSize = configMap.at("laplaceKernelSize");

    if(imageSize < CONFIG_IMAGE_SIZE_MIN) {
        cerr << "ImageSize can't smaller than " << CONFIG_IMAGE_SIZE_MIN << endl;
        return false;
    }
    if(gaborKernelSize > imageSize 
        || gaborKernelSize % 2 == 0) {
        cerr << "Gabor Kernel Size must be smaller than imageSize."
            << "And must be odd!" << endl;
        return false;
    }
    if(laplaceKernelSize > imageSize 
        || laplaceKernelSize % 2 == 0) {
        cerr << "Laplace Kernel Size must be smaller than imageSize."
            << "And must be odd!" << endl;
        return false;
    }
    if(gaborDirections > CONFIG_VALID_GABOR_DIRECTIONS_MAX
        || gaborDirections < CONFIG_VALID_GABOR_DIRECTIONS_MIN) {
        cerr << "Gabor Directions must in range [" << "CONFIG_VALID_GABOR_DIRECTIONS_MIN"
            << ", " << CONFIG_VALID_GABOR_DIRECTIONS_MAX << "]!" << endl;
        return false;
    }
        
    return true;
}

bool Check::checkPalmprintGroupValid(const vector<PalmprintCode> &data)
{
    
    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        for(vector<PalmprintCode>::const_iterator dataInnerIte = dataIte + 1;
            dataInnerIte != data.end(); ++dataInnerIte) {
            if(dataIte->imagePath == dataInnerIte->imagePath) {
                cerr << "Image Path: " << dataIte->imagePath << " Conflict!" << endl;
                return false;
            }
        }
    }
    
    return true;
}

bool Check::checkPalmprintFeatureData(const vector<PalmprintCode> &data)
{
    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        if((dataIte->zipCodingC).length() != imageSize * imageSize
            || !checkCodingC(dataIte->zipCodingC)) {
            cerr << "EDCCoding C format error!" << endl;
            return false;
        }
        if((dataIte->zipCodingCs).length() != imageSize * imageSize / 8 + 1) {
            cerr << "EDCCoding Cs format error!" << endl;
            return false;
        }
        
    }
    
    return true;
}

bool Check::checkCodingC(const string &codingC)
{
    for(size_t i = 0; 
        i < codingC.length() 
        && gaborDirections < CONFIG_VALID_GABOR_DIRECTIONS_MAX;
        ++i) {
        if(codingC.at(i) >= hexArray[gaborDirections]) {
            return false;
        }
    }
    return true;
}



