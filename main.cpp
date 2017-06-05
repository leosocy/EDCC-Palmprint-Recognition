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
#if 0
	VideoCapture video( "../PalmprintData/2017-06-02 232612.mp4" );
	 
	if( !video.isOpened() ) {
		printf( "No Video Data!" );
		exit( EXIT_FAILURE );
	}

	double rate = video.get( CV_CAP_PROP_FPS );
	bool stop( false );
	Mat image;
	int delay = 1000 / rate;
	while( !stop ) {
		
		if( !video.read( image ) ) break;
		clock_t bt = clock(), et;
		//image = imread( "../PalmprintData/palm_rotate.jpg", CV_LOAD_IMAGE_COLOR );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
	
		origin_image_without_scale = image.clone();
		image_scale = (double)IMAGE_HEIGHT / image.rows;
		resize(image, image, dsize);
		origin_image = image.clone();
		Mat dst;
		dst = Mat::zeros( image.rows, image.cols, CV_8U);
		binarization( image, dst );
		//imshow( "Binarization", dst );
		adjust_palm( dst, dst );
		//imshow( "Adjust", dst );
		imshow( "原始图片", origin_image );
		
	
	//start_timing();
		Mat roi;
		if( extract_roi( dst, roi ) == EXIT_SUCCESS ) {
			resize( roi, roi, Size( 256, 256 ) );
			et = clock();
			imshow( "感兴趣区域", roi ); 
			printf( "Extract ROI successful!\tCost Time : %lfms\n",  ( ( double )et - bt ) / CLOCKS_PER_SEC * 1000 );
		} else {
			printf( "Bad Image.Can't extract ROI!\n" );		
		}
		
		waitKey();
	}
	int successful_count = 0;
	int sum = 0;
#endif
	FILE *list_multispectral_B = fopen( "./roi_list/Multispectral_B.txt", "r" );
	FILE *list_multispectral_G = fopen( "./roi_list/Multispectral_G.txt", "r" );
	FILE *list_multispectral_I = fopen( "./roi_list/Multispectral_I.txt", "r" );
	FILE *list_multispectral_R = fopen( "./roi_list/Multispectral_R.txt", "r" );
	FILE *list_tongji = fopen( "./roi_list/Tongji.txt", "r" );
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
	if( list_tongji == NULL ) {
		roi_list_with_tongji( "/home/leosocy/Desktop/PalmprintData/Tongji/", "./roi_list/Tongji.txt" );	
	}
	/*FILE *list_to_deal = list_multispectral_G;
	const char *list_to_deal_ch = "./roi_list/Multispectral_B.txt";
#if INIT_TRAIN_TEST_LIST
	split_roi_list( list_to_deal_ch, "./roi_list/Multispectral_B_Train.txt", "./roi_list/Multispectral_B_Test.txt", LIST_TRAIN_NUM_PER + LIST_TEST_NUM_PER, LIST_TRAIN_NUM_PER );*/


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

			
		//	predict_subspace( "./roi_list/Multispectral_B_Predict_Train.txt",  "./roi_list/Multispectral_B_Predict_Test.txt" );
#if 1
			
			/*int direc[7] = { 6, 8, 9, 10, 12, 15, 18 };
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
			}*/
			
			
			

			/*DRCC p;
			p.doBatchDRCC( "./roi_list/Multispectral_B_Predict_Test.txt" );

			PRMLEDRCC ml( 0.464633, 1 );
			
			PRVerify ver;
			ver.trainFeatures = &t;
			ver.predictFeatures = &p;
			ver.doPRML( &ml );*/
#endif

#if 1
			const char *list_all = "./roi_list/Multispectral_I.txt";
			const char *list_train = "./roi_list/Multispectral_I_Train.txt";
			const char *list_test = "./roi_list/Multispectral_I_Test.txt";
			const char *resultFile = "./feature_info/Identification/Identification_NIR.txt";
			for( int i = 1; i < 7; ++i ) {
				create_predict_list( list_all, list_train, list_test, 12, i, 12 - i );
				DRCC t1;
				t1.bIsEDCC = true;
				t1.doIdentification( list_train, list_test, 500, i, 12 - i, resultFile );
				DRCC t2;
				t2.bIsEDCC = false;
				t2.doIdentification( list_train, list_test, 500, i, 12 - i, resultFile );
				WBS w;
				w.waveletLevel = 4;
				w.blockSize = Size( 8, 8 );
				w.doIdentification( list_train, list_test, 500, i, 12 - i, resultFile );
				BDPCALDA b;
				b.Krow = 25;
				b.Kcol = 25;
				b.doIdentification( list_train, list_test, 500, i, 12 - i, resultFile );
			}
#endif 
			
#if 0
				DRCC t;
				t.bIsEDCC = true;
				t.doBatchDRCC( "./roi_list/Multispectral_B.txt" );
				//t.doVerification( 6000 );
				//t.doIdentification( list_train, list_test, 500, 4, 8, );
				/*WBS w;
				w.waveletLevel = 4;
				w.blockSize = Size( 8, 8 );
				//w.doExtractFeatures( "./roi_list/Multispectral_B.txt" );
				//w.doVerification( 6000 );
				w.doIdentification( list_train, list_test, 500, 1, 11 );*/
				/*BDPCALDA b;
				b.trainNum = 12;
		
				b.Krow = 25;
				b.Kcol = 25;
				b.doIdentification( list_train, list_test, 500, 4, 8, "./feature_info/Identification/Identification_Red.txt" );*/
				//./roi_list/Tongji.txt
				//b.doExtractFeatures( "./roi_list/Multispectral_B.txt" );
				//b.doVerification( 6000 );
#endif
#if 0
			Mat image = imread(  "/home/leosocy/Desktop/PalmprintData/ForPreprocessTest.jpg", CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize(image_gray, image_gray, dsize);
			origin_image_without_scale = image.clone();
			image_scale = (double)IMAGE_HEIGHT / image.rows;
			resize( image, image, dsize );
			origin_image = image.clone();
			Mat dst;
			dst = Mat::zeros( image.rows, image.cols, CV_8U);
			binarization( image, dst );
			imshow( "Binarization", dst );
			adjust_palm( dst, dst );
			imshow( "Adjust", dst );
			imshow( "Origin", origin_image );
		
	
		//start_timing();
			Mat roi;
			if( extract_roi( dst, roi ) == EXIT_SUCCESS ) {
				resize( roi, roi, Size( 256, 256 ) );		
				imshow( "roi", roi ); 
			}
			waitKey();
			/*ImageEnhance enhance;
			Mat t;	
			image_gray.convertTo( t, CV_64FC1 );
			normalize( t, t, 0, 1, CV_MINMAX );	
			enhance.enhanceWithLaplace( t, dst, 23 );
			normalize( dst, dst, 0, 1, CV_MINMAX );
			imshow( "enhance", dst );
			waitKey();*/
#endif
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
