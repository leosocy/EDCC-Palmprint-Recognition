/*************************************************************************
	> File Name: extract_features_with_statisic.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/04 21:49:45
 ************************************************************************/

#include "extract_features_with_statisic.h"
#include "../image_transform/dft.h"
#include "../image_transform/wavelet.h"

void get_features( const Mat &src, const string &type, Mat &features, const Size block, const int num );
void statistic_info( Mat &src, const string &type,  double &info1, double &info2 );	//Get statistical information.Type can be AM( Ampltitude Phase ) or MV( Mean Variance).
void calc_dis( const features_statistic_struct &test, const vector< features_statistic_struct > &v_feature, int &correct );
void calc_dis( const features_statistic_struct &test, const vector< features_statistic_struct > &v_feature, int &correct, int &fr, int &fa );
void calc_eer( const features_statistic_struct &src1, const features_statistic_struct &src2, int &correct, int &fr, int &fa, double threshold );

int train_statistic( const char *trainList )
{
	FILE *roi_list = fopen( trainList, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", trainList );
		return EXIT_FAILURE;
	}
	
	string feature_string = "feature_info/WDT_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< Mat >v_feature;

	clock_t bt = clock();
	printf( "--------------------------------Begin Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );

		n = feature_file[num];
		if( n.empty() ) {
			printf( "Begin Write Num:%d\n", i );
			feature_file.open( feature_string, FileStorage::APPEND );
			int id = 0;
			char image_path[200];
			fscanf( roi_list, "%d %s", &id, image_path );
			Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
			Mat image_gray;
			cvtColor( image, image_gray, CV_BGR2GRAY );
			double proportion = (double)image.cols / image.rows;
			Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
			resize(image_gray, image_gray, dsize);
			/*************************extract features with WDT********************************/
			WDT( image_gray, image_gray, "haar", WAVELET_LEVEL );
			Mat features;
			Size block( WAVELET_BLOCK_SIZE, WAVELET_BLOCK_SIZE );
			get_features( image_gray, FEATURES_STATISTIC_TYPE, features, block, i );

			//v_feature.push_back( features );
			feature_file << s_num;
			feature_file << "{";
			feature_file << "id" << id;
			feature_file << "imagepath" << image_path;
			feature_file << "features" << features;
			feature_file << "}";
			printf( "End Write Num:%d\n\n", i );
		} else {
			
		}
	}
	feature_file.release();
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	
	fflush( stdout );
	return EXIT_SUCCESS;
}

WBS::WBS()
{
}

WBS::~WBS()
{
}

int WBS::doExtractFeatures( const char *filename )
{
	FILE *roi_list = fopen( filename, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", filename );
		return EXIT_FAILURE;
	}
	vector< Mat >v_feature;
	clock_t bt = clock();
	printf( "--------------------------------Begin Train--------------------\n" );

	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {
		printf( "Begin Write Num:%d\n", i );
		int id = 0;
		char image_path[200];
		fscanf( roi_list, "%d %s", &id, image_path );
		Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
		resize(image_gray, image_gray, dsize);
		/************************extract features with WDT********************************/
		WDT( image_gray, image_gray, "haar", this->waveletLevel );
		Mat features;
		get_features( image_gray, FEATURES_STATISTIC_TYPE, features, this->blockSize, i );

		this->features.push_back( features );
		this->labels.push_back( id );
			
		printf( "End Write Num:%d\n\n", i );
	}
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );
	
	fflush( stdout );
	return EXIT_SUCCESS;
}

