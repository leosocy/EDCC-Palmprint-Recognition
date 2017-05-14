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
	setMethodName( PRML_NBAYES_METHOD_NAME );
}

PRMLNBayes::~PRMLNBayes()
{
	delete( this->bayes );
	delete( this->genFunc );
}

void PRMLNBayes::setMethodName( const string &methodName )
{
	this->methodName = methodName;
}

string PRMLNBayes::getMethodName()
{
	return this->methodName;
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
		labels.push_back( (int)responses.at<float>( r, 0 ) );
	}
	return ret;
}
