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
#define INIT_TRAIN_TEST_LIST 1

#define CV_COLOR_WHITE CV_RGB(255, 255, 255)
#define CV_COLOR_BLACK CV_RGB(0, 0, 0)

#define IMAGE_HEIGHT  128

#define FILL_CONTOURS_AREA_THRE 3000
#define FINGER_ERODE_ELEMENT_SIZE 2

#define PEAK_VALLEY_DETECT_FINGER_WIDTH_STEP IMAGE_HEIGHT / 100
#define PEAK_VALLEY_DETECT_FINGER_HEIGHT_STEP IMAGE_HEIGHT / 20

#define DIST_BETWEEN_CENTER_VALLEY 0

//-----------Train Test Args-------------------
#define FEATURES_STATISTIC_TYPE "WDT"
#define LIST_TRAIN_NUM_PER 8
#define LIST_TEST_NUM_PER 4
#define LIST_TRAIN_NUM_TOTAL 3200
#define LIST_TEST_NUM_TOTAL 2800
#define WAVELET_LEVEL 5
#define WAVELET_BLOCK_SIZE 4,4
#define WAVELET_MATCH_METHOD "AC"  // ED = Euclidean distance   AC = "Angle cosine"

#define PREDICT_TRAIN_NUMBER_OF_PEOPLE 400
#define PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE 8
#define ALL_PER_PEOPLE_NUMBER_OF_IMAGE 12
#define PREDICT_TEST_NUMBER_OF_PEOPLE 100
#define PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE 12  
#define CALCU_EER_NUMBER_OF_IMAGE 4800
#define PREDICT_THRESHOLD 0.94
//-----------Train Test Args-------------------

#endif	/* global_definition.h */
