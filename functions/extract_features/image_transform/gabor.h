/*************************************************************************
	> File Name: gabor.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 15:35:43
 ************************************************************************/

#ifndef __GABOR_H__
#define __GABOR_H__

#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include <cmath>
using namespace std;

/** type 0 : mag
 ** type 1 : real
 ** type 2 : imag
*/

extern void gaborFilter( const Mat &src, Mat &result, int scaleNum, int directionNum, cv::Size kernelSize, int type );

#endif /* end gabot.h */

