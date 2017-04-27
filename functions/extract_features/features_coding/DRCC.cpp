/*************************************************************************
	> File Name: DRCC.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/06 22:12:08
 ************************************************************************/

#include "coding_algorithm.h"
#include "../image_transform/gabor.h"
#include "../image_enhance/image_enhance.h"


DRCC::DRCC()
{
	this->numOfScales = 3;
	this->numOfDirections = 9;
	this->imageSize = Size( 25, 25 );
	this->laplaceSize = 7;
	this->gaborDirections = 9;
	this->gaborKernelSize = Size( 7, 7 );
}

DRCC::~DRCC()
{

}

int DRCC::getMaxGaborResponse( const Mat &src, Mat &result, Mat &C, Mat &Cs, int numOfScales, int numOfDirections, int kernelType )
{
	assert( numOfScales >= 1 && numOfDirections >= 1 );
	GaborFilter filter;
	Mat batchResult;
	Mat gaborFilter;
	filter.numOfScales = numOfScales;
	filter.numOfDirections = numOfDirections;
	filter.kernelSize = this->gaborKernelSize;
	filter.doBatchGaborFilter( src, batchResult, kernelType, false );
	filter.showGaborFilter( gaborFilter, kernelType );
	//imshow( "gabor filter result", batchResult );
	//imshow( "gabor filter", gaborFilter );
	filter.doBatchGaborFilter( src, batchResult, kernelType, true );
	vector< Mat > mv;
	vector< Mat >::iterator iter;
	split( batchResult, mv );
	/*for( iter = mv.begin(); iter != mv.end(); ++iter ) {
		GaussianBlur( *iter, *iter, Size( 5, 5 ), 1, 0 );
	}
	merge( mv, batchResult );*/
	int height = src.rows;
	int width = src.cols;
	int Cleft = -1, Cright = -1;
	//int freq[6] = { 0 };
	result = Mat( src.size(), CV_64F );
	C = Mat( src.size(), CV_8S );
	Cs = Mat( src.size(), CV_8S );
	for( int i = 0; i < height; ++i ) {
		for( int j = 0; j < width; ++j ) {
			double maxResponse = -DBL_MAX;
			int maxScale = -1;
			int maxDirection = -1;
			for( int s = 0; s < numOfScales; ++s ) {
				for( int d = 0; d < numOfDirections; ++d ) {
					//printf( "Scale:%d  Direction:%d  Response:%lf Gray:%lf\n", s, d, mv[ s * numOfDirections + d ].at<double>( i, j ),  src.at<double>( i, j ) );
					if( mv[ s * numOfDirections + d ].at<double>( i ,j ) > maxResponse ) {
						maxResponse = mv[ s * numOfDirections + d ].at<double>( i, j );
						maxScale = s;
						maxDirection = d;
					}
				}
			}
			//printf( "index:%d maxScale:%d maxDirection:%d value:%lf\n", j, maxScale, maxDirection, DBL_MIN );
			result.at<double>( i, j ) = maxResponse;
	//		++freq[ maxDirection ];
			C.at<char>( i, j ) = maxDirection;
			if( maxDirection == numOfDirections - 1 ) {
				Cleft = 0;
			} else {
				Cleft = maxDirection + 1;
			}

			if( maxDirection == 0 ) {
				Cright = numOfDirections - 1;
			} else {
				Cright = maxDirection -1 ;
			}
			Cs.at<char>( i, j ) = mv[ maxScale * numOfDirections + Cleft ].at<double>( i, j ) >= mv[ maxScale * numOfDirections + Cright].at<double>( i, j )  ? 1 : 0; 
		}
	}
	/*for( int i = 0; i < 6; ++i ) {
		printf( "%d:%d\n", i, freq[i] );
	}*/
	return EXIT_SUCCESS;
}

int DRCC::doOnceDRCC( const Mat &src, int label )
{
	Mat maxResponseResult, C, Cs;
	this->numOfScales = 1;
	
	getMaxGaborResponse( src, maxResponseResult, C, Cs, this->numOfScales, this->gaborDirections, GaborFilter::GABOR_KERNEL_REAL );
	
#if 1
	/*for( int h = 0; h < maxResponseResult.rows; ++h ) {
		for( int w = 0; w < maxResponseResult.cols; ++w ) {
			maxResponseResult.at<double>( h, w ) = 1 - maxResponseResult.at<double>( h, w );
		}
	}*/
	imshow( "max response", maxResponseResult );
	this->CVector.push_back( C );
	this->CsVector.push_back( Cs );
	this->labels.push_back( label );
#endif
	return EXIT_SUCCESS; 
}

