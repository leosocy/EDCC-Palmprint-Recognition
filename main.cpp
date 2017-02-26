/*************************************************************************
	> File Name: main.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/13 20:11:46
 ************************************************************************/

#include <stdio.h>
#include "functions/functions.hpp"
#include "global/global_definition.h"

using namespace std;

Mat origin_image_without_scale;
Mat origin_image;
Mat origin_rotate_image;
double image_scale;

int main( int argc, char **argv )
{
	/*if( argc !=  2 ) {
		printf( "error" );
		exit( EXIT_FAILURE );
	}

	Mat image;
	image = imread( *++argvs , 1 );*/

	process( argc, argv );
	
	/*Open Video*/
	VideoCapture video( "../TestVideo/2016-12-25 184700.mp4" );
	 
	if( !video.isOpened() ) {
		printf( "No Video Data!" );
		exit( EXIT_FAILURE );
	}

	double rate = video.get( CV_CAP_PROP_FPS );
	bool stop( false );
	Mat image;
	int delay = 1000 / rate;
	video.read( image );
	double proportion = (double)image.cols / image.rows;
	Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
	
	int successful_count = 0;
	int sum = 0;

	while( !stop ) {
		if( !video.read( image ) ) break;
		sum++;
		origin_image_without_scale = image.clone();
		image_scale = (double)IMAGE_HEIGHT / image.rows;
		resize(image, image, dsize);
		origin_image = image.clone();
		Mat dst;
		dst = Mat::zeros( image.rows, image.cols, CV_8U);
		binarization( image, dst );
		adjust_palm( dst, dst );
		imshow( "Origin", origin_image );
		start_timing();
		Mat roi;
		if( extract_roi( dst, roi ) == EXIT_SUCCESS ) {
			//Mat roi_d = Mat::zeros( roi.size(), CV_8U );
		//	binarization( roi, roi_d );
		//	imshow( "roi_d", roi_d ); 
			WDT( roi, roi, "haar", 1 );
			imshow( "roi_wavelet", roi );
			successful_count++;	
		}
		
		printf( "peak cost time %lf \n", stop_timing() );

		//waitKey();
	#if _DEBUG_
		//imshow( "Origin Image", origin_image );
	#endif
		if( waitKey( delay ) >= 0 ) stop = true;
	}
	printf( " Success : %d  Fail : %d ", successful_count, sum - successful_count ); 
	fflush( stdout );
	waitKey();
}
