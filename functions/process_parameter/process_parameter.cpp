/*************************************************************************
	> File Name: process_parameter.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/02/15 18:49:44
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_parameter.h"

void print_usage( void )
{
	printf("Usage: ./main	[Train/Test/Predict]	[TrainListFileName/TestListFileName/PredictDataFileName] \n \
		[DatabaseImageWithRoi/DatabaseImage/CameraImage/Video\n");
}

void process( int argc, char **argv )
{
	if( argc < 4 ) {
		print_usage();
		exit( EXIT_FAILURE );
	}

	char *para1 = *( argv + 1 );
	char *para2 = *( argv + 2 );
	char *para3 = *( argv + 3 );
	FILE *input_stream = fopen( para2, "r" );
	if( strcmp( para1, "Train" ) == 0 ) {
		if( input_stream == NULL ) {
			printf( "%s is not exist.Please provide a right filename.", *( argv + 2 ) );			
			exit( EXIT_FAILURE );
		} else {
			if( strcmp( para3, "DatabaseImageWithRoi" ) == 0 ) {
					/*
					 *train( filename, 0 )
					 * */
			} else if( strcmp( para3, "DatabaseImage" ) == 0 ) {
					/*
					 * train( filename, 1 )
					 * */
			} else if( strcmp( para3, "CameraImage") == 0 ) {
					/*
					 * train( filename, 2 )
					 */
			} else if( strcmp( para3, "Video" ) == 0 ) {
					/*
					 *train( filename, 3 ) 
					 */
			} else {
					print_usage();
					exit( EXIT_FAILURE );
			}
		}
	} else if( strcmp( para1, "Test" ) == 0 ) {
		if( input_stream == NULL ) {
			printf( "%s is not exist.Please provide a right filename.", *( argv + 2 ) );			
			exit( EXIT_FAILURE );
		} else {
			if( strcmp( para3, "DatabaseImageWithRoi" ) == 0 ) {
					/*
					 *test( filename, 0 )
					 * */
			} else if( strcmp( para3, "DatabaseImage" ) == 0 ) {
					/*
					 * test( filename, 1 )
					 * */
			} else if( strcmp( para3, "CameraImage") == 0 ) {
					/*
					 * test( filename, 2 )
					 */
			} else if( strcmp( para3, "Video" ) == 0 ) {
					/*
					 *test( filename, 3 ) 
					 */
			} else {
					print_usage();
					exit( EXIT_FAILURE );
			}
		}
	} else if( strcmp( para1, "Predict" ) == 0 ) {
		if( input_stream == NULL ) {
			printf( "%s is not exist.Please provide a right filename.", *( argv + 2 ) );			
			exit( EXIT_FAILURE );
		} else {
			if( strcmp( para3, "DatabaseImageWithRoi" ) == 0 ) {
					/*
					 *predict( filename, 0 )
					 * */
			} else if( strcmp( para3, "DatabaseImage" ) == 0 ) {
					/*
					 * predict( filename, 1 )
					 * */
			} else if( strcmp( para3, "CameraImage") == 0 ) {
					/*
					 * predict( filename, 2 )
					 */
			} else if( strcmp( para3, "Video" ) == 0 ) {
					/*
					 *predict( filename, 3 ) 
					 */
			} else {
					print_usage();
					exit( EXIT_FAILURE );
			}
		}
	} else {
		print_usage();
		exit( EXIT_FAILURE );
	}
}
