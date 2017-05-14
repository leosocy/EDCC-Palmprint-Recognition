/*************************************************************************
	> File Name: PRMLKNearest.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/27 16:31:39
 ************************************************************************/

#include "../include/PRML.h"

PRMLKNearest::PRMLKNearest( int k )
{
	this->knn = new CvKNearest;
	this->genFunc = new GeneralFunctions;
	this->k = k;
	setMethodName( PRML_KNEAREST_METHOD_NAME );
}

PRMLKNearest::~PRMLKNearest()
{
	if( this->knn )	delete( this->knn );
	if( this->genFunc ) delete( this->genFunc );
	if( this->neighborResponses ) {
		delete( this->neighborResponses );
	}
	if( this->dists ) {
		delete( this->dists );
	}
}

void PRMLKNearest::setMethodName( const string &methodName )
{
	this->methodName = methodName;
}

string PRMLKNearest::getMethodName()
{
	return this->methodName;
}

bool PRMLKNearest::prTrain( const PRFeatures &trainFeatures )
{
	CV_Assert( this->k != 0 );
	printf( "KNearest Begin Train.\n" );
	Mat trainData, responses;
	genFunc->prfeaturesVec2Mat( trainFeatures, trainData, responses );
	return knn->train( trainData, responses, Mat(), false, this->k );
}

float PRMLKNearest::prPredict( const PRFeatures &predictFeatures, vector< int > &labels )
{
	CV_Assert( this->k != 0  );
	printf( "KNearest Begin Predict." );
	Mat predictData, results;
	genFunc->prfeaturesVec2Mat( predictFeatures, predictData, results );
	float ret = 0.0;
	this->neighborResponses = new Mat( predictData.rows, this->k, CV_32F );
	this->dists = new Mat( predictData.rows, this->k, CV_32F );
	ret = knn->find_nearest( predictData, this->k, results, *(this->neighborResponses), *(this->dists) );
	for( int r = 0; r < results.rows; ++r ) {
		labels.push_back( (int)results.at<float>( r, 0 ) );
	}

	return 0.0;
}

