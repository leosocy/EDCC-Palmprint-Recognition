/*************************************************************************
	> File Name: gabor.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 15:50:09
 ************************************************************************/

#include "gabor.h"

GaborFilter::GaborFilter()
{
	//init
	this->kernelSize = Size( 5, 5 );
	this->Kmax = CV_PI / 2;
	this->f = sqrt( 2.0 );
	this->sigma = 2 * CV_PI;
	this->numOfScales = 5;
	this->numOfDirections = 8;
	this->ktype = CV_64F;
}

GaborFilter::~GaborFilter()
{
}

void GaborFilter::doBatchGaborFilter( const Mat &src, Mat &result, int kernelType )
{
	CV_Assert( this->kernelSize.width % 2 == 1 && this->kernelSize.height % 2 == 1 ); //kernel size(weight, height) should be odd
	CV_Assert( kernelType == GaborFilter::GABOR_KERNEL_REAL || kernelType == GaborFilter::GABOR_KERNEL_IMAG || kernelType == GaborFilter::GABOR_KERNEL_MAG );

	int U = 0, V = 0;
	int UStart = 0, UEnd = this->numOfScales;
	int VStart = 0, VEnd = this->numOfDirections;

	Mat dst_t;
	// variables for filter2D  
	Point archor( -1, -1 );
	//int ddepth = CV_64F;  
	//double delta = 0;

	// filter image with gabor bank  
	Mat totalMat;
	for ( U = UStart; U < UEnd; U++ ) {
		Mat colMat;
		for ( V = VStart; V < VEnd; V++ ) {
			doOnceGaborFilter( src, dst_t, U, V, kernelType );
			if (V == VStart) {
				colMat = dst_t;
			} else{
				hconcat( colMat, dst_t, colMat );
			}
		}
		if ( U == UStart ) {
			totalMat = colMat;
		} else{
			vconcat( totalMat, colMat, totalMat );
		}
	}
	
	result = totalMat.clone();	
}

void GaborFilter::doOnceGaborFilter( const Mat &src, Mat &result, int scale, int direction, int kernelType )
{
	CV_Assert( this->kernelSize.width % 2 == 1 && this->kernelSize.height % 2 == 1 ); //kernel size(weight, height) should be odd
	CV_Assert( kernelType == GaborFilter::GABOR_KERNEL_REAL || kernelType == GaborFilter::GABOR_KERNEL_IMAG || kernelType == GaborFilter::GABOR_KERNEL_MAG );

	int GaborH = this->kernelSize.height;
	int GaborW = this->kernelSize.width;
	Mat gaborKernel;
 	int ddepth = CV_64F;

	getGaborKernel( gaborKernel, GaborW, GaborH, scale, direction, this->Kmax, this->f, this->sigma, this->ktype, kernelType );
	filter2D( src, result, ddepth, gaborKernel );
	normalize( result, result, 0, 1, CV_MINMAX );
}

void GaborFilter::showGaborFilter( Mat &result, int kernelType )
{
	CV_Assert( kernelType == GaborFilter::GABOR_KERNEL_REAL || kernelType == GaborFilter::GABOR_KERNEL_IMAG || kernelType == GaborFilter::GABOR_KERNEL_MAG );

	int GaborH = 69;
	int GaborW = 69;
	Mat gaborKernel;
 	int ddepth = CV_64F;

	int U = 0, V = 0;
	int UStart = 0, UEnd = this->numOfScales;
	int VStart = 0, VEnd = this->numOfDirections;

	Mat dst_t; 
	Point archor( -1, -1 );
	Mat totalMat;
	for ( U = UStart; U < UEnd; U++ ) {
		Mat colMat;
		for ( V = VStart; V < VEnd; V++ ) {
			getGaborKernel( gaborKernel, GaborW, GaborH, U, V, this->Kmax, this->f, this->sigma, this->ktype, kernelType );
			normalize( gaborKernel, gaborKernel, 0, 1, CV_MINMAX );
			if (V == VStart) {
				colMat = gaborKernel;
			} else{
				hconcat( colMat, gaborKernel, colMat );
			}
		}
		if ( U == UStart ) {
			totalMat = colMat;
		} else{
			vconcat( totalMat, colMat, totalMat );
		}
	}
	result = totalMat.clone();
}

void GaborFilter::getGaborKernel( Mat &gaborKernel, int kernelWidth, int kernelHeight, int scale, int direction, double Kmax, double f, double sigma, int ktype, int kernelType )
{
	CV_Assert( ktype == CV_32F || ktype == CV_64F );  
	CV_Assert( kernelType == GaborFilter::GABOR_KERNEL_REAL || kernelType == GaborFilter::GABOR_KERNEL_IMAG || kernelType == GaborFilter::GABOR_KERNEL_MAG );
	int half_width = kernelWidth / 2;
	int half_height = kernelHeight / 2;
	double Qu = CV_PI * direction / this->numOfDirections;
	double sqsigma = sigma * sigma;
	double Kv = Kmax / pow( f, scale );
	double postmean = exp( -sqsigma / 2 );
	Mat kernel( kernelWidth, kernelHeight, ktype );
	Mat kernel_2( kernelWidth, kernelHeight, ktype );
	Mat kernel_mag( kernelWidth, kernelHeight, ktype );

	double tmp1, tmp2, tmp3;
	for ( int j = -half_height; j <= half_height; j++ ){
		for ( int i = -half_width; i <= half_width; i++ ){
			tmp1 = exp( -( Kv * Kv * ( j * j + i * i ) ) / ( 2 * sqsigma ) );
			tmp2 = cos( Kv * cos( Qu ) * i + Kv * sin( Qu ) * j ) - postmean;
			tmp3 = sin( Kv * cos( Qu ) * i + Kv * sin( Qu ) * j );

			if( kernelType == GaborFilter::GABOR_KERNEL_REAL ) {
				if (ktype == CV_32F) {
					kernel.at<float>( j + half_height, i + half_width ) =
					(float)( Kv * Kv * tmp1 * tmp2 / sqsigma );
				} else {
					kernel.at<double>( j + half_height, i + half_width ) =
					(double)( Kv * Kv * tmp1 * tmp2 / sqsigma );
				}
			} else if( kernelType == GaborFilter::GABOR_KERNEL_IMAG ) {
				if (ktype == CV_32F) {
					kernel.at<float>( j + half_height, i + half_width ) =
					(float)( Kv * Kv * tmp1 * tmp3 / sqsigma );
				} else {
					kernel.at<double>( j + half_height, i + half_width ) =
					(double)( Kv * Kv * tmp1 * tmp3 / sqsigma );
				}
			} else {
				if (ktype == CV_32F) {
					kernel.at<float>( j + half_height, i + half_width ) =
					(float)( Kv * Kv * tmp1 * tmp2 / sqsigma );
					kernel_2.at<float>( j + half_height, i + half_width ) =
					(float)( Kv * Kv * tmp1 * tmp3 / sqsigma );
				} else {
					kernel.at<double>( j + half_height, i + half_width ) =
					(double)( Kv * Kv * tmp1 * tmp2 / sqsigma );
					kernel_2.at<double>( j + half_height, i + half_width ) =
					(double)( Kv * Kv * tmp1 * tmp3 / sqsigma );
				}
				
			}
		}
	}
	
	if( kernelType == GaborFilter::GABOR_KERNEL_MAG ) {
		magnitude(kernel, kernel_2, kernel_mag);
	}

	if ( kernelType == GaborFilter::GABOR_KERNEL_MAG ) {
		gaborKernel = kernel_mag.clone();
	} else {
		gaborKernel = kernel.clone();
	}
}

