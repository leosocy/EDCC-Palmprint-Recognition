/*************************************************************************
	> File Name: pca.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/12 20:47:51
 ************************************************************************/

#ifndef __PCA_H__
#define __PCA_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <vector>
#include <stdio.h>
using namespace std;

extern void Do2DPCA( const vector< Mat > &palm_all, Mat &U_Result, double threshold, int index );	//Two dimension PCA
extern void DoBDPCA( const vector< Mat > &palm_all, Mat &U_Result, Mat &V_Result, double threshold, int index_row, int index_col );	//Bidirectinal Two Dimension PCA   result is The feature vector used to reduce the dimension.

#endif /* end pca.h.h */
