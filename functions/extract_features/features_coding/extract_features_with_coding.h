/*************************************************************************
	> File Name: extract_features_with_coding.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/29 21:15:03
 ************************************************************************/

#ifndef __EXTRACT_FEATURES_WITH_CODING_H__
#define __EXTRACT_FEATURES_WITH_CODING_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

#include "../../../global/global_definition.h"
#include "../image_transform/gabor.h"
#include "coding_algorithm.h"

typedef struct features_coding {
	int id;
	string imagepath;
	Mat features;
} features_coding;

int train_coding( const char *trainlist );
int test_coding( const char *testlist );
int calcu_eer_threshold_coding( const char *calculist );
int predict_coding( const char *trainlist, const char *predictlist );

#endif  /* end of extract_features_with_coding */
