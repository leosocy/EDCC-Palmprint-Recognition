/*************************************************************************
	> File Name: PRMLSVM.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/27 22:38:25
 ************************************************************************/
#include "../include/PRML.h"

PRMLSVM::PRMLSVM( const CvSVMParams &params )
{
	this->svm = new CvSVM;
	this->genFunc = new GeneralFunctions;
	this->params = new CvSVMParams;
	this->params->svm_type = params.svm_type;
	this->params->kernel_type = params.kernel_type;
	this->params->term_crit = params.term_crit;
	setMethodName( PRML_SVM_METHOD_NAME );
}

PRMLSVM::~PRMLSVM()
{
	if( svm ) delete( svm );
	if( genFunc ) delete( genFunc );
	if( params ) delete( params );
}

void PRMLSVM::setMethodName( const string &methodName )
{
	this->methodName = methodName;
}

string PRMLSVM::getMethodName()
{
	return this->methodName;
}

bool PRMLSVM::prTrain( const PRFeatures &trainFeatures )
{
	Mat trainData, responses;
	genFunc->prfeaturesVec2Mat( trainFeatures, trainData, responses );
	return svm->train( trainData, responses, Mat(), Mat(), *(this->params) );
}

float PRMLSVM::prPredict( const PRFeatures &predictFeatures, vector< int > &labels )
{
	Mat predictData, responses;
	genFunc->prfeaturesVec2Mat( predictFeatures, predictData, responses );

	svm->predict( predictData, responses );
	for( int r = 0; r < responses.rows; ++r ) {
		labels.push_back( (int)responses.at<float>( r, 0 ) );
	}

	return responses.at<float>( 0, 0 );
}


