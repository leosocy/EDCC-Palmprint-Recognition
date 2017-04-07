/*************************************************************************
	> File Name: DRCC.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/06 22:12:08
 ************************************************************************/

#include "coding_algorithm.h"
#include "../image_transform/gabor.h"

DRCC::DRCC()
{
	this->numOfScales = 5;
	this->numOfDirections = 8;
}

DRCC::~DRCC()
{

}

int DRCC::getMaxGaborResponse( const Mat &src, Mat &result, int numOfScales, int numOfDirections, int kernelType )
{
	assert( numOfScales >= 1 && numOfDirections >= 1 );
	GaborFilter filter;
	Mat batchResult;
	filter.numOfScales = numOfScales;
	filter.numOfDirections = numOfDirections;
	filter.doBatchGaborFilter( src, batchResult, kernelType );
	imshow( "gabor filter", batchResult );
	int height = src.rows;
	int width = src.cols;
	result = Mat( src.size(), CV_64F );
	for( int i = 0; i < height; ++i ) {
		for( int j = 0; j < width; ++j ) {
			double maxResponse = DBL_MAX;
			for( int s = 0; s < numOfScales; ++s ) {
				for( int d = 0; d < numOfDirections; ++d ) {
					if( batchResult.at<double>( i + s * height, j + d * width ) < maxResponse ) {
						maxResponse = batchResult.at<double>( i + s * height, j + d * width  );
					}
				}
			}
			result.at<double>( i, j ) = maxResponse;	
		}
	}
	return EXIT_SUCCESS;
}

int DRCC::doOnceDRCC( const Mat &src, const string &label )
{
	Mat maxResponseResult;
	
	getMaxGaborResponse( src, maxResponseResult, this->numOfScales, this->numOfDirections, GaborFilter::GABOR_KERNEL_REAL );

	imshow( "max response", maxResponseResult );
	return EXIT_SUCCESS;
}

int DRCC::doBatchDRCC( const char *filename )
{
}

int DRCC::saveFeatures( const char *filename )
{
}

int DRCC::loadFeatures( const char *filename )
{
}


