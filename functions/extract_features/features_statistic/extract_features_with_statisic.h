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

class WBS
{
public:
	WBS();
	~WBS();
	/*--------For Parameters Tuning------------*/
	int waveletLevel;
	Size blockSize;
	int trainNum;
	int trainTotalNum;
	/*----------------------------------------*/
	
	int doExtractFeatures( const char *filename );
	void doVerification( int dataSize );
	void doIdentification( const char *trainFileName, const char *testFileName, int peopleNum, int trainNum, int testNum, const char *resultFileName );
	void complexity( const char *imageName, int computeTime );
	double matchingPoint2Point( const Mat &X, const Mat &Y );
	vector< cv::Mat> features;
	vector< int > labels;
};


#endif/* end of extract_features_with_statistic.h */
