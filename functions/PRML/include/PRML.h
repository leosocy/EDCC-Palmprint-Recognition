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
using namespace std;

class PRML
{
public:
	PRML();
	virtual ~PRML();

	virtual bool prTrain( const PRFeatures &trainFeatures ) = 0;
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels ) = 0;

private:

};

class PRMLNBayes : public PRML
{
public:
	PRMLNBayes();
	virtual ~PRMLNBayes();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );

private:
	CvNormalBayesClassifier *bayes;
	GeneralFunctions	*genFunc;
};

class PRMLKNearest : public PRML
{	
public:
	PRMLKNearest();
	virtual ~PRMLKNearest();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );
};

class PRMLSVM : public PRML
{
public:
	PRMLSVM();
	virtual ~PRMLSVM();

	virtual bool prTrain( const PRFeatures &trainFeatures );
	virtual float prPredict( const PRFeatures &predictFeatures, vector< int > &labels );
};


#endif /* end of PRML.h */
