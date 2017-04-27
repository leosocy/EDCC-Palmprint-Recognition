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

int GeneralFunctions::blockImage( const Mat &src, vector< Mat > &blockingResult, const Size &blockingSize )
{
	assert( src.rows % blockingSize.height == 0 && src.cols % blockingSize.width == 0 );
	for( int i = 0; i < src.rows; i += blockingSize.height ) {
		for( int j = 0; j < src.cols; j += blockingSize.width ) {
			Mat m( src, Rect( j , i, blockingSize.width, blockingSize.height ) );
			blockingResult.push_back( m.clone() );
			char name[5];
			sprintf( name, "%d_%d", ( i / blockingSize.height ),  ( j / blockingSize.width ) );
			string n( name );
			//imshow( n, m );
		}
	}
	return EXIT_SUCCESS;
}

int GeneralFunctions::prfeaturesVec2Mat( const PRFeatures &fea, Mat &feaMat, Mat &labMat ) 
{
	CV_Assert( fea.features.size() == fea.labels.size() && fea.features.size() != 0 );

	feaMat = Mat( fea.features.size(), fea.features[0].rows * fea.features[0].cols, CV_32F ); 
	labMat = Mat( fea.labels.size(), 1, CV_32S ); 
	for( size_t i = 0; i < fea.features.size(); ++i ) {
		for( int r = 0; r < fea.features[i].rows; ++r ) {
			for( int c = 0; c < fea.features[i].cols; ++c ) {
				feaMat.at<float>( i, r * fea.features[i].cols + c ) = fea.features[i].at<float>( r, c );
			}
		}
		labMat.at<int>( i, 0 ) = fea.labels[i];
	}
	return EXIT_SUCCESS;
}

