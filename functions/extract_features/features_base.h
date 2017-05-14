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

#include "global_definition.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <assert.h>
using namespace std;

const string PRF_BASE_METHOD_NAME = "PRF_BASE";
const string PRF_DRCC_METHOD_NAME = "PRF_DRCC";
const string PRF_BDRCC_METHOD_NAME = "PRF_BDRCC";

class PRFeatures
{
public:
	PRFeatures();
	virtual ~PRFeatures();

	virtual int saveFeatures( const char *filename ) = 0;
	virtual int loadFeatures( const char *filename ) = 0;
	
	virtual void setMethodName( const string &methodName ) = 0;
	virtual string getMethodName() = 0;
	
	string methodName;
	vector< cv::Mat> features;
	vector< int > labels;
};

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
	virtual int saveFeatures( const char *filename );
	virtual int loadFeatures( const char *filename );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName();
	
	int doBatchDRCC( const char *filename );
	int doOnceDRCC( const Mat &src, int label );

	void doVerification( int dataSize );
	void doIndentification( int trainNum, int testNum );
	
	double matchingPoint2Point( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy );
	double matchingPoint2Area( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy, const Rect &areaRect );
	
private:
	int numOfScales;
	int numOfDirections;


	int getMaxGaborResponse( const Mat &src, Mat &result, Mat &C, Mat &Cs, Mat &fea, int scale, int direction, int kernelType );
};

class BDRCC : public PRFeatures		//Blocking DRCC
{
public:
	BDRCC();
	~BDRCC();
	
	int saveFeatures( const char *filename );
	int loadFeatures( const char *filename );
	
	virtual int doBatchBDRCC( const char *filename );
	virtual int doOnceBDRCC( const Mat &src, int label );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName();
	
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

#endif /* end of features_base */
