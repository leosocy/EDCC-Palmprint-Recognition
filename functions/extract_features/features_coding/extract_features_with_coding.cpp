/*************************************************************************
	> File Name: extract_features_with_coding.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/03 16:40:42
 ************************************************************************/

#include "extract_features_with_coding.h"

int train_coding( const char *trainlist )
{
	FILE *roi_list = fopen( trainlist, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", trainlist );
		return EXIT_FAILURE;
	}

	string feature_string = "feature_string_info/CODING_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< Mat >v_feature;

	clock_t bt = clock();
	printf( "--------------------------------Begin Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );

		n = feature_file[num];
		if( n.empty() ) {
			printf( "Begin Write Num:%d\n", i );
			feature_file.open( feature_string, FileStorage::APPEND );
			int id = 0;
			char image_path[200];
			fscanf( roi_list, "%d %s", &id, image_path );
			Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize(image_gray, image_gray, dsize);
			//Mat gaborResult;
			//GaborFilter filter;
			//filter.numOfDirections = 12;
			//filter.doBatchGaborFilter( image_gray, gaborResult, GaborFilter::GABOR_KERNEL_REAL );
			DRCC d;
			d.doOnceDRCC( image_gray, "1" );
			//imshow( "gabor", gaborResult );
			waitKey();
			/*************************extract features with WDT********************************/
			/*WDT( image_gray, image_gray, "haar", WAVELET_LEVEL );
			Mat features;
			Size block( WAVELET_BLOCK_SIZE );
			get_features( image_gray, FEATURES_STATISTIC_TYPE, features, block, i );

			
			feature_file << s_num;
			feature_file << "{";
			feature_file << "id" << id;
			feature_file << "imagepath" << image_path;
			feature_file << "features" << features;
			feature_file << "}";*/
			printf( "End Write Num:%d\n\n", i );
		} else {
			
		}
	}
	feature_file.release();
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	
	fflush( stdout );
	//calc_dis( v_feature );
	/************************************end of WDT************************************/

	/*************************extract features with DFT********************************/
	/************************************end of DFT************************************/
	return EXIT_SUCCESS;
}

