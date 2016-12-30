/*************************************************************************
	> File Name: adjust_palm.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/14  18:05:10
 ************************************************************************/

#ifndef __ADJUST_PALM_H__
#define __ADJUST_PALM_H__

#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#include "../../global/global_definition.h"
#include "../../global/general_function.h"

int adjust_palm( const Mat &src, Mat &dst );

#endif	/* adjust_palm.h  */
