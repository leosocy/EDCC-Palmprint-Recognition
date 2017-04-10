/*************************************************************************
	> File Name: general_functions.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/10 20:25:38
 ************************************************************************/

#ifndef __GENERAL_FUNCTIONS_H__
#define __GENERAL_FUNCTIONS_H__

#include <opencv2/opencv.hpp>
using namespace cv;
#include <vector>
#include <assert.h>
#include <stdio.h>
using namespace std;

class GeneralFunctions
{
public:
	GeneralFunctions();
	~GeneralFunctions();

	int blockImage( const Mat &src, vector< Mat > &blockingResult, Size &blockingSize);
};


#endif  /*end of general_functions*/
