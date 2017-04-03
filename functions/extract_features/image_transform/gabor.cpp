/*************************************************************************
	> File Name: gabor.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 15:50:09
 ************************************************************************/

#include "gabor.h"

void getGabor( Mat &gabor, int kernelWidth, int kernelHeight, int scale, int direction, \
		double Kmax, double f, double sigma, int ktype, const string &kernel_name );
void construct_gabor_bank();

void gaborFilter( const Mat &src, Mat &result, int scaleNum, int directionNum, cv::Size kernelSize, int type )
{
	CV_Assert( kernelSize.width % 2 == 1 && kernelSize.height % 2 == 1 ); //kernel size(weight, height) should be odd

	double Kmax = CV_PI / 2;
	double f = sqrt( 2.0 );
	double sigma = 2 * CV_PI;
	int U = 0, V = 0;
	int GaborH = kernelSize.height;
	int GaborW = kernelSize.width;
	int UStart = 0, UEnd = directionNum;
	int VStart = 0, VEnd = scaleNum;

	Mat kernel_re, kernel_im, kernel_mag;
	Mat dst_re, dst_im, dst_mag;

	// variables for filter2D  
	Point archor( -1, -1 );
	int ddepth = CV_64F;  
	double delta = 0;

	// filter image with gabor bank  
	Mat totalMat, totalMat_re, totalMat_im;
	Mat totalGaborMat, totalGaborMat_re, totalGaborMat_im;
	for ( U = UStart; U < UEnd; U++ ) {
		Mat colMat, colMat_re, colMat_im;
		Mat colGaborMat, colGaborMat_re, colGaborMat_im;
		for ( V = VStart; V < VEnd; V++ ) {
			getGabor( kernel_re, GaborW, GaborH, U, V,
				Kmax, f, sigma, CV_64F, "real" );
			getGabor( kernel_im, GaborW, GaborH, U, V,
				Kmax, f, sigma, CV_64F, "imag" );
			/*getGabor( kernel_mag, GaborW, GaborH, U, V,
				Kmax, f, sigma, CV_64F, "mag" );*/
			// normalize kernel
			//normalize(kernel_re, kernel_re, 0, 255, CV_MINMAX);  
			//normalize(kernel_im, kernel_im, 0, 255, CV_MINMAX);  

			// flip kernel  
			// Gabor kernel is symmetric, so do not need flip  
			//flip(kernel_re, kernel_re, -1);  
			//flip(kernel_im, kernel_im, -1);  

			filter2D( src, dst_re, ddepth, kernel_re );
			filter2D( src, dst_im, ddepth, kernel_im );

			dst_mag.create( src.rows, src.cols, CV_64FC1 );
			magnitude( Mat_<float>( dst_re ), Mat_<float>( dst_im ), dst_mag);

			//show gabor kernel  
			normalize( dst_mag, dst_mag, 0, 1, CV_MINMAX );
			normalize( dst_re, dst_re, 0, 1, CV_MINMAX );
			normalize( dst_im, dst_im, 0, 1, CV_MINMAX );
			
			/*normalize( kernel_mag, kernel_mag, 0, 1, CV_MINMAX );
			normalize( kernel_re, kernel_re, 0, 1, CV_MINMAX );  
			normalize( kernel_im, kernel_im, 0, 1, CV_MINMAX );*/

			if (V == VStart) {
				colMat = dst_mag;
				colMat_re = dst_re;
				colMat_im = dst_im;
				
				/*colGaborMat = kernel_mag;
				colGaborMat_re = kernel_re;
				colGaborMat_im = kernel_im;*/
			}
			else{
				vconcat( colMat, dst_mag, colMat );
				vconcat( colMat_re, dst_re, colMat_re );
				vconcat( colMat_im, dst_im, colMat_im );

				/*vconcat( colGaborMat, kernel_mag, colGaborMat );
				vconcat( colGaborMat_re, kernel_re, colGaborMat_re );
				vconcat( colGaborMat_im, kernel_re, colGaborMat_im );*/
			}
		}
		if ( U == UStart ) {
			totalMat = colMat;
			totalMat_re = colMat_re;
			totalMat_im = colMat_im;
			
			/*totalGaborMat = colGaborMat;
			totalGaborMat_re = colGaborMat_re;
			totalGaborMat_im = colGaborMat_im;*/
		}
		else{
			hconcat( totalMat, colMat, totalMat );
			hconcat( totalMat_re, colMat_re, totalMat_re );
			hconcat( totalMat_im, colMat_im, totalMat_im );
			
			/*hconcat( totalGaborMat, colGaborMat, totalGaborMat );
			hconcat( totalGaborMat_re, colGaborMat_re, totalGaborMat_re );
			hconcat( totalGaborMat_im, colGaborMat_im, totalGaborMat_im );*/
		}
	}

	// return   
	switch ( type ){
	case 0: {
			//imshow( "gabor_kernel", totalGaborMat );
			result = totalMat.clone();
			break;
		}
	case 1: {
			//imshow( "gabor_kernel", totalGaborMat_re );
			result = totalMat_re.clone();
			break;
		}
	case 2: {
			//imshow( "gabor_kernel", totalGaborMat_im );
			result = totalMat_im.clone();
			break;
		}
	default: {
			//imshow( "gabor_kernel", totalGaborMat );
			result = totalMat.clone();
			break;
		}
	}
}