void WBS::doVerification( int dataSize )
{
	FILE *matchFile = fopen( "./feature_info/Multispectral_B_MatchScore_WBS.txt", "w" );
	FILE *tuningFile = fopen( "./feature_info/Tuning_WBS.txt", "a" );
	clock_t bt = clock(), et;
	int gen[71] = { 0 };
	int imp[71] = { 0 };
	double score = 0.0;
	int GAR = 0, FAR = 0, FRR = 0;
	Mat scoreMat = Mat::zeros( Size( dataSize, dataSize ), CV_64FC1 );
	for( int i = 0; i < (this->features).size(); ++i ) {
		for( int j = i + 1; j < (this->features).size(); ++j ) {	
			score = matchingPoint2Point( this->features[i], this->features[j] );
			scoreMat.at<double>( i, j ) = score;
			scoreMat.at<double>( j, i ) = score;
			printf( "Cacl ---- src1:%d  src2:%d  Score:%lf\n\n", this->labels[i], this->labels[j], score );	
			int classType = 1;
			int index = score / 0.001 - 930;
			if( this->labels[i] == this->labels[j] ) { 
				classType = 0;
				gen[index] += 1;			
			} else {
				imp[index] += 1;			
			}
			//fprintf( matchFile, "%d %d %d %lf\n", i, j , classType, score );
		}	
	}
	for( int i = 0; i < 71; ++i ) {
		fprintf( matchFile, "%.3f %lf %lf\n", i * 0.001 + 0.93, 100 * (double)gen[i] / ( LIST_TRAIN_NUM_TOTAL * ( LIST_TRAIN_NUM_PER - 1 ) ) * 2,  100 * (double)imp[i] / ( LIST_TRAIN_NUM_TOTAL * ( LIST_TRAIN_NUM_TOTAL - LIST_TRAIN_NUM_PER ) ) * 2 );	
	}
	fclose( matchFile );
	for( int i = 0; i < scoreMat.rows; ++i ) {
		double maxScore = -DBL_MAX;
		int maxIndex = -1;
		for( int j = 0; j < scoreMat.cols; ++j ) {
			if( scoreMat.at<double>( i, j ) > maxScore ) {
				maxScore = scoreMat.at<double>( i, j );
				maxIndex = j;			
			}	
		}	
		if( this->labels[i] == this->labels[maxIndex] ) {
			++GAR;		
		}
		printf( "src1:%d  src2:%d  Score:%lf\n\n", this->labels[i], this->labels[maxIndex], maxScore );	
	}
	et = clock();
	//fprintf( tuningFile, "%d %d %lf", this->waveletLevel, this->blockSize.width, (double)GAR / this->features.size() );
	fclose( tuningFile );
	printf( "End of Matching.Total Cost Time: %lf  Per Image Cost Time: %lf  GAR:%lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL, (double)GAR / this->features.size() );
}


double WBS::matchingPoint2Point( const Mat &X, const Mat &Y )
{
	assert( X.rows == Y.rows && X.cols == Y.cols );
	double M1 = cv::norm( X );
	double M2 = cv::norm( Y );
	double dist = X.dot ( Y ) / ( M1 * M2 );
	return dist;
}

int test_statistic( const char *testList )
{
	FILE *roi_list = fopen( testList, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", testList );
		return EXIT_FAILURE;
	}
	
	string feature_string = "feature_info/WDT_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< features_statistic_struct >v_feature;

	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL; ++i ) {
		features_statistic_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		features_statistic f;
		n = feature_file[num];
		n["id"] >> fs.id;
		n["imagepath"] >> fs.imagepath;
		n["features"] >> fs.features;
		v_feature.push_back( fs );
	}
	feature_file.release();
	int correct = 0;
	clock_t bt = clock();
	printf( "----------------Begin Test----------------------------\n" );
	for( int i = 0; i < LIST_TEST_NUM_TOTAL; i++ ) {

		features_statistic_struct fs;
		printf( "Begin Caclulate Num:%d\n", i );
		int id = 0;
		char image_path[200];
		fscanf( roi_list, "%d %s", &id, image_path );
		fs.id = id;
		fs.imagepath = image_path;
		Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
		resize(image_gray, image_gray, dsize);
		/*************************extract features with WDT********************************/
		WDT( image_gray, image_gray, "haar", WAVELET_LEVEL );
		Mat features;
		Size block( WAVELET_BLOCK_SIZE, WAVELET_BLOCK_SIZE );
		get_features( image_gray, FEATURES_STATISTIC_TYPE, features, block, i );

		fs.features = features.clone();
			//v_feature.push_back( features );
		calc_dis( fs, v_feature, correct );
		printf( "End Caculate Num:%d\n\n", i );
	}
	printf( "--------------------------End Of Cacul----------------------------\n\n" );
	clock_t et = clock();
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TEST_NUM_TOTAL );
	printf( "\n\n-------Result : GAR : %lf WrongNum: %d\n", (double)correct / LIST_TEST_NUM_TOTAL, LIST_TEST_NUM_TOTAL - correct );
	fflush( stdout );
	return EXIT_SUCCESS;
}

