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

