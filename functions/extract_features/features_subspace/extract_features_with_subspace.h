/*************************************************************************
	> File Name: extract_features_with_subspace.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/12 19:24:41
 ************************************************************************/

#ifndef __EXTRACT_FEATURES_WITH_SUBSPACE_H__
#define __EXTRACT_FEATURES_WITH_SUBSPACE_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
using namespace std;

#include "../../../global/global_definition.h"
#include "../image_reduce_dimension/pca.h"

typedef struct features_subspace {
	int id;
	string imagepath;
	Mat features;
} features_subspace_struct;

typedef struct lda_struct {
	int label;
	Mat img;
} lda_struct;

int train_subspace( const char *trainlist );
int test_subspace( const char *testlist );
int predict_subspace( const char *trainlist, const char *predictlist );
int calcu_eer_threshold_subspace( const char *calculist );

class BDPCALDA
{
public:
	BDPCALDA();
	~BDPCALDA();
	/*--------For Parameters Tuning------------*/
	int Krow;
	int Kcol;
	int trainNum;
	/*----------------------------------------*/
	
	int doExtractFeatures( const char *filename );
	void doVerification( int dataSize );
	double match( const Mat &X, const Mat &Y );
	vector< cv::Mat> features;
	vector< int > labels;
	Mat U, VT, LDAEIVECTOR;
};


#endif /* end of extract_features_with_subspace */
