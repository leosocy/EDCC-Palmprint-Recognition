/*************************************************************************
	> File Name: IO..cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/18 22:00:10
 ************************************************************************/

#include <Check.h>
using namespace EDCC;

bool Check::checkConfigValid(_IN const map<string, int> &configMap)
{
    if(configMap.find(IMAGE_SIZE) == configMap.end()
       || configMap.find(GABOR_KERNEL_SIZE) == configMap.end()
       || configMap.find(GABOR_DIRECTIONS) == configMap.end()
       || configMap.find(LAPLACE_KERNEL_SIZE) == configMap.end()) {
        cerr << "Load config fail, config params miss!" << endl;
        return false;
    }

    imageSize = configMap.at(IMAGE_SIZE);
    gaborKernelSize = configMap.at(GABOR_KERNEL_SIZE);
    gaborDirections = configMap.at(GABOR_DIRECTIONS);
    laplaceKernelSize = configMap.at(LAPLACE_KERNEL_SIZE);

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
       || laplaceKernelSize % 2 == 0
       || laplaceKernelSize > CONFIG_VALID_LAPLACE_KERNEL_SIZE_MAX) {
        cerr << "Laplace Kernel Size must be smaller than imageSize."
            << "And must be odd and samller than 31!" << endl;
        return false;
    }
    if(gaborDirections > CONFIG_VALID_GABOR_DIRECTIONS_MAX
       || gaborDirections < CONFIG_VALID_GABOR_DIRECTIONS_MIN) {
        cerr << "Gabor Directions must in range [" << CONFIG_VALID_GABOR_DIRECTIONS_MIN
            << ", " << CONFIG_VALID_GABOR_DIRECTIONS_MAX << "]!" << endl;
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
            if(dataIte->imagePath == dataInnerIte->imagePath) {
                cerr << "Image Path: " << dataIte->imagePath << " Conflict!" << endl;
                return false;
            }
        }
    }
    
    return true;
}

bool Check::checkPalmprintFeatureData(_IN const vector<PalmprintCode> &data)
{
    vector<PalmprintCode>::const_iterator dataIte;
    for(dataIte = data.begin(); dataIte != data.end(); ++dataIte) {
        if((dataIte->zipCodingC).length() != imageSize * imageSize
            || !checkCodingC(dataIte->zipCodingC)) {
            cerr << "EDCCoding C format error!" << endl;
            return false;
        }
        if((dataIte->zipCodingCs).length() != (imageSize * imageSize / 4 + 1)) {
            cerr << "EDCCoding Cs format error!" << endl;
            return false;
        }
    }
    
    return true;
}

bool Check::checkCodingC(_IN const string &zipCodingC)
{
    for(size_t i = 0; 
        i < zipCodingC.length()
        && gaborDirections < CONFIG_VALID_GABOR_DIRECTIONS_MAX;
        ++i) {
        if(zipCodingC[i] >= hexArray[gaborDirections]) {
            return false;
        }
    }
    return true;
}



