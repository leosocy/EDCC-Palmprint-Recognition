/*************************************************************************
	> File Name: features_base.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/04 21:33:50
 ************************************************************************/

/********************This is a abstract class.Extracting features in different ways must inherit this abstract class, \\***********************and achieve the pure virtual function.*/

#ifndef __FEATURES_BASE_H__
#define __FEATURES_BASE_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

class PRFeatures
{
public:
	PRFeatures();
	virtual ~PRFeatures();

	virtual int saveFeatures( const char *filename ) = 0;
	virtual int loadFeatures( const char *filename ) = 0;

	vector< cv::Mat> features;
	vector< int > labels;
};

#endif /* end of features_base */
