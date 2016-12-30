/*************************************************************************
	> File Name: global_definition.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/13  21:56:14
 ************************************************************************/



#ifndef _GLOBAL_DEFINITION_H
#define _GLOBAL_DEFINITION_H

#include <opencv2/opencv.hpp>



#define _DEBUG_ 1

#define CV_COLOR_WHITE CV_RGB(255, 255, 255)
#define CV_COLOR_BLACK CV_RGB(0, 0, 0)

#define IMAGE_HEIGHT  800

#define FILL_CONTOURS_AREA_THRE 3000
#define FINGER_ERODE_ELEMENT_SIZE 2

#define PEAK_VALLEY_DETECT_FINGER_WIDTH_STEP IMAGE_HEIGHT / 100
#define PEAK_VALLEY_DETECT_FINGER_HEIGHT_STEP IMAGE_HEIGHT / 20

#define DIST_BETWEEN_CENTER_VALLEY 0


#endif	/* global_definition.h */
