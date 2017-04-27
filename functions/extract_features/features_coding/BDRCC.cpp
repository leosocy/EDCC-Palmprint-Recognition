/*************************************************************************
	> File Name: BDRCC.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/10 21:01:52
 ************************************************************************/

#include "coding_algorithm.h"
#include "../../../global/general_functions.h"
#include "../image_transform/gabor.h"

BDRCC::BDRCC()
{
	this->numOfScales = 1;
	this->numOfDirections = 12;
	this->blockingSize = Size( 5, 5 );
	this->imageSize = Size( 125, 125 );
}


BDRCC::~BDRCC()
{
}

int BDRCC::doOnceBDRCC( const Mat &src, int label )
{
	vector< Mat > blockingResult;
	GeneralFunctions func;
	func.blockImage( src, blockingResult, this->blockingSize );
	GaborFilter filter;
	filter.kernelSize = this->blockingSize;
	filter.numOfScales = this->numOfScales;
	filter.numOfDirections = this->numOfDirections;

	Mat gaborFilterResult;
	vector< Mat >::iterator iter;
	Mat C( src.rows / this->blockingSize.height, src.cols / this->blockingSize.width, CV_8S );
	Mat Cs( src.rows / this->blockingSize.height, src.cols / this->blockingSize.width, CV_8S );
	
	for( iter = blockingResult.begin(); iter != blockingResult.end(); ++iter ) {
		filter.doBatchGaborFilter( *iter, gaborFilterResult, GaborFilter::GABOR_KERNEL_REAL, true );
		vector< Mat > mv;
		split( gaborFilterResult, mv );
		vector< Mat >::iterator iter_mv;
		vector< double > imageResponse;
		for( iter_mv = mv.begin(); iter_mv != mv.end(); ++iter_mv ) {
			double response = 0.0;
			for( int r = 0; r < (*iter_mv).rows; ++r ) {
				for( int c = 0; c < (*iter_mv).cols; ++c ) {
					response += (*iter_mv).at<double>( r, c );
				}
			}
			imageResponse.push_back( response );
		}
		vector< double >::iterator response_iter;
		double maxResponse = DBL_MIN;
		int maxIndex = -1;
		for( response_iter = imageResponse.begin(); response_iter != imageResponse.end(); ++response_iter ) {
			if( *response_iter > maxResponse ) {
				maxResponse = *response_iter;
				maxIndex = response_iter - imageResponse.begin();
			}
		}
		int Cleft = -1, Cright = -1;
		if( maxIndex == this->numOfDirections - 1 ) {
			Cleft = 0;
		} else {
			Cleft = maxIndex - 1;
		}

		if( maxIndex == 0 ) {
			Cright = this->numOfDirections - 1;
		} else {
			Cright = maxIndex - 1;
		}
		C.at<char>( ( iter - blockingResult.begin() ) / C.rows, ( iter - blockingResult.begin() ) % C.rows ) = maxIndex;
	//	printf( "\n\nMaxResponse:%d\n", maxIndex );
		Cs.at<char>( ( iter - blockingResult.begin() ) / C.rows, ( iter - blockingResult.begin() ) % C.rows ) = ( imageResponse[Cleft] >= imageResponse[Cright] ? 1 : 0 );
	}

	this->CVector.push_back( C );
	this->CsVector.push_back( Cs );
	this->labels.push_back( label );

	return EXIT_SUCCESS;
}

int BDRCC::doBatchBDRCC( const char *filename )
{
	FILE *roi_list = fopen( filename, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", filename );
		return EXIT_FAILURE;
	}
	string feature_string = "feature_string_info/CODING_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< Mat >v_feature;

	clock_t bt = clock();
	printf( "--------------------------------Begin Coding Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );

		n = feature_file[num];
		if( n.empty() ) {
			printf( "Begin Write Num:%d\n", i );
			feature_file.open( feature_string, FileStorage::APPEND );
			int id;
			char image_path[200];
			fscanf( roi_list, "%d %s", &id, image_path );
			Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize( image_gray, image_gray, this->imageSize );
			//Mat gaborResult;
			//GaborFilter filter;
			//filter.numOfDirections = 12;
			//filter.doBatchGaborFilter( image_gray, gaborResult, GaborFilter::GABOR_KERNEL_REAL );
			Mat t;	
			image_gray.convertTo( t, CV_64FC1 );
			normalize( t, t, 0, 1, CV_MINMAX );	
			for( int h = 0; h < image_gray.rows; ++h ) {
				for( int w = 0; w < image_gray.cols; ++w ) {
					t.at<double>( h, w ) = 1 - t.at<double>( h, w );
				}
			}
			//imshow( "origin", t );
			doOnceBDRCC( image_gray, id );
			waitKey();
			printf( "End Write Num:%d\n\n", i );
		} else {
			
		}
	}
	feature_file.release();
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	
	fflush( stdout );
	double score = 0.0;
	int right = 0;
	for( int i = 0; i < CVector.size(); ++i ) {
		double maxScore = DBL_MIN;
		int maxIndex = -1;
		for( int j = 0; j < CVector.size(); ++j ) {
			if( i != j ) {
				score = matchingPoint2Point( this->CVector[i], this->CsVector[i], this->CVector[j], this->CsVector[j] );
				if( score > maxScore && i != j ) {
					maxScore = score;
					maxIndex = j;			
				}	
			}
		}	
		if( this->labels[i] == this->labels[maxIndex] ) {
			++right;		
		}
		printf( "src1:%d  src2:%d  Score:%lf\n\n", this->labels[i], this->labels[maxIndex], score );	
	}
	printf( "GAR:%lf", (double)right / CVector.size() );
	return EXIT_SUCCESS;
}
	
double BDRCC::matchingPoint2Point( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy )
{
	assert( Cx.rows == Csx.rows && Cy.rows == Csy.rows && Cx.rows == Cy.rows && Cx.cols == Csx.cols && Cy.cols == Csy.cols && Cx.cols == Cy.cols );
	int width = Cx.cols;
	int height = Cx.rows;
	
	double score = 0.0;
	
	for( int i = 0; i < height; ++i ) {
		for( int j = 0; j < width; ++j ) {
			score += ( Cx.at<char>( i, j ) == Cy.at<char>( i, j ) ) + ( ( Cx.at<char>( i, j ) == Cy.at<char>( i, j ) ) && ~( Csx.at<char>( i, j ) ^ Csy.at<char>( i, j ) ) ); 		
		}	
	}
	return score / ( 2 * width * height );
	return 0.0;
}

double BDRCC::matchingPoint2Area( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy, const Rect &areaRect )   //If areaRect( x, y, width, height ), Cx( i, j ) match Cy rect( i + x, j + y, width, height )
{
	assert( Cx.rows == Csx.rows && Cy.rows == Csy.rows && Cx.rows == Cy.rows && Cx.cols == Csx.cols && Cy.cols == Csy.cols && Cx.cols == Cy.cols && areaRect.width );
	return 0.0;
	
}

int BDRCC::saveFeatures( const char *filename )
{
	return EXIT_SUCCESS;
}
		
int BDRCC::loadFeatures( const char *filename )
{
	return EXIT_SUCCESS;
}
			
					
