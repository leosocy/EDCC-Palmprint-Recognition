/*************************************************************************
	> File Name: PRMLNBayes.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/25 14:51:23
 ************************************************************************/
#include "../include/PRML.h"

PRMLNBayes::PRMLNBayes()
{
	this->bayes = new CvNormalBayesClassifier;
	this->genFunc = new GeneralFunctions;
}

PRMLNBayes::~PRMLNBayes()
{
	delete( this->bayes );
	delete( this->genFunc );
}

bool PRMLNBayes::prTrain( const PRFeatures &trainFeatures )
{
	Mat trainData, responses;
	genFunc->prfeaturesVec2Mat( trainFeatures, trainData, responses );
	return bayes->train( trainData, responses );
}

float PRMLNBayes::prPredict( const PRFeatures &predictFeatures, vector< int > &labels  )
{
	Mat predictData, responses;
	genFunc->prfeaturesVec2Mat( predictFeatures, predictData, responses );

	float ret = 0.0;
	ret = bayes->predict( predictData, &responses );
	for( int r = 0; r < responses.rows; ++r ) {
		labels.push_back( responses.at<int>( r, 0 ) );
	}
	return ret;
}
