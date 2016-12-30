/*************************************************************************
	> File Name: general_function.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/15  18:37:13
 ************************************************************************/

#ifndef _GENERAL_FUNCTION_H
#define _GENERAL_FUNCTION_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv; 

void start_timing();
double stop_timing();

int rotate_image( const Mat &src, Mat &dst, double angle, double scale );

#endif
