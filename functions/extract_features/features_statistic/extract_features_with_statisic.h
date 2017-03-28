/*************************************************************************
	> File Name: extract_features_with_statisic.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/04 21:01:19
 ************************************************************************/

#ifndef __EXTRACT_FEATURES_WITH_STATISTIC_H__
#define __EXTRACT_FEATURES_WITH_STATISTIC_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

#include "../../../global/global_definition.h"

typedef struct features_statistic {
	int id;
	string imagepath;
	Mat features;
} features_statistic_struct;


int train_statistic( const char *trainlist );
int test_statistic( const char *testlist );
int calcu_eer_threshold_statistic( const char *calculist );
int predict_statistic( const char *trainlist, const char *predictlist );


#endif/* end of extract_features_with_statistic.h */
