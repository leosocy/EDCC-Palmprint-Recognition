/*************************************************************************
	> File Name: Match.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/08/01 22:42:15
 ************************************************************************/
#ifndef __MATCH_H__
#define __MATCH_H__
#include <opencv2/opencv.hpp>
#include <Core.h>
using namespace std;
using namespace cv;

namespace EDCC {
    class Match {
        public:
            double matchP2P(const PalmprintCode &instance1, const PalmprintCode &instance2);
    };
}

#endif 
