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

/**
 * remove small connected region just save the biggest region
 * */
int remove_small_area( const Mat &src, Mat &dst );

/**
 * fill internal contours which area is smaller than area_thre 
 */
int fill_contours( const Mat &src, Mat &dst, double area_thre );


/**
 * calculate the center of gravity
 */

int gravity_center( const Mat &src, cv::Point &center );

/**
 * find middle finger and rotate the image make the middle finger be top of the image.
 */

int find_middle_finger( const Mat &src, Mat &dst );

int adjust_palm( const Mat &src, Mat &dst );

#endif	/* adjust_palm.h  */
