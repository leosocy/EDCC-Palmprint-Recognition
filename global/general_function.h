/*************************************************************************
	> File Name: general_function.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/15  18:37:13
 ************************************************************************/

#ifndef _GENERAL_FUNCTION_H
#define _GENERAL_FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <opencv2/opencv.hpp>
using namespace cv; 

#include "./global_definition.h"

void start_timing();
double stop_timing();

int rotate_image( const Mat &src, Mat &dst, double angle, double scale );


int roi_list_with_multispectral( const char *dir_path, const char *output_filename );
int roi_list_with_tongji( const char *dir_path, const char *output_filename );
int roi_list_with_polyu( const char *dir_path, const char *output_filename );
int roi_list_with_cameraimage( const char *dir_path, const char *output_filename );


int split_roi_list( const char *input_filename, const char *output_trainlist, const char *output_testlist, const int num_per_type, const int train_num );
int create_predict_list(  const char *all_list, const char *output_trainlist, const char *output_testlist, int perNum, int trainNum, int testNum );

#endif
