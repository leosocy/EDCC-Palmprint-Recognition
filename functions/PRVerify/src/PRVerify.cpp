/*************************************************************************
	> File Name: PRVerify.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/28 16:08:24
 ************************************************************************/
#include "../include/PRVerify.h"

PRVerify::PRVerify()
{

}

PRVerify::~PRVerify()
{

}

void PRVerify::doPRPreprocess()
{

}

void PRVerify::doPRROI()
{

}

void PRVerify::doPRFeatures( PRFeatures *featuresMethod )
{
	CV_Assert( featuresMethod );
}

void PRVerify::doPRML( PRML *mlMethod )
{
	CV_Assert( mlMethod );

	mlMethod->prTrain( *(this->trainFeatures) );

	vector< int > labelsResult;
	mlMethod->prPredict( *(this->predictFeatures), labelsResult );

	PRMLEDRCC *edrcc = ( PRMLEDRCC *)mlMethod;
	double GAR = 0.0;
	for( size_t i = 0; i < labelsResult.size(); ++i ) {
		printf( "src:%d match:%d dist:%f\n", predictFeatures->labels[i], labelsResult[i], edrcc->dists.at<float>( i, 0 ) );
		if( predictFeatures->labels[i] == labelsResult[i]  ) {
			GAR += 1;
		}
	}

	printf( "Predict End.GAR:%lf", GAR / labelsResult.size() );
}

void PRVerify::doVerify( PRFeatures *featuresMethod, PRML *mlMethod )
{
	doPRFeatures( featuresMethod );
	doPRML( mlMethod );
}

