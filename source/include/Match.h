/*************************************************************************
	> File Name: Match.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/08/01 22:42:15
 ************************************************************************/
#ifndef __MATCH_H__
#define __MATCH_H__
#include <opencv2/opencv.hpp>
#include <Pub.h>
using namespace std;

namespace EDCC {
    class PalmprintCode;
    class Match {
    public:
        double matchP2P(_IN const PalmprintCode &instance1, 
                        _IN const PalmprintCode &instance2);
    };
}

#endif 
