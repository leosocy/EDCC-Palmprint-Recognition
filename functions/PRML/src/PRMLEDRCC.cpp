/*************************************************************************
	> File Name: PRMLEDRCC.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/28 17:45:06
 ************************************************************************/
#include "../include/PRML.h"
#include <map>

PRMLEDRCC::PRMLEDRCC( float scoreThreshold, int k )
{
	CV_Assert( k % 2 == 1 );
	this->scoreThreshold = scoreThreshold;
	this->k = k;
	this->genFunc = new GeneralFunctions;
	setMethodName( PRML_EDRCC_METHOD_NAME );
}

PRMLEDRCC::~PRMLEDRCC()
{
	if( this->genFunc ) delete( this->genFunc );
}

void PRMLEDRCC::setMethodName( const string &methodName )
{
	this->methodName = methodName;
}

string PRMLEDRCC::getMethodName()
{
	return this->methodName;
}

bool PRMLEDRCC::prTrain( const PRFeatures &trainFeatures )
{
	CV_Assert( trainFeatures.methodName == PRF_DRCC_METHOD_NAME );
	DRCC *d;
	d = (DRCC *)&trainFeatures;
	this->trainC = d->CVector;
	this->trainCs = d->CsVector;
	this->trainLabels = d->labels;
	return true;
}

float PRMLEDRCC::prPredict( const PRFeatures &predictFeatures, vector< int > &labels )
{
	CV_Assert( predictFeatures.methodName == PRF_DRCC_METHOD_NAME && predictFeatures.features.size() != 0 );
	DRCC *d;
	d = ( DRCC * )&predictFeatures;
	this->dists = Mat( d->CVector.size(), 1, CV_32F );
	for( size_t i = 0; i < d->CVector.size(); ++i ) {
		double maxScore = -DBL_MAX;
		int maxIndex = -1;
		vector< float > dists;
		for( size_t j = 0; j < this->trainC.size(); ++j ) {
			double score = d->matchingPoint2Point( d->CVector[i], d->CsVector[i], this->trainC[j], this->trainCs[j] );
			dists.push_back( score );
			if( score > maxScore ) {
				maxScore = score;
				maxIndex = j;
			}
		}
		vector< float > distsMaxK;
		vector< int > indexMaxK;
		getMaxTopK( dists, this->k, distsMaxK, indexMaxK );

		//printf( "src:%d neighbor:%d %d %d %d %d %d %d %d %d dists:%f %f %f %f %f %f %f %f %f\n", d->labels[i], this->trainLabels[indexMaxK[0]], this->trainLabels[indexMaxK[1]], this->trainLabels[indexMaxK[2]], this->trainLabels[indexMaxK[3]], this->trainLabels[indexMaxK[4]], this->trainLabels[indexMaxK[5]], this->trainLabels[indexMaxK[6]], this->trainLabels[indexMaxK[7]], this->trainLabels[indexMaxK[8]], distsMaxK[0], distsMaxK[1], distsMaxK[2], distsMaxK[3], distsMaxK[4], distsMaxK[5], distsMaxK[6], distsMaxK[7], distsMaxK[8] );
		printf( "src:%d", d->labels[i] );
		printf( " neighbor:" );
		for( int c = 0; c < this->k; ++c ) {
			printf( "%d ", this->trainLabels[indexMaxK[c]] );
		}
		printf( " dists:" );
		for( int c = 0; c < this->k; ++c ) {
			printf( "%f ", distsMaxK[c] );
		}
		printf( "\n" );
		map< int, char > freqMap;
		for( int c = 0; c < this->k; ++c ) {
			if( freqMap.find( this->trainLabels[indexMaxK[c]] ) == freqMap.end() ) {
				freqMap.insert( map< int, char >::value_type( this->trainLabels[indexMaxK[c]], 1 ) );	
			} else {
				freqMap.find( this->trainLabels[indexMaxK[c]] )->second += 1;
			}
		}

		map< int, char >::iterator it;
		int maxFreq = 0;
		int maxFreqI = -1;
		int c = 0;
		for( it = freqMap.begin(); it != freqMap.end(); ++it, ++c ) {
			if( it->second > maxFreq ) {
				maxFreq = it->second;
				maxFreqI = it->first;
			}
		}

		if( maxFreq < ( this->k - 1 ) / 2 ) {
			labels.push_back( -1 );
		} else {
			maxScore = -FLT_MAX;
			for( int c = 0; c < this->k; ++c ) {
				if( this->trainLabels[indexMaxK[c]] == maxFreqI ) {
					if( distsMaxK[c] > maxScore ) {
						maxScore = distsMaxK[c];
					}
				}
			}
			if( maxScore >= this->scoreThreshold ) {
				labels.push_back( maxFreqI );
			} else {
				labels.push_back( -1 );
			}
		}
		this->dists.at<float>( i, 0 ) = maxScore;
	}
	return this->dists.at<float>( 0, 0 );
}

void heapAdjust( float *dists, int *index, int left, int right )
{
	if( left >= right ) return;
	int r = left;
	int lChild = 2 * r + 1;
	int child = lChild;

	float tmpD = dists[r];
	int tmpI = index[r];
	while( lChild <= right ) {
		if( lChild < right && dists[lChild] > dists[lChild + 1] ) {
			child = lChild + 1;
		}
		if( tmpD > dists[child] ) {
			dists[r] = dists[child];
			index[r] = index[child];
			r = child;
			lChild = 2 * r + 1;
			child = lChild;
		} else {
			break;
		}
	}
	dists[r] = tmpD;
	index[r] = tmpI;
}

void PRMLEDRCC::getMaxTopK( const vector< float> &dists, int k, vector< float> &distsMaxK, vector< int > &indexMaxK )
{
	float *heap = ( float * )malloc( k * sizeof( float ) );
	int *index = ( int * )malloc( k * sizeof( int ) );

	memset( heap, 0, k * sizeof( float ) );
	memset( index, 0, k * sizeof( int ) );

	for( size_t i = 0; i < dists.size(); ++i ) {
		if( dists[i] > heap[0] ) {
			heap[0] = dists[i];
			index[0] = i;
			heapAdjust( heap, index, 0, k - 1 );
		}
	}

	for( int i = 0; i < k; ++i ) {
		distsMaxK.push_back( heap[i] );
		indexMaxK.push_back( index[i] );
	}

	heap = NULL;
	index = NULL;
	free( heap );
	free( index );
}
