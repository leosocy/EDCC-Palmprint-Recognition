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
#include <Pub.h>
using namespace std;

#define CONFIG_IMAGE_SIZE_W_MIN 29
#define CONFIG_IMAGE_SIZE_H_MIN 29
#define CONFIG_VALID_LAPLACE_KERNEL_SIZE_MAX 31
#define CONFIG_VALID_GABOR_DIRECTIONS_MIN 4
#define CONFIG_VALID_GABOR_DIRECTIONS_MAX 16

namespace EDCC
{
    class PalmprintCode;
    class EDCCoding;
    typedef struct tag_EDCC_CFG_T EDCC_CFG_T;
    class Check {
    public:
        bool checkConfigValid(_IN const EDCC_CFG_T &config);
        bool checkPalmprintGroupValid(_IN const vector<PalmprintCode> &data);
        bool checkPalmprintFeatureData(_IN const vector<PalmprintCode> &data,
                                       _IN const EDCC_CFG_T &config);
        bool checkTwoPalmprintCodeConfigEQAndValid(_IN const PalmprintCode &firstPalmprintCode,
                                                   _IN const PalmprintCode &secondPalmprintCode);
    private:
        bool checkCodingMagicKey(_IN const EDCCoding &coding);
        bool checkCoding(_IN const EDCCoding &coding);
        bool checkTwoConfigEQAndValid(_IN const EDCC_CFG_T &firstConfig,
                                      _IN const EDCC_CFG_T &secondConfig);
    };
}

#endif