int calcu_eer_threshold_statistic( const char *calculist )
{
	FILE *calcu_list = fopen( calculist, "r" );
	if( calcu_list == NULL ) {
		printf( "%s Not Exist.\n", calculist );
		return EXIT_FAILURE;
	}
	#undef LIST_TRAIN_NUM_TOTAL
	#define LIST_TRAIN_NUM_TOTAL CALCU_EER_NUMBER_OF_IMAGE
	train_statistic( calculist );
	getchar();
	string feature_string = "feature_info/WDT_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< features_statistic_struct >v_feature;
	for( int i = 0; i < CALCU_EER_NUMBER_OF_IMAGE; ++i ) {
		features_statistic_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		features_statistic f;
		n = feature_file[num];
		n["id"] >> fs.id;
		n["imagepath"] >> fs.imagepath;
		n["features"] >> fs.features;
		v_feature.push_back( fs );
	}

	feature_file.release();
	
	double threshold = 0.9830;
	double min_eer = 1.0;
	double min_eer_threshold = 0.0;
	printf( "----------------EER Begin Calcu----------------------------\n" );
	for( ; threshold <= 0.9830; threshold += 0.0001 ) {
		int correct = 0;
		int fr = 0;
		int fa = 0;
		clock_t bt = clock();
		for( size_t i = 0; i < v_feature.size(); ++i ) {
			for( size_t j = 0; j < v_feature.size(); ++j ) {
				if( i != j ) {
					calc_eer( v_feature[i], v_feature[j], correct, fr, fa, threshold );			
				}		
			}	
		}
		clock_t et = clock();
		double frr = (double)fr / ( CALCU_EER_NUMBER_OF_IMAGE *  ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - 1 ) );
		double far = (double)fa / ( CALCU_EER_NUMBER_OF_IMAGE * ( CALCU_EER_NUMBER_OF_IMAGE - ALL_PER_PEOPLE_NUMBER_OF_IMAGE ) );
		if( min_eer > fabs( frr - far ) ) {
			min_eer = fabs( frr - far );
			min_eer_threshold = threshold;		
		}
		printf( "G : %d\n", correct );
		printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / CALCU_EER_NUMBER_OF_IMAGE );
		printf( "\n\n-------Result : GAR : %lf FRR : %lf  FAR : %lf Threshold : %lf EER : %lf\n", (double)correct / ( CALCU_EER_NUMBER_OF_IMAGE * ( CALCU_EER_NUMBER_OF_IMAGE - 1 ) ), frr, far, threshold, fabs( frr - far ) );
		fflush( stdout );
		printf( "--------------------------EER End Of Cacul----------------------------\n\n" );
	}
	printf( "\n\nEER : %lf    Threshold : %lf\n\n", min_eer, min_eer_threshold );
		
	return EXIT_SUCCESS;
}