int DRCC::doBatchDRCC( const char *filename )
{
#if 0
	Mat image = imread( "/home/leosocy/Desktop/Inkedtan0.5_LI.jpg", CV_LOAD_IMAGE_COLOR );
	resize(image, image, Size( 125, 125 ) );
	/*vector< Mat > mv;
	split( image, mv );
	mv[0].at<uchar>( 75, 25 ) = 0;
	mv[1].at<uchar>( 75, 25 ) = 0;
	mv[2].at<uchar>( 75, 25 ) = 0;
	merge( mv, image );*/
	imshow( "origin", image );
	//Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//filter2D(image_gray, image_gray, CV_8UC1, kernel);*/ 
	/*Mat imageLog(image.size(), CV_32FC3);  
	for (int i = 0; i < image.rows; i++)  
	{  
		for (int j = 0; j < image.cols; j++)  
		{  
			imageLog.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);  
			imageLog.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);  
			imageLog.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);					}  
	}  
	normalize(imageLog, imageLog, 0, 1, CV_MINMAX);  */
	//convertScaleAbs(imageLog, imageLog);
	Mat image_gray;
	cvtColor( image, image_gray, CV_BGR2GRAY );
	/*Mat t;	
	image_gray.convertTo( t, CV_64FC1 );
	normalize( t, t, 0, 1, CV_MINMAX );	
	for( int h = 0; h < image_gray.rows; ++h ) {
		for( int w = 0; w < image_gray.cols; ++w ) {
			if( h - image_gray.rows / 2 == w ) {
				t.at<double>( h, w ) = 1;
			} else {
				t.at<double>( h, w ) = 0;
			}
		}
	}*/
	//imshow( "origin", t );
	doOnceDRCC( image_gray, "1" );	

	waitKey();
#endif

#if 1
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
	//printf( "--------------------------------Begin Coding Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );

		n = feature_file[num];
		if( n.empty() ) {
			//printf( "Begin Write Num:%d\n", i );
			feature_file.open( feature_string, FileStorage::APPEND );
			int id;
			char image_path[200];
			fscanf( roi_list, "%d %s", &id, image_path );
			Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			//Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize(image_gray, image_gray, this->imageSize );
			//Mat gaborResult;
			//GaborFilter filter;
			//filter.numOfDirections = 12;
			//filter.doBatchGaborFilter( image_gray, gaborResult, GaborFilter::GABOR_KERNEL_REAL );
			Mat t;	
			image_gray.convertTo( t, CV_64FC1 );
			normalize( t, t, 0, 1, CV_MINMAX );	
			/*for( int h = 0; h < image_gray.rows; ++h ) {
				for( int w = 0; w < image_gray.cols; ++w ) {
					t.at<double>( h, w ) = 1 - t.at<double>( h, w );
				}
			}*/
			
			ImageEnhance enhance;
			enhance.enhanceWithLaplace( t, image_gray, this->laplaceSize  );
			//imshow( "origin", image_gray );
			doOnceDRCC( image_gray, id );
			//printf( "End Write Num:%d\n\n", i );
			//waitKey();
		} else {
			
		}
	}
	fclose( roi_list );
	feature_file.release();
	clock_t et = clock();
	//printf( "--------------------------End Of Train----------------------------\n\n" );
	FILE *tuningResult = fopen( "./feature_info/TuningResult.txt", "a" );
	printf( "TrainEnd----->>>>>TrainCostTimeTotal:%lf\tTrainCostTimePer:%lf\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	fprintf( tuningResult, "TrainEnd----->>>>>TrainCostTimeTotal:%lf\tTrainCostTimePer:%lf\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	fflush( tuningResult );
	fflush( stdout );

	bt = clock();
	double score = 0.0;
	int right = 0;
	int index_1 = 0, index_2 = 0;
	Mat class_1 = Mat::zeros( Size( 1, LIST_TRAIN_NUM_TOTAL * ( LIST_TRAIN_NUM_PER - 1 ) ), CV_64FC1 );
	Mat class_2 = Mat::zeros( Size( 1, LIST_TRAIN_NUM_TOTAL * ( LIST_TRAIN_NUM_TOTAL - LIST_TRAIN_NUM_PER ) ), CV_64FC1 );
	for( int i = 0; i < CVector.size(); ++i ) {
		double maxScore = -DBL_MAX;
		int maxIndex = -1;
		for( int j = 0; j < CVector.size(); ++j ) {
			if( i != j ) {	
				score = matchingPoint2Point( this->CVector[i], this->CsVector[i], this->CVector[j], this->CsVector[j] );
				//score = matchingPoint2Area( this->CVector[i], this->CsVector[i], this->CVector[j], this->CsVector[j], Rect( -1, -1, 3, 3 ) );
				if( this->labels[i] == this->labels[j] ) {
					class_1.at<double>( 0, index_1++ ) = score;		
				} else {
					class_2.at<double>( 0, index_2++ ) = score;				
				}
				if( score > maxScore ) {
					maxScore = score;
					maxIndex = j;			
				}	
			}
		}	
		if( this->labels[i] == this->labels[maxIndex] ) {
			++right;		
		}
		//printf( "src1:%d  src2:%d  Score:%lf\n\n", this->labels[i], this->labels[maxIndex], maxScore );	
	}
	et = clock();
	//printf( "End of Matching.Total Cost Time: %lf  Per Image Cost Time: %lf  GAR:%lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL, (double)right / CsVector.size() );
	Mat mean1, mean2, convr1, convr2;
	meanStdDev( class_1, mean1, convr1 );
	meanStdDev( class_2, mean2, convr2 );
	double m1 = 0.0, m2 = 0.0, S1 = 0.0, S2 = 0.0;
	m1 = mean1.at<double>( 0, 0 );
	m2 = mean2.at<double>( 0, 0 );
	S1 = pow( convr1.at<double>( 0, 0 ), 2 );
	S2 = pow( convr2.at<double>( 0, 0 ), 2 );
	printf( "Parameters---->>>>ImageSize:(%d, %d)\tLaplaceSize:%d\tNumOfDirections:%d\tGaborKernelSize:(%d, %d)\n", this->imageSize.width, this->imageSize.height, this->laplaceSize, this->gaborDirections, this->gaborKernelSize.width, this->gaborKernelSize.height );
	printf( "Result------>>>>>>MatchingCostTimeTotal:%lf\tMatchingCostTimePer:%lf\tGAR:%lf\tm1:%lf\tm2:%lf\tS1:%lf\tS2:%lf\tJF:%lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL, (double)right / CsVector.size(), m1, m2, S1, S2, pow( fabs( m1 - m2 ), 2 ) / ( S1 + S2 ) );
	fprintf( tuningResult, "Parameters---->>>>ImageSize:(%d, %d)\tLaplaceSize:%d\tNumOfDirections:%d\tGaborKernelSize:(%d, %d)\n", this->imageSize.width, this->imageSize.height, this->laplaceSize, this->gaborDirections, this->gaborKernelSize.width, this->gaborKernelSize.height );
	fprintf( tuningResult, "Result------>>>>>>MatchingCostTimeTotal:%lf\tMatchingCostTimePer:%lf\tGAR:%lf\tm1:%lf\tm2:%lf\tS1:%lf\tS2:%lf\tJF:%lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL, (double)right / CsVector.size(), m1, m2, S1, S2, pow( fabs( m1 - m2 ), 2 ) / ( S1 + S2 ) );
	fflush( tuningResult );
	fclose( tuningResult );
	
#endif
	return EXIT_SUCCESS;
}

double DRCC::matchingPoint2Point( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy )
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
	/*double M1 = cv::norm( Cx );
	double M2 = cv::norm( Cy );
	double dist = Cx.dot( Cy ) / ( M1 * M2 );
	printf( "M1:%lf M2:%lf dist:%lf\n", M1, M2, dist );*/
	return score / ( 2 * width * height );
}

double DRCC::matchingPoint2Area( const Mat &Cx, const Mat &Csx, const Mat &Cy, const Mat &Csy, const Rect &areaRect )   //If areaRect( x, y, width, height ), Cx( i, j ) match Cy rect( i + x, j + y, width, height )
{
	assert( Cx.rows == Csx.rows && Cy.rows == Csy.rows && Cx.rows == Cy.rows && Cx.cols == Csx.cols && Cy.cols == Csy.cols && Cx.cols == Cy.cols && areaRect.width >= 0 && areaRect.height >= 0 );
	double score = 0.0;
	for( int i = 0; i < Cx.rows; ++i ) {
		for( int j = 0; j < Cx.cols; ++j ) {
			for( int m = areaRect.x; m < areaRect.width; ++m ) {
				for( int n = areaRect.y; n < areaRect.height; ++n ) {
					if( ( i + n  >= 0 ) && ( i + n  < Cy.rows ) && ( j + m  >= 0 ) && ( j + m < Cy.cols ) ) {
						if( Cx.at<char>( i, j ) == Cy.at<char>( i + n, j + m ) && ~( Csx.at<char>( i, j ) ^ Csy.at<char>( i + n, j + m ) ) ) {
							score += 2;
							m = areaRect.width;
							break;
						} else if( Cx.at<char>( i, j ) == Cy.at<char>( i + n, j + m ) ) {
							score += 1;
							m = areaRect.width;
							break;
						} else {
							continue;
						}
					}
				}
			}
		}
	}
	return score / ( 2 * Cx.rows * Cx.cols );
}

int DRCC::saveFeatures( const char *filename )
{
}

int DRCC::loadFeatures( const char *filename )
{
}