void getGabor( Mat &gabor, int kernelWidth, int kernelHeight, int scale, int direction, \
		double Kmax, double f, double sigma, int ktype, const string &kernel_name )
{
	//CV_ASSERT(width % 2 == 0 && height % 2 == 0);  
	CV_Assert(ktype == CV_32F || ktype == CV_64F);  

	int half_width = kernelWidth / 2;
	int half_height = kernelHeight / 2;
	double Qu = CV_PI * scale / 8;
	double sqsigma = sigma * sigma;
	double Kv = Kmax / pow( f, direction );
	double postmean = exp( -sqsigma / 2 );

	Mat kernel_re( kernelWidth, kernelHeight, ktype );
	Mat kernel_im( kernelWidth, kernelHeight, ktype );
	Mat kernel_mag( kernelWidth, kernelHeight, ktype );

	double tmp1, tmp2, tmp3;
	for ( int j = -half_height; j <= half_height; j++ ){
		for ( int i = -half_width; i <= half_width; i++ ){
			tmp1 = exp( -( Kv * Kv * ( j * j + i * i ) ) / ( 2 * sqsigma ) );
			tmp2 = cos( Kv * cos( Qu ) * i + Kv * sin( Qu ) * j ) - postmean;
			tmp3 = sin( Kv * cos( Qu ) * i + Kv * sin( Qu ) * j );

			if (ktype == CV_32F)
				kernel_re.at<float>(j + half_height, i + half_width) =
				(float)(Kv*Kv*tmp1*tmp2 / sqsigma);
			else
				kernel_re.at<double>(j + half_height, i + half_width) =
				(double)(Kv*Kv*tmp1*tmp2 / sqsigma);

			if (ktype == CV_32F)
				kernel_im.at<float>(j + half_height, i + half_width) =
				(float)(Kv*Kv*tmp1*tmp3 / sqsigma);
			else
				kernel_im.at<double>(j + half_height, i + half_width) =
				(double)(Kv*Kv*tmp1*tmp3 / sqsigma);
		}
	}

	magnitude(kernel_re, kernel_im, kernel_mag);

	if (kernel_name.compare("real") == 0)
		gabor = kernel_re.clone();
	else if (kernel_name.compare("imag") == 0)
		gabor = kernel_im.clone();
	else if (kernel_name.compare("mag") == 0 )
		gabor = kernel_mag.clone();
	else 
		printf("Invalid kernel name!\n");
}


void construct_gabor_bank()
{
	double Kmax = CV_PI/2;
	double f = sqrt(2.0);
	double sigma = 2*CV_PI;
	int U = 7;
	int V = 4;
	int GaborH = 129;
	int GaborW = 129;

	Mat kernel;
	Mat totalMat;
	for(U = 0; U < 8; U++){
		Mat colMat;
		for(V = 0; V < 5; V++){
			getGabor(kernel, GaborW, GaborH, U, V,
				Kmax, f, sigma, CV_64F, "real");

			//show gabor kernel
			normalize(kernel, kernel, 0, 1, CV_MINMAX);
			printf("U%dV%d\n", U, V);
			//imshow("gabor", kernel);
			//waitKey(0);

			if(V == 0)
				colMat = kernel;
			else
				vconcat(colMat, kernel, colMat);
		}
		if(U == 0)
			totalMat = colMat;
		else
			hconcat(totalMat, colMat, totalMat);
	}

	imshow("gabor bank", totalMat);
	//imwrite("gabor_bank.jpg",totalMat);
	waitKey(0);
}
