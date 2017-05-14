/*************************************************************************
	> File Name: PRML.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/25 10:08:56
 ************************************************************************/
#ifndef __PRML_H__
#define __PRML_H__

#include "../../extract_features/features_base.h"
#include "../../../global/general_functions.h"
#include <vector>
#include <string>
using namespace std;

const string PRML_NBAYES_METHOD_NAME = "PRML_NBAYES";
const string PRML_KNEAREST_METHOD_NAME = "PRML_KNEAREST";
const string PRML_SVM_METHOD_NAME = "PRML_SVM";
const string PRML_EDRCC_METHOD_NAME = "PRML_EDRCC";

class PRML
{
public:
	PRML();
	virtual ~PRML();

	virtual bool prTrain( const PRFeatures &trainFeatures ) = 0;
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels ) = 0;
	
	virtual void setMethodName( const string &methodName ) = 0;
	virtual string getMethodName( void ) = 0;
	
	string methodName;
private:
	
};

class PRMLNBayes : public PRML
{
public:
	PRMLNBayes();
	virtual ~PRMLNBayes();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName( void );

private:
	CvNormalBayesClassifier *bayes;
	GeneralFunctions	*genFunc;
};

class PRMLKNearest : public PRML
{	
public:
	PRMLKNearest( int k );
	virtual ~PRMLKNearest();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName( void );
	
	int k;
	Mat *neighborResponses;
	Mat *dists;
private:
	CvKNearest		*knn;
	GeneralFunctions	*genFunc;
};

class PRMLSVM : public PRML
{
public:
	PRMLSVM( const CvSVMParams &params );
	virtual ~PRMLSVM();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName( void );
	
private:
	CvSVM			*svm;
	CvSVMParams 		*params;
	GeneralFunctions	*genFunc;	
};

class PRMLEDRCC : public PRML
{
public:
	PRMLEDRCC( float scoreThreshold, int k );
	virtual ~PRMLEDRCC();
	
	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );

	virtual void setMethodName( const string &methodName );
	virtual string getMethodName( void );
	
	float scoreThreshold;
	int k;
	Mat dists;
private:
	GeneralFunctions	*genFunc; 
	vector< Mat > trainC;
	vector< Mat > trainCs;
	vector< int > trainLabels;

	void getMaxTopK( const vector< float > &dists, int k, vector< float > &distsMaxK, vector< int > &indexMaxK );
};


#endif /* end of PRML.h */
