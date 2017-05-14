/*************************************************************************
	> File Name: general_function.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/15  18:39:19
 ************************************************************************/

#include "general_function.h"
#include <time.h>

static clock_t begin_time = 0.0;
static clock_t end_time = 0.0;
static double cost_time = 0.0;


void start_timing( void )
{
	begin_time = clock();
}

double stop_timing( void )
{
	end_time = clock();
	cost_time = (double)( end_time - begin_time ) / CLOCKS_PER_SEC;
	return cost_time;
}

int rotate_image( const Mat &src, Mat &dst, double angle, double scale )
{
	double radian = angle / 180.0 * CV_PI;

	double sinValue = fabs( sin( radian ) );
	double cosValue = fabs( cos( radian ) );

	int length = sqrt( src.cols * src.cols + src.rows * src.rows);
	double width = (int)( src.cols * cosValue + src.rows * sinValue );
	double height = (int)( src.cols * sinValue + src.rows * cosValue );

	Mat tempImg( length, length , src.type(), Scalar(0) );
	int ROI_X = length / 2 - src.cols / 2;
	int ROI_Y = length / 2 - src.rows / 2;
	Rect ROIRect( ROI_X, ROI_Y, src.cols, src.rows );
	Mat tempImgROI2( tempImg, ROIRect );
	src.copyTo( tempImgROI2 );
	Size target_size( width, height );
	//resize( dst, dst, target_size );
	Point2f center = Point( length / 2, length / 2 );
	Mat affine_matrix = getRotationMatrix2D( center, angle, scale );
	warpAffine( tempImg, dst, affine_matrix, Size( length, length ) );
	int x = ( tempImg.cols - target_size.width ) / 2;
  int y = ( tempImg.rows - target_size.height ) / 2;

  Rect rect( x, y, target_size.width, target_size.height );
  dst = Mat( dst, rect );
	//resize( dst, dst, target_size );
	return EXIT_SUCCESS;
}

int roi_list_with_multispectral( const char *dir_path, const char *output_filename )
{
	FILE *multispectral_roi_list = fopen( output_filename, "w" );
	if( multispectral_roi_list == NULL ) {
		perror( "Create OutputFIle Failed!\n" );
		return EXIT_FAILURE;
	}

	for( int i = 1; i <= 500; i++ ) {
		for( int j = 1; j <= 2; j++ ) {
			for( int k = 1; k <= 6; k++ ) {
				printf( "%d %s%03d/%d_%02d_s.bmp\n", i, dir_path, i, j, k );
				fprintf( multispectral_roi_list, "%d %s%03d/%d_%02d_s.bmp\n", i, dir_path, i, j, k );
				
			}
		}
	}
	fclose( multispectral_roi_list );
	return EXIT_SUCCESS;
}

int roi_list_with_tongji( const char *dir_path, const char *output_filename )
{
	FILE *tongji_roi_list = fopen( output_filename, "w" );
	if( tongji_roi_list == NULL ) {
		perror( "Create OutputFIle Failed!\n" );
		return EXIT_FAILURE;
	}
	for( int i = 1; i <= 600; ++i ) {
		for( int j = 1; j <= 10; ++j ) {
			fprintf( tongji_roi_list, "%d %ssession1/%05d.bmp\n", i, dir_path, ( i - 1 ) * 10 + j );
			fprintf( tongji_roi_list, "%d %ssession2/%05d.bmp\n", i, dir_path, ( i - 1 ) * 10 + j );		
		}	
	}
	fclose( tongji_roi_list );
	return EXIT_SUCCESS;
}

int roi_list_with_polyu( const char *dir_path, const char *output_filename )
{
	return EXIT_SUCCESS;
}

int roi_list_with_cameraimage( const char *dir_path, const char *output_filename )
{
	return EXIT_SUCCESS;
}

int split_roi_list( const char *input_filename, const char *output_trainlist, const char *output_testlist, const int num_per_type, const int train_num )
{
	FILE *inputFile = fopen( input_filename, "r" );
	FILE *output_train = fopen( output_trainlist, "w" );
	FILE *output_test = fopen( output_testlist, "w" );

	if( inputFile == NULL ) {
		printf( "%s Not Exist!", input_filename );
		return EXIT_FAILURE;
	}
	for( int i = 0; !feof( inputFile ); i += num_per_type ) {
		for( int j = 0; j < train_num && !feof( inputFile ); j++ ) {
			char tmp[200];
			fgets( tmp, 200, inputFile );
			fputs( tmp, output_train );
			//fflush( output_train );
		}
		for( int j = train_num; j < num_per_type && !feof( inputFile ); j++ ) {
			char tmp[200];
			fgets( tmp, 200, inputFile );
			fputs( tmp, output_test );
			//fflush( output_test );
		}
	}
	fflush( output_train );
	fflush( output_test );
	return EXIT_SUCCESS;
}

int create_predict_list(  const char *alllist, const char *output_trainlist, const char *output_predictlist )
{
	FILE *inputFile = fopen( alllist, "r" );
	FILE *output_train = fopen( output_trainlist, "w" );
	FILE *output_predict = fopen( output_predictlist, "w" );

	if( inputFile == NULL ) {
		printf( "%s Not Exist!", alllist );
		return EXIT_FAILURE;
	}
	for( int i = 0, j = 0; !feof( inputFile ); i += ALL_PER_PEOPLE_NUMBER_OF_IMAGE, ++j ) {
		if( j < PREDICT_TRAIN_NUMBER_OF_PEOPLE ) {
			for( int k = 0; k < PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE && !feof( inputFile ); k++ ) {
				char tmp[200];
				fgets( tmp, 200, inputFile );
				fputs( tmp, output_train );
				fflush( output_train );
			}
			for( int k = PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE; k < ALL_PER_PEOPLE_NUMBER_OF_IMAGE && !feof( inputFile ); k++ ) {
				char tmp[200];
				fgets( tmp, 200, inputFile );
				fputs( tmp, output_predict );
				fflush( output_predict );
			}
		} else {
			for( int k = 0; k < ALL_PER_PEOPLE_NUMBER_OF_IMAGE && !feof( inputFile ); k++ ) {
				char tmp[200];	
				int id = 0;
				fscanf( inputFile, "%d %s", &id, tmp );
				fprintf( output_predict, "%d %s", -1, tmp );
				if( !feof( inputFile ) ) {
					fprintf( output_predict, "\n" );
				} else {
					return 0;
				}
				fflush( output_predict );
			}		
		}
	}
	//fflush( output_train );
	//fflush( output_predict );
}

