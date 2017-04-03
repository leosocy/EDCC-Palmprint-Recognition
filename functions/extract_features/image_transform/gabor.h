/*************************************************************************
	> File Name: gabor.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 15:35:43
 ************************************************************************/

#ifndef __GABOR_H__
#define __GABOR_H__

#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include <cmath>
using namespace std;

class GaborFilter
{
public:
	// Gabor kernel type
	enum { GABOR_KERNEL_REAL = 0, GABOR_KERNEL_IMAG, GABOR_KERNEL_MAG };
	GaborFilter();
	~GaborFilter();
	
	void doBatchGaborFilter( const Mat &src, Mat &result, int kernelType = GaborFilter::GABOR_KERNEL_REAL );
	void doOnceGaborFilter( const Mat &src, Mat &result, int scale, int direction, int kernelType = GaborFilter::GABOR_KERNEL_REAL );
	void getGaborKernel( Mat &gaborKernel, int kernelWidth, int kernelHeight, int scale, int direction, double Kmax, double f, double sigma, int ktype, int kernelType );	
	
	//Gabor parameter.Default kernelSize = ( 5, 5 ), Kmax = PI / 2, f = sqrt( 2.0 ), sigma = 2 * PI, numOfScales = 5, numOfDirections = 8, ktype = CV_64F
	cv::Size kernelSize;
	double Kmax;
	double f;
	double sigma;
	int numOfScales;
	int numOfDirections;
	int ktype;

private:

};

#endif /* end gabor.h */

