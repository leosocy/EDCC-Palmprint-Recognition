/*************************************************************************
	> File Name: coding_algorithm.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 21:08:25
 ************************************************************************/

#ifndef __CODING_ALGORITHM_H__
#define __CODING_ALGORITHM_H__

#include "../features_base.h"

#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include <cmath>
#include <assert.h>
using namespace std;

class DRCC : public PRFeatures
{
public:
	DRCC();
	~DRCC();
	
	int saveFeatures( const char *filename );
	int loadFeatures( const char *filename );
	
	int doBatchDRCC( const char *filename );
	int doOnceDRCC( const Mat &src, const string &label );
	

private:
	int numOfScales;
	int numOfDirections;
	int getMaxGaborResponse( const Mat &src, Mat &result, int scale, int direction, int kernelType );
};

#endif /* end of coding_algorithm */
