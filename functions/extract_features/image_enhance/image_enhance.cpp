/*************************************************************************
	> File Name: image_enhancement.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/15 22:33:41
 ************************************************************************/

#include "image_enhance.h"

ImageEnhance::ImageEnhance()
{
}

ImageEnhance::~ImageEnhance()
{
}

int ImageEnhance::enhanceWithHistogram( const Mat &src, Mat &dst )
{
	CV_Assert( src.type() == CV_8UC3 );
	vector< Mat > mv;
	split( src, mv );
	vector< Mat >::iterator iter;
	for( iter = mv.begin(); iter != mv.end(); ++iter ) {
		equalizeHist( *iter, *iter );	
	}
	merge( mv, dst );
	//normalize( dst, dst, 0, 1, CV_MINMAX );
	return EXIT_SUCCESS;
}

int ImageEnhance::enhanceWithLaplace( const Mat &src, Mat &dst, int aperture_size )
{
	//CV_Assert( src.type() == CV_64FC1 );
	//Mat kernel = ( Mat_<float>( 3, 3 ) << 0, -1, 0, -1, 5, -1, 0, -1, 0 );
	//filter2D( src, dst, src.type(), kernel );
	Mat gaussian;
	GaussianBlur( src, gaussian, Size(3,3), 0, 0, BORDER_DEFAULT );
	Laplacian( gaussian, dst, CV_64F, aperture_size );
	normalize( dst, dst, 0, 255, CV_MINMAX );
	return EXIT_SUCCESS;
}

int ImageEnhance::enhanceWithLog( const Mat &src, Mat &dst, int v )
{
	CV_Assert( src.type() == CV_64FC1 );
	dst = Mat( src.size(), src.type() );
	for( int i = 0; i < src.rows; ++i ) {
		for( int j = 0; j < src.cols; ++j ) {
			dst.at<double>( i, j ) = log( 1 + v * src.at<double>( i, j )) / log( v + 1 );
		}
	}
	normalize( dst, dst, 0, 1, CV_MINMAX );
	return EXIT_SUCCESS;
}

int ImageEnhance::enhanceWithGamma( const Mat &src, Mat &dst, double gamma )
{
	CV_Assert( src.type() == CV_64FC1 );
	
	dst = Mat( src.size(), src.type() );
	for( int i = 0; i < src.rows; ++i ) {
		for( int j = 0; j < src.cols; ++j ) {
			dst.at<double>( i, j ) = pow( src.at<double>( i, j ), gamma );
		}
	}
	normalize( dst, dst, 0, 1, CV_MINMAX );
	return EXIT_SUCCESS;
}
