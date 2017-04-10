/*************************************************************************
	> File Name: general_functions.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/10 20:35:48
 ************************************************************************/

#include "general_functions.h"

GeneralFunctions::GeneralFunctions()
{
}

GeneralFunctions::~GeneralFunctions()
{
}

int GeneralFunctions::blockImage( const Mat &src, vector< Mat > &blockingResult, Size &blockingSize )
{
	assert( src.rows % blockingSize.height == 0 && src.cols % blockingSize.width == 0 );
	for( int i = 0; i < src.rows; i += blockingSize.height ) {
		for( int j = 0; j < src.cols; j += blockingSize.width ) {
			Mat m( src, Rect( j , i, blockingSize.width, blockingSize.height ) );
			blockingResult.push_back( m.clone() );
			char name[5];
			sprintf( name, "%d_%d", ( i / blockingSize.height ),  ( j / blockingSize.width ) );
			string n( name );
			imshow( n, m );
		}
	}
	return EXIT_SUCCESS;
}