int predict_statistic( const char *trainlist, const char *predictlist ) 
{
	
	train_statistic( trainlist );
	getchar();
	FILE *predict_file = fopen( predictlist, "r" );
	if( predict_file == NULL ) {
		printf( "%s Not Exist.\n", predictlist );
		return EXIT_FAILURE;
	}
	
	string feature_string = "feature_info/WDT_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< features_statistic_struct >v_feature;

	for( int i = 0; i < PREDICT_TRAIN_NUMBER_OF_PEOPLE * PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE; ++i ) {
		features_statistic_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		features_statistic f;
		n = feature_file[num];
		n["id"] >> fs.id;
		n["imagepath"] >> fs.imagepath;
		n["features"] >> fs.features;
		v_feature.push_back( fs );
	}
	feature_file.release();
	int correct = 0;
	int fr = 0;
	int fa = 0;
	clock_t bt = clock();
	printf( "----------------Begin Predict----------------------------\n" );
	for( int i = 0; i < PREDICT_TEST_NUMBER_OF_PEOPLE * PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE + PREDICT_TRAIN_NUMBER_OF_PEOPLE * ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE ); i++ ) {

		features_statistic_struct fs;
		printf( "Begin Caclulate Num:%d\n", i );
		int id = 0;
		char image_path[200];
		fscanf( predict_file, "%d %s", &id, image_path );
		fs.id = id;
		fs.imagepath = image_path;
		Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
		resize(image_gray, image_gray, dsize);
		WDT( image_gray, image_gray, "haar", WAVELET_LEVEL );
		Mat features;
		Size block( WAVELET_BLOCK_SIZE, WAVELET_BLOCK_SIZE );
		get_features( image_gray, FEATURES_STATISTIC_TYPE, features, block, i );

		fs.features = features.clone();
		calc_dis( fs, v_feature, correct, fr, fa );
		printf( "End Caculate Num:%d\n\n", i );
	}
	double frr = ( double )fr / ( PREDICT_TRAIN_NUMBER_OF_PEOPLE * ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE ) );
	double far = ( double )fa / ( PREDICT_TEST_NUMBER_OF_PEOPLE * PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE );
	clock_t et = clock();
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TEST_NUM_TOTAL );
	printf( "\n\n-------Result : GAR : %lf FRR : %lf  FAR : %lf\n", (double)correct / ( PREDICT_TEST_NUMBER_OF_PEOPLE * PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE + PREDICT_TRAIN_NUMBER_OF_PEOPLE * ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE ) ), frr, far );
		fflush( stdout );
		printf( "--------------------------End Of Predict----------------------------\n\n" );
	return EXIT_SUCCESS;
}

void get_features( const Mat &src, const string &type, Mat &features, const Size block, const int num )
{
	assert( src.rows % block.height == 0 && src.cols % block.width == 0 );
	Mat channels[] = { Mat::zeros( Size( src.rows / block.height, src.cols / block.width ), CV_64FC1), Mat::zeros( Size( src.rows / block.height, src.cols / block.width ), CV_64FC1 ) };

	int block_hor = src.cols / block.width;
	int block_ver = src.rows / block.height;
	double info1 = 0.0, info2 = 0.0;
	printf( "get_features_wdt BEGIN   %d---------------------\n", num );
	for( int i = 0; i < block_ver; i++ ) {
		for( int j = 0; j < block_hor; j++ ) {
			Mat block_rect( src, Rect( j * block.width, i * block.height, block.width, block.height ) );

			statistic_info( block_rect, type, info1, info2 ); 
			channels[0].at<double>( i, j ) = info1;
			channels[1].at<double>( i, j ) = info2;
		}
	}
	merge( channels, 2, features );
	printf( "get_features_wdt END   %d-----------------------\n", num );
}

void statistic_info( Mat &src, const string &type, double &info1, double &info2 )
{
	if( type == "WDT" ) {
		Mat mean, convr;
		meanStdDev( src, mean, convr );
		//printf( "%lf %lf ", mean.at<double>( 0, 0 ), convr.at<double>( 0, 0 ) );
		info1 = mean.at<double>( 0, 0 );
		info2 = convr.at<double>( 0, 0 );
	}
}

