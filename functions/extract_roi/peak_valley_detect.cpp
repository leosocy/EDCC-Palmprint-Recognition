/*************************************************************************
	> File Name: peak_valley_detect.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/17  12:51:52
 ************************************************************************/

#include "peak_valley_detect.h"
#include <stdio.h>


extern Mat origin_rotate_image;

int left_x = INT_MAX, right_x = INT_MIN, top_y = INT_MAX, bottom_y = INT_MIN;	/*palm contour boundary*/

int peak_valley_detect( const Mat &src, vector< cv::Point> &peak, vector< cv::Point > &valley )
{
	vector < vector < cv::Point > > contours;
	vector < cv::Vec4i > hierarchy;
	findContours( src, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	drawContours( src, contours, 0, CV_COLOR_WHITE, CV_FILLED );
	if( contours.size() != 1 ) return EXIT_FAILURE;

	vector< cv::Point > palm_contour = contours[0];
	peak.push_back( palm_contour[0] );	/* middle finger */

	int pcs = palm_contour.size();
	int step = pcs / 20;


	left_x = INT_MAX, right_x = INT_MIN, top_y = INT_MAX, bottom_y = INT_MIN;
	for( int i = 0; i < pcs; i++ ) {
		if( palm_contour[i].x < left_x ) left_x = palm_contour[i].x;
		else if( palm_contour[i].x > right_x ) right_x = palm_contour[i].x;
		if( palm_contour[i].y < top_y ) top_y = palm_contour[i].y;
		else if( palm_contour[i].y > bottom_y ) bottom_y = palm_contour[i].y;
	}

	int second_finger_y = INT_MAX, fourth_finger_y = INT_MAX;
	int second_finger_index = -1, fourth_finger_index = -1;
	for( int i = step; i < pcs / 2; i++ ) {
		if( palm_contour[i].x < MIN( palm_contour[0].x, palm_contour[step].x ) ) {
			if( palm_contour[i].y < second_finger_y ) {
				second_finger_y = palm_contour[i].y;
				second_finger_index = i;
			}
		}
	}
	if( second_finger_index == -1 ) return EXIT_FAILURE;

	for( int i = pcs - step - 1; i > pcs / 2; i-- ) {
		if( palm_contour[i].x > MAX( palm_contour[0].x, palm_contour[pcs - step - 1].x ) ) {
			if( palm_contour[i].y < fourth_finger_y ) {
				fourth_finger_y  = palm_contour[i].y;
				fourth_finger_index = i;
			}
		}
	}
	if( fourth_finger_index == -1 || palm_contour[second_finger_index].x >= palm_contour[fourth_finger_index].x  ) return EXIT_FAILURE;

	peak.push_back( palm_contour[second_finger_index] );
	peak.push_back( palm_contour[fourth_finger_index] );

	int second_valley_y = INT_MIN, third_valley_y = INT_MIN;
	int second_valley_index = -1, third_valley_index = -1;
	for( int i = 0; i < second_finger_index; i++ ) {
		if( palm_contour[i].y > second_valley_y ) {
			second_valley_y = palm_contour[i].y;
			second_valley_index = i;
		}
	}
	if( second_valley_index == -1 ) return EXIT_FAILURE;

	for( int i = pcs - 1; i > fourth_finger_index; i-- ) {
		if( palm_contour[i].y > third_valley_y ) {
			third_valley_y = palm_contour[i].y;
			third_valley_index = i;
		}
	}

	int finger_length = src.rows / 10;	/* Approximate finger length to judge the peak-valley leagal */
	if( third_valley_index == -1 || palm_contour[second_valley_index].x >= palm_contour[third_valley_index].x || palm_contour[second_valley_index].y - peak[0].y < finger_length || palm_contour[third_valley_index].y - peak[0].y < finger_length || palm_contour[second_valley_index].y - peak[1].y < finger_length || palm_contour[third_valley_index].y - peak[2].y < finger_length ) return EXIT_FAILURE;

	valley.push_back( palm_contour[second_valley_index] );
	valley.push_back( palm_contour[third_valley_index] );

	for( int i = 0; i < 3; i++ ) {
		circle( origin_rotate_image, peak[i], 5, CV_RGB( 255, 0, 0 ), -1 );
	}
	for( int i = 0; i < 2; i ++ ) {
		circle( origin_rotate_image, valley[i], 5, CV_RGB( 0, 255, 0 ), -1 );
	}

	return EXIT_SUCCESS;
}
