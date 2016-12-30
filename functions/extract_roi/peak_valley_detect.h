/*************************************************************************
	> File Name: peak_valley_detect.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/17  00:12:14
 ************************************************************************/

#ifndef __PEAK_VALLEY_DETECT_H__
#define __PEAK_VALLEY_DETECT_H__

#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#include "../../global/global_definition.h"
#include "../../global/general_function.h"



/**
 * src : binary image && just one contour( palm )
 *
 * peak : store finger peak	( three element. whether left or right hand these are peak[0]--middle finger peak		peak[1]--finger which is left on middle finger peak[2]--finger which is right on middle finger )
 *
 * valley : store  valley between finger( four element. whether left or right hand these are valley[0]--valley between third finger and second finger		valley[1]--valley between third finger and fourth finger)
 *
 * return EXIT_SUCCESS : if the result is legal		EXIT_FAILURE : if the result is illegal
 *
 */
int peak_valley_detect( const Mat &src, vector< cv::Point > &peak, vector< cv::Point > &valley );

#endif	/* peak_valley_detect.h */
