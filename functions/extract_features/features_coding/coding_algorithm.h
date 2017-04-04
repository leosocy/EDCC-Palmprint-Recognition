/*************************************************************************
	> File Name: coding_algorithm.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 21:08:25
 ************************************************************************/

#ifndef __CODING_ALGORITHM_H__
#define __CODING_ALGORITHM_H__

#include "../features_base.h"

#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include <cmath>
using namespace std;

class DRCC : public PRFeatures
{
public:
	DRCC();
	~DRCC();
	
	int saveFeatures( const char *filename );
	int loadFeatures( const char *filename );
}

#endif /* end of coding_algorithm */
