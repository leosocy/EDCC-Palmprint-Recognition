/*************************************************************************
	> File Name: Match.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/10 20:50:15
 ************************************************************************/

#ifndef __CHECK_H__
#define __CHECK_H__

#include <iostream>
#include <map>
#include <vector>
#include <Core.h>
#include <Pub.h>
using namespace std;

#define CONFIG_IMAGE_SIZE_MIN 29
#define CONFIG_VALID_LAPLACE_KERNEL_SIZE_MAX 31
#define CONFIG_VALID_GABOR_DIRECTIONS_MIN 4
#define CONFIG_VALID_GABOR_DIRECTIONS_MAX 16

namespace EDCC
{
    class Check {
    public:
        bool checkConfigValid(_IN const map<string, int> &configMap);
        bool checkPalmprintGroupValid(_IN const vector<PalmprintCode> &data);
        bool checkPalmprintFeatureData(_IN const vector<PalmprintCode> &data);
    private:
        bool checkCodingC(_IN const string &zipCodingC);

        int imageSize;
        int gaborKernelSize;
        int gaborDirections;
        int laplaceKernelSize;
    };
}

#endif
