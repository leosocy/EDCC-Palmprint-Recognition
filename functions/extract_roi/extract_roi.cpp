/*************************************************************************
	> File Name: extract_roi.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/21  13:39:20
 ************************************************************************/
#include <stdlib.h>
#include "extract_roi.h"

#define CIRCLE_FINGER_PROPORTION 0.13	/*Use to position the maximum inscribed circle up to the palm*/
#define CIRCLE_WIDTH_PROPORTION 0.01	/*Use to reduce slightly the maximum inscribed circle*/
#define CIRCLE_RADIUS_WIDTH_PROPORTION 0.22	/*Use to judge wthether the circle compliance with standards*/

extern int peak_valley_detect( const Mat &src, vector< cv::Point > &peak, vector< cv::Point > &valley );
extern Mat origin_rotate_image;
extern Mat origin_image_without_scale;
extern Mat origin_image;
extern int left_x, right_x, top_y, bottom_y;
extern double image_scale;
extern double palm_adjust_angle;

int get_point_affined_pos( const Size &originSize, const Size &newSize, const cv::Point &src, const cv::Point rotate_center, cv::Point &dst, double angle, double scale );
double point_dist( const cv::Point &p1, const cv::Point &p2 ); 

int extract_roi( const Mat &src, Mat &roi )
{
	vector< cv::Point > peak, valley;
	if( peak_valley_detect( src, peak, valley ) == EXIT_FAILURE ) return EXIT_FAILURE;
	Rect s_r_rect( left_x, top_y, ( right_x - left_x - 1 ), ( bottom_y - top_y - 1 ) );
	Mat s_r;
	s_r = src( s_r_rect ).clone();

	vector < vector < cv::Point > > contours;
	vector < cv::Vec4i > hierarchy;
	findContours( s_r, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	drawContours( s_r, contours, 0, CV_COLOR_WHITE, CV_FILLED );
	vector< cv::Point > contour = contours[0];
	/* distanceTransform compute palm max effictive */
	Mat dist_img;
	distanceTransform( s_r, dist_img, CV_DIST_L2, 5 );
	int temp = 0, R = 0, px = 0, py = 0;
	int left = peak[1].x - left_x, right = MIN( s_r.cols, 2 * ( peak[2].x - left_x ) - ( peak[0].x - left_x ) ), top = valley[0].y - top_y, bottom = MIN( s_r.rows, 2 * ( valley[0].y - top_y ) - ( peak[0].y - top_y ) );

	for( int i = top; i < bottom; i++ ) {
		for( int j = left; j < right; j++ ) {
			temp = (int)dist_img.ptr<float>(i)[j];
			/*int k = 0;
			for( ; k < contour.size() / 5; k++ ) {
				if( point_dist( contour[k], Point( j, i ) ) < temp ) break;
			} 
			if( k < contour.size() / 3 ) continue;*/
			if( temp > R ) {
				if( (double)( i - temp - valley[1].y + top_y ) / ( valley[1].y - peak[0].y ) < CIRCLE_FINGER_PROPORTION ) {
				R = temp;
				px = j;
				py = i;
				}
			/*	if( array_size < CIRCLE_VECTOR_SIZE ) {
					int k = 0;
					for( ; k < array_size; k++ ) {
						if( circle_pointer[k].radius < R ) {
							for( int l = k; l < array_size; l++ ) {
								circle_pointer[l + 1].radius = circle_pointer[l].radius;
								circle_pointer[l + 1].center = circle_pointer[l].center;
							}
							break;
						}
					}
					circle_pointer[k].radius = R;
					circle_pointer[k].center = Point( px, py );
					array_size++;
				} else {
					int t;
					for(  t = array_size - 1; t >= 0; t-- ) {
						if( circle_pointer[t].radius > R ) break;
					}
					for( int k = t + 1; k < array_size - 1; k++ ) {
							circle_pointer[k + 1].radius = circle_pointer[k].radius;
							circle_pointer[k + 1].center = circle_pointer[k].center;
					}
					if( t != array_size - 1 ) {
						circle_pointer[t + 1].radius = R;
						circle_pointer[t + 1].center = Point( px, py );
					}
				}*/
			}
		}
	}
//	Point v( valley[1].x - px, -valley[1].y + py );
//	double angle = acos( (double)v.y / sqrt( v.x * v.x + v.y * v.y ) ) / CV_PI * 180.0;
//	if( v.x < 0 ) angle *= -1;

//	rotate_image( origin_rotate_image, origin_rotate_image, angle, 1.0 );
//	Point basePoint, center;
//	get_point_affined_pos( src.size(), origin_rotate_image.size(), valley[1], Point( src.cols / 2, src.rows / 2 ), basePoint, angle, 1.0 );
//	get_point_affined_pos( src.size(), origin_rotate_image.size(), Point( px, py ), Point( src.cols / 2, src.rows / 2 ), center, angle, 1.0 );



	/* Normalizes the postion of the inscribd circle */
	if( (double)R / ( right_x - left_x ) < CIRCLE_RADIUS_WIDTH_PROPORTION ) return EXIT_FAILURE;
	R -= ( right_x - left_x ) * CIRCLE_WIDTH_PROPORTION;
	Point basePoint( valley[1].x - left_x, valley[1].y - top_y );
	Point center( px, py );
	Point sub( basePoint.x - center.x, -basePoint.y + center.y );
	circle( s_r, center, R, CV_COLOR_BLACK, 1 );
	circle( s_r, center, 3, CV_COLOR_BLACK, -1 );
	printf( "Distance : %d    %lf\n", right_x - left_x, (double)R / ( right_x - left_x ) );
	imshow( "Binary_Roate", s_r );
	double radian = acos( (double)sub.y / sqrt( sub.x * sub.x + sub.y * sub.y ) );
	if( sub.x < 0 ) radian *= -1;
	/* reset circle center */
	//center.x = basePoint.x - ( R + DIST_BETWEEN_CENTER_VALLEY ) * sin( radian );
	//center.y = basePoint.y + ( R + DIST_BETWEEN_CENTER_VALLEY ) * cos( radian );
	circle( origin_rotate_image, Point( center.x + left_x, center.y + top_y ), R, CV_COLOR_BLACK, 1 );

	imshow("Origin_Rotate", origin_rotate_image);
	sub.x = basePoint.x - center.x;
	sub.y = basePoint.y - center.y;
	if( sqrt( ( (double)valley[0].x - left_x - center.x ) * ( valley[0].x - left_x - center.x ) + ( valley[0].y - top_y - center.y ) * ( valley[0].y - top_y - center.y ) ) < R ) return EXIT_FAILURE;

	/* oBasePoint oCenter oR correspond origin_image_without_scale */
	Point oBasePoint, oCenter;
	double oR = R * 1 / image_scale;
	get_point_affined_pos( src.size(), origin_image_without_scale.size(), Point( center.x + left_x, center.y + top_y ), Point( src.cols / 2, src.rows / 2 ), oCenter, -palm_adjust_angle, 1 / image_scale );
	get_point_affined_pos( src.size(), origin_image_without_scale.size(), Point( basePoint.x + left_x, basePoint.y + top_y ), Point( src.cols / 2, src.rows / 2 ), oBasePoint, -palm_adjust_angle, 1 / image_scale );
	if( oCenter.x - oR < 0 || oCenter.x + oR > origin_image_without_scale.cols || oCenter.y - oR < 0 || oCenter.y + oR > origin_image_without_scale.rows ) return EXIT_FAILURE; 
	Rect roi_rect( oCenter.x - oR, oCenter.y - oR, 2 * oR, 2 * oR );
	Mat roi_rect_mat = Mat( origin_image_without_scale, roi_rect );

	Point roi_sub( oBasePoint.x - oCenter.x, -oBasePoint.y + oCenter.y );
	double roi_angle = acos( (double)roi_sub.y / sqrt( (double)roi_sub.x * roi_sub.x + roi_sub.y * roi_sub.y ) ) / CV_PI * 180.0;
	Mat roi_rotate;
	rotate_image( roi_rect_mat, roi_rotate, roi_angle, 1.0 );
//	Rect rect( roi_rotate.cols / 2 - 0.707 * oR, roi_rotate.rows / 2 - 0.707 * oR, 1.414 * oR, 1.414 * oR );
//	roi = Mat( roi_rotate, rect );
//	imshow("roi", roi );
#if 1
//	int length = cvRound( sqrt( (double)roi_rect_mat.rows * roi_rect_mat.rows + roi_rect_mat.cols * roi_rect_mat.cols ) );
	Point roi_rect_center( roi_rotate.cols / 2, roi_rotate.rows / 2 );
	Mat mask = Mat::zeros( roi_rotate.size(), CV_8UC1 );
	circle( mask, roi_rect_center, oR, Scalar( 255 ) );
	floodFill( mask, roi_rect_center, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE );

	Mat maskImage;
	roi_rotate.copyTo( maskImage, mask );

	Rect rect( roi_rect_center.x - oR, roi_rect_center.y - oR, 2 * oR, 2 * oR );
	roi = Mat( maskImage, rect );
//	rectangle( roi, Point( roi.cols / 2 - 0.707 * oR, roi.rows / 2 - 0.707 * oR ), Point( roi.cols / 2 + 0.707 * oR, roi.rows / 2 + 0.707 * oR ), CV_COLOR_BLACK, 1 );
	imshow( "roi", roi );
	/*for( int i = 0; i < roi.rows; i++ ) {
		for( int j = 0; j < roi.cols; j++ ) {
			printf( "%d ", roi.at< cv::Vec3b >( j, i )[0] );
		}
		printf( "\n" );
	}*/
#endif
	
	return EXIT_SUCCESS;
}


int get_point_affined_pos( const Size &originSize, const Size &newSize, const cv::Point &src, const cv::Point rotate_center, cv::Point &dst, double angle, double scale )
{
	double radian = angle / 180.0 * CV_PI;
	int x = src.x - rotate_center.x;
	int y = src.y - rotate_center.y;

	dst.x = cvRound( x * cos( radian ) * scale + y * sin( radian ) * scale + rotate_center.x );
	dst.y = cvRound( -x * sin( radian ) * scale + y * cos( radian ) * scale + rotate_center.y );

	int movx = cvRound( ( newSize.width - originSize.width ) / 2 );
	int movy = cvRound( ( newSize.height - originSize.height ) / 2 );

	dst.x += movx;
	dst.y += movy;
	return EXIT_SUCCESS;
}


double point_dist( const cv::Point &p1, const cv::Point &p2 )
{
	return sqrt( (double)( p1.x - p2.x ) * ( p1.x * p2.x ) + ( p1.y - p2.y ) *  ( p1.y - p2.y ) );
}
