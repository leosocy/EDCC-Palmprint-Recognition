/*************************************************************************
	> File Name: pca.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/12 21:32:09
 ************************************************************************/

#include "./pca.h"

void Do2DPCA( const vector< Mat > &palm_all, Mat &U_Result, double threshold, bool ATA, int index) //如果ATA=flase时,dst=scale*(I-delta)*(I-delta).t();
{
	
	assert( ( palm_all.size() != 0 ) && ( palm_all[0].channels() == 1 ) && ( threshold > 0 ) && ( threshold < 1 ) );
	Mat averagePalm = Mat::zeros( palm_all[0].size(), CV_64FC1 );
	for( size_t i = 0; i < palm_all.size(); ++i ) {
		Mat tmp;

		palm_all[i].convertTo( tmp, CV_64FC1 );
		averagePalm += tmp / ( palm_all.size() * 1.0 );
	}
	Mat CovG;
	if( ATA ) {
		CovG = Mat::zeros( palm_all[0].cols, palm_all[0].cols, CV_64FC1 );
	} else {
		CovG = Mat::zeros( palm_all[0].rows, palm_all[0].rows, CV_64FC1 );
	}
	for( size_t i = 0; i < palm_all.size(); ++i ) {
		Mat tmp;
		palm_all[i].convertTo( tmp, CV_64FC1 );
		Mat sub = tmp - averagePalm;
		Mat mul;
		mulTransposed( sub, mul, ATA );
		CovG += mul / ( palm_all.size() * 1.0  * ATA ? palm_all[0].rows : palm_all[0].cols );
	}
	printf( "DoPCA\n" );
	Mat U, UT, evals;
	SVD thissvd;
	thissvd.compute( CovG, evals, U, UT, SVD::FULL_UV );
	/*double sum = 0.0;
	for( int i = 0; i < evals.rows; ++i ) {
		sum += evals.at<double>( i, 0 );
	}
	double tmp = 0.0;
	for( int i = 0; i < evals.rows; ++i ) {
		tmp += evals.at<double>( i, 0 );
		if( tmp / sum > threshold ) {
			
			U_Result = U.colRange( 0, i );
			cout << "rows: " << U_Result.rows << "     Cols :" << U_Result.cols << endl;
			return;
		}
	}*/
	if( ATA )
		U_Result = U.colRange( 0, index );	//row
	else 	
		U_Result = U.colRange( 0, index );	//col
	cout << "rows: " << U_Result.rows << "     Cols :" << U_Result.cols << endl;
	
}

void DoBDPCA( const vector< Mat > &palm_all, Mat &U_Result, Mat &V_Result,  double threshold, int index_row, int index_col ) //类内均值的双投影   Yj = Vt * Aj * U
{
	Do2DPCA( palm_all, U_Result, threshold, true, index_row );
	vector< Mat > inte_image;
	for( size_t i = 0; i < palm_all.size(); ++i ) {
		Mat tmp;
		palm_all[i].convertTo( tmp, CV_64FC1 );
		inte_image.push_back( tmp * U_Result );
	}
	Do2DPCA( inte_image, V_Result, threshold, false, index_col );
}
