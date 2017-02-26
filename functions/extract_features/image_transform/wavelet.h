/*************************************************************************
	> File Name: wavelet.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/02/25 22:19:54
 ************************************************************************/
#ifndef __WAVELET_H__
#define __WAVELET_H__

#include <opencv2/opencv.hpp>
using namespace cv;

extern void WDT( const Mat &src, Mat &dst, const string &w_name, const int level );
extern void IWDT( const Mat &src, Mat &dst, const string &w_name, const int level );

#endif /* end wavelet.h */