void calc_dis( const features_statistic_struct &test, const vector< features_statistic_struct > &v_feature, int &correct )
{
	double dist_min = 99999.0;
	int min_index = 0;
	if( WAVELET_MATCH_METHOD == "ED") {
		for( size_t i = 0; i < v_feature.size(); i++ ) {
			double dist = cv::norm( test.features, v_feature[i].features );
			//printf( "src1:%lu	src2:%lu	dist:%lf\n", i + 1, j + 1, dist ) );
		//	dist < min_dist ? ( min_dist = dist, index = j ) : min_dist += 0;
			//fflush( stdout );
			if( dist < dist_min ) {
				dist_min = dist;
				min_index = i;
			}
		}
	} else if( WAVELET_MATCH_METHOD == "AC" ) {
		dist_min = -2;
		for( size_t i = 0; i < v_feature.size(); i++ ) {
			double M1 = cv::norm( test.features );
			double M2 = cv::norm( v_feature[i].features );
			double dist = test.features.dot ( v_feature[i].features ) / ( M1 * M2 );
			if( dist > dist_min ) {
				dist_min = dist;
				min_index = i;
			}
		}
	} else {
	}

	if( test.id == v_feature[min_index].id ) {
		++correct;
	}
	printf( "Test ID : %d---------Cacul ID : %d----------%s\n", test.id, v_feature[min_index].id, test.id == v_feature[min_index].id ? "!!!RIGHT!!!" : "!!!WRONG!!!" );
	fflush( stdout );
}

void calc_dis( const features_statistic_struct &test, const vector< features_statistic_struct > &v_feature, int &correct, int &fr, int &fa )
{

	double dist_max = -2;
	double index = 0;
	for( size_t i = 0; i < v_feature.size(); i++ ) {
		double M1 = cv::norm( test.features );
		double M2 = cv::norm( v_feature[i].features );
		double dist = test.features.dot ( v_feature[i].features ) / ( M1 * M2 );
		if( dist > dist_max ) {
			dist_max = dist;
			index = i;
		}
	}

	printf( "Palm%d	Palm%d Similar:%lf\n", test.id, v_feature[index].id, ( dist_max + 1 ) / 2 );
	if( ( dist_max + 1 ) / 2 > PREDICT_THRESHOLD ) {			//accept
		//printf( "   -----Accept------     \n" );
		if( test.id == v_feature[index].id ) {
			++correct;
			printf( "Test ID : %d---------Cacul ID : %d------ CORRECT!", test.id, v_feature[index].id );
		}
		else {
			++fa;
			printf( "Test ID : %d---------Cacul ID : %d------ WRONG! FALSE ACCEPT!", test.id, v_feature[index].id );
		}		
	} else {		
		//printf( "   -----Reject------     \n" );				//reject
		if( test.id == v_feature[index].id ) {
			++fr;
			printf( "Test ID : %d---------Cacul ID : %d------ WRONG FALSE REJECT", test.id, v_feature[index].id );
		}
		else { 
			++correct;
			printf( "Test ID : %d---------Cacul ID : %d------ CORRECT!", test.id, v_feature[index].id );
		}
	}
	fflush( stdout );
}

void calc_eer( const features_statistic_struct &src1, const features_statistic_struct &src2, int &correct, int &fr, int &fa, double threshold )
{
	double M1 = cv::norm( src1.features );
	double M2 = cv::norm( src2.features );
	double dist = src1.features.dot ( src2.features ) / ( M1 * M2 );
	//printf( "Palm%d	Palm%d Similar:%lf\n", src1.id, src2.id, ( dist + 1 ) / 2 );
	if( ( dist + 1 ) / 2 > threshold ) {			//accept
		//printf( "   -----Accept------     \n" );
		if( src1.id == src2.id ) {
			++correct;
		}
		else {
			++fa;
		}		
	} else {		
		//printf( "   -----Reject------     \n" );				//reject
		if( src1.id == src2.id ) {
			++fr;
		}
		else { 
			++correct;
		}
	}

}
