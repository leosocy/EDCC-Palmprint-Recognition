/*************************************************************************
	> File Name: adjust_palm.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/14  18:08:41
 ************************************************************************/

#include "adjust_palm.h"

extern Mat origin_image;
extern Mat origin_rotate_image;

double palm_adjust_angle; 

/**
 * remove small connected region just save the biggest region
 * */
int remove_small_area( const Mat &src, Mat &dst );

/**
 * fill internal contours which area is smaller than area_thre 
 */
int fill_contours( const Mat &src, Mat &dst, double area_thre );

/**
 * calculate the center of gravity
 */

int gravity_center( const Mat &src, cv::Point &center );

/**
 * find middle finger and rotate the image make the middle finger be top of the image.
 */

int find_middle_finger( const Mat &src, Mat &dst );

int adjust_palm( const Mat &src, Mat &dst )
{
	start_timing();
	fill_contours( src, dst, FILL_CONTOURS_AREA_THRE );
	remove_small_area( dst, dst );
	//circle( origin_image, center1, 5, CV_RGB(0, 255, 0), -1 );
	find_middle_finger( dst, dst );
	fill_contours( dst, dst, FILL_CONTOURS_AREA_THRE );
	remove_small_area( dst, dst );
#if _DEBUG_
	printf( "adjust cost time : %lf \n", stop_timing() );
	fflush( stdout );
#endif
	return EXIT_SUCCESS;
}

int fill_contours( const Mat &src, Mat &dst, double area_thre )
{
	clock_t start = clock();
	int g_nStructElementSize = 1;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * g_nStructElementSize + 1, 2 * g_nStructElementSize + 1 ), Point( g_nStructElementSize, g_nStructElementSize ));
//	morphologyEx( src, dst, MORPH_OPEN,element );

	double con_area;
	vector< vector< cv::Point > > contours;
	vector< cv::Vec4i > hierarchy;

	findContours( dst, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	drawContours( dst, contours, -1, CV_COLOR_WHITE, CV_FILLED );
	
	int index = 0;
	for( ; index >= 0; index = hierarchy[index][0] ) {
		con_area = fabs( contourArea( contours[index] ) );
		if( con_area < area_thre ) {
			drawContours( dst, contours, index, CV_COLOR_WHITE, CV_FILLED );
		}
	}
	
	morphologyEx( dst, dst, MORPH_CLOSE, element );
	//morphologyEx( dst, dst, MORPH_OPEN,element );
	//blur( dst, dst, Size( 3, 3 ) );

	return EXIT_SUCCESS;
}

int remove_small_area( const Mat &src, Mat &dst )
{
	double con_area = 0;
	double max_area = 0;
	int max_index;
	vector< vector< cv::Point > > contours;
	vector< cv::Vec4i > hierarchy;

	findContours( src, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	drawContours( dst, contours, -1, CV_COLOR_BLACK, CV_FILLED );

	int index = 0;
	for( ; index >= 0; index = hierarchy[index][0] ) {
		con_area = fabs( contourArea( contours[index] ) );
		if( con_area > max_area ) {
			max_area = con_area ;
			max_index = index;
		} 
	}
	drawContours( dst, contours, max_index, CV_COLOR_WHITE, CV_FILLED );

	return EXIT_SUCCESS;
}

int gravity_center( const Mat &src, cv::Point &center )
{
	Moments moment = moments( src, true );
	center.x = moment.m10 / moment.m00;
	center.y = moment.m01 / moment.m00;
	return EXIT_SUCCESS;
}

int find_middle_finger( const Mat &src, Mat &dst )
{
	Mat erode_img = src.clone();
	Point center1;	/* center1 is full palm center of gravity*/
	gravity_center( src, center1 );

	Mat element = getStructuringElement(MORPH_RECT, Size(2 * FINGER_ERODE_ELEMENT_SIZE + 1, 2 * FINGER_ERODE_ELEMENT_SIZE + 1 ), Point( FINGER_ERODE_ELEMENT_SIZE, FINGER_ERODE_ELEMENT_SIZE ) );
	erode( src, erode_img, element, cv::Point( -1, -1 ), 5 );	/* time consuming */
	
	Point center2;	/* center2 is palm which erode finger center of gravity */
	gravity_center( erode_img, center2 );
	
	Point v( center1.x - center2.x, -center1.y + center2.y );
	if( ( v.y == 0 && v.x == 0 ) || ( v.x > src.cols) || v.y > src.rows ) return EXIT_FAILURE;

	palm_adjust_angle = acos( (double)v.y / sqrt( v.x * v.x + v.y * v.y ) ) / CV_PI * 180.0;
	if( v.x < 0 ) palm_adjust_angle *= -1;
	//Mat dst;
	
	rotate_image( src, dst, palm_adjust_angle, 1.0 );
	rotate_image( origin_image, origin_rotate_image, palm_adjust_angle, 1.0 );
	//imshow( "Origin Rotate Image ", origin_rotate_image ); 
	return EXIT_SUCCESS;
}


