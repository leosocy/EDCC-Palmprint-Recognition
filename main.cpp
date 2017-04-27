/*************************************************************************
	> File Name: main.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/13 20:11:46
 ************************************************************************/

#include <stdio.h>
#include "functions.hpp"
#include "global_definition.h"


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
	//VideoCapture video( "../TestVideo/2016-12-25 184700.mp4" );
	 
	/*if( !video.isOpened() ) {
		printf( "No Video Data!" );
		exit( EXIT_FAILURE );
	}*/

	/*double rate = video.get( CV_CAP_PROP_FPS );
	bool stop( false );*/
	//Mat image;
	//int delay = 1000 / rate;
	/*video.read( image );
	double proportion = (double)image.cols / image.rows;
	Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );*/
	
	int successful_count = 0;
	int sum = 0;
	
	FILE *list_multispectral_B = fopen( "./roi_list/Multispectral_B.txt", "r" );
	FILE *list_multispectral_G = fopen( "./roi_list/Multispectral_G.txt", "r" );
	FILE *list_multispectral_I = fopen( "./roi_list/Multispectral_I.txt", "r" );
	FILE *list_multispectral_R = fopen( "./roi_list/Multispectral_R.txt", "r" );
	if( list_multispectral_B == NULL ) {
		roi_list_with_multispectral( "/home/leosocy/Desktop/PalmprintData/Multispectral/Multispectral_B/", "./roi_list/Multispectral_B.txt" );
	}
	if( list_multispectral_G == NULL ) {
		roi_list_with_multispectral( "/home/leosocy/Desktop/PalmprintData/Multispectral/Multispectral_G/", "./roi_list/Multispectral_G.txt" );
	}
	if( list_multispectral_I == NULL ) {
		roi_list_with_multispectral( "/home/leosocy/Desktop/PalmprintData/Multispectral/Multispectral_I/", "./roi_list/Multispectral_I.txt" );
	}
	if( list_multispectral_R == NULL ) {
		roi_list_with_multispectral( "/home/leosocy/Desktop/PalmprintData/Multispectral/Multispectral_R/", "./roi_list/Multispectral_R.txt" );
	}
	FILE *list_to_deal = list_multispectral_G;
	const char *list_to_deal_ch = "./roi_list/Multispectral_B.txt";
#if INIT_TRAIN_TEST_LIST
	split_roi_list( list_to_deal_ch, "./roi_list/Multispectral_B_Train.txt", "./roi_list/Multispectral_B_Test.txt", LIST_TRAIN_NUM_PER + LIST_TEST_NUM_PER, LIST_TRAIN_NUM_PER );
#endif

	//while( !feof(list_to_deal) ) {
	//	int image_num = 0;
	//	char image_name[200];
	//	fscanf( list_to_deal, "%d %s", &image_num, image_name );
	//	Mat image = imread( image_name, CV_LOAD_IMAGE_COLOR );
	//	if( image.empty() ) break;
	//	double proportion = (double)image.cols / image.rows;
	//	Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
	//	sum++;
	//	origin_image_without_scale = image.clone();
	//	image_scale = (double)IMAGE_HEIGHT / image.rows;
	//	resize(image, image, dsize);
		/*origin_image = image.clone();
		Mat dst;
		dst = Mat::zeros( image.rows, image.cols, CV_8U);
		binarization( image, dst );
		adjust_palm( dst, dst );
		imshow( "Origin", origin_image );
		start_timing();
		Mat roi;
		if( extract_roi( dst, roi ) == EXIT_SUCCESS ) {
			resize( roi, roi, Size( 512, 512 ) );*/
		//	Mat roi_g;
		//	cvtColor( image, roi_g, CV_BGR2GRAY );
		//	binarization( roi, roi_d );
		//	imshow( "roi_g", roi_g ); 
			char list_name_train[] = "./roi_list/Multispectral_B_Train.txt";
			char list_name_test[] = "./roi_list/Multispectral_B_Test.txt";
		//	create_predict_list( "./roi_list/Multispectral_B.txt", "./roi_list/Multispectral_B_Predict_Train.txt",  "./roi_list/Multispectral_B_Predict_Test.txt" );
		//	predict_subspace( "./roi_list/Multispectral_B_Predict_Train.txt",  "./roi_list/Multispectral_B_Predict_Test.txt" );
		
			DRCC d;
			int direc[7] = { 6, 8, 9, 10, 12, 15, 18 };
			int kernelSize[10] = { 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 };
			for( int i = 29; i < 125; i += 2 ) {
				for( int j = 27; j <= i; j+= 2 ) {
					for( int k = 0; k < 7; ++k ) {
						for( int l = 0; l < 10 && kernelSize[l] <= i; ++l ) {
								d.imageSize = Size( i, i );
								d.laplaceSize = j;
								d.gaborDirections = direc[k];
								d.gaborKernelSize = Size( kernelSize[l], kernelSize[l] );
								d.CVector.clear();
								d.CsVector.clear();
								d.labels.clear();
								d.doBatchDRCC( list_name_train );
						}
					}
				}
			}
		
			/*Mat image = imread(  "/home/leosocy/Desktop/PalmprintData/Multispectral/Multispectral_B/003/1_02_s.bmp", CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize(image_gray, image_gray, dsize);
			ImageEnhance enhance;
			Mat dst;
			Mat t;	
			image_gray.convertTo( t, CV_64FC1 );
			normalize( t, t, 0, 1, CV_MINMAX );	
			/*for( int h = 0; h < image_gray.rows; ++h ) {
				for( int w = 0; w < image_gray.cols; ++w ) {
					t.at<double>( h, w ) = 1 - t.at<double>( h, w );
				}
			}
			enhance.enhanceWithLaplace( t, dst, 7  );
			dst.convertTo( dst, CV_8U );
			enhance.enhanceWithCanny( dst, dst, 80, 240  );
			imshow( "enhance", dst );
			dst.convertTo( dst, CV_8U );
			
			binarization( dst, dst );
			
			imshow( "enhance_bi", dst );
			waitKey();*/
			//train_coding( list_name_train );
		/*	getchar();
			test_subspace( list_name_test );*/
			/*train_statistic( list_name_train );
			
			fflush( stdin );
			getchar();
			fflush( stdout );
			test_statistic( list_name_test );*/
		//	calcu_eer_threshold_subspace( "./roi_list/Multispectral_B.txt" );
		//	imshow( "roi_wavelet", roi_g );
	//		successful_count++;	
		
		//printf( "peak cost time %lf \n", stop_timing() );

	//	waitKey();
	#if _DEBUG_
		//imshow( "Origin Image", origin_image );
	#endif
		//if( waitKey( delay ) >= 0 ) stop = true;
	//}
	//printf( " Success : %d  Fail : %d ", successful_count, sum - successful_count ); 
	//fflush( stdout );
	//waitKey();
}
