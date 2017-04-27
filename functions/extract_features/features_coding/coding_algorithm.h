/*************************************************************************
	> File Name: coding_algorithm.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 21:08:25
 ************************************************************************/

#ifndef __CODING_ALGORITHM_H__
#define __CODING_ALGORITHM_H__

#include "features_base.h"
#include "global_definition.h"

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
	vector< Mat > CVector;
	vector< Mat > CsVector;
	/*--------For Parameters Tuning------------*/
	Size imageSize;
	int laplaceSize;
	int gaborDirections;
	Size gaborKernelSize;
	/*----------------------------------------*/
	int saveFeatures( const char *filename );
	int loadFeatures( const char *filename );
	
	int doBatchDRCC( const char *filename );
	int doOnceDRCC( const Mat &src, int label );
	
	double matchingPoint2Point( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy );
	double matchingPoint2Area( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy, const Rect &areaRect );
	
private:
	int numOfScales;
	int numOfDirections;


	int getMaxGaborResponse( const Mat &src, Mat &result, Mat &C, Mat &Cs, int scale, int direction, int kernelType );
};

class BDRCC : public PRFeatures		//Blocking DRCC
{
public:
	BDRCC();
	~BDRCC();
	
	int saveFeatures( const char *filename );
	int loadFeatures( const char *filename );
	
	int doBatchBDRCC( const char *filename );
	int doOnceBDRCC( const Mat &src, int label );
	
	double matchingPoint2Point( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy );
	double matchingPoint2Area( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy, const Rect &areaRect );
private:
	int numOfScales;
	int numOfDirections;
	cv::Size blockingSize;
	cv::Size imageSize;

	vector< Mat > CVector;
	vector< Mat > CsVector;
};

#endif /* end of coding_algorithm */
