/*************************************************************************
	> File Name: extract_features_with_subspace.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/13 20:47:36
 ************************************************************************/

#include "extract_features_with_subspace.h"

void calc_dis( const features_subspace_struct &test, const vector< features_subspace_struct > &v_feature, int &correct );
void calc_dis( const features_subspace_struct &test, const vector< features_subspace_struct > &v_feature, int &correct, int &fr, int &fa, FILE *dist_out );
void calc_eer( const features_subspace_struct &src1, const features_subspace_struct &src2, int &correct, int &fr, int &fa, double threshold );

BDPCALDA::BDPCALDA()
{
}

BDPCALDA::~BDPCALDA()
{
}

int BDPCALDA::doExtractFeatures( const char *filename )
{
	FILE *roi_list = fopen( filename, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", filename );
		return EXIT_FAILURE;
	}
	vector< features_subspace_struct > v_feature;
	vector< Mat >palm_all;

	
	int count = 0;
	int preID = 0;
	clock_t bt = clock();
	printf( "--------------------------------Begin Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {	
		int id = 0;
		char image_path[200];
		fscanf( roi_list, "%d %s", &id, image_path );	
		count += 1;
		if( count > this->trainNum ) {
			if( preID == id ) {
				continue;
			} else {
				count = 1;			
			}		
		}
		preID = id;
		this->labels.push_back( id );
		Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
		resize(image_gray, image_gray, dsize);
		features_subspace_struct one_palm;
		one_palm.id = id;
		one_palm.imagepath = image_path;
		v_feature.push_back( one_palm );
		printf( "Push back Num:%d\n", i );
		fflush( stdout );
		Mat tmp;
		image_gray.convertTo( tmp, CV_64FC1 );
		palm_all.push_back( tmp );	
	}
	/*************************Reduce Dimension With BDPCA********************************/
	Mat U, V;
	Mat palm_pca, VT;
	DoBDPCA( palm_all, U, V, 0.5, this->Krow, this->Kcol );
	Mat lda_image = Mat::zeros( palm_all.size(), V.cols * U.cols, CV_64FC1 );
	vector< int > tmp_labels;
	vector< lda_struct > lda_record;
	for( size_t i = 0; i < palm_all.size(); ++i ) {
		transpose( V, VT );
		palm_pca = VT * palm_all[i] * U;
		v_feature[i].features = palm_pca.clone();
		lda_struct one_lda;
		tmp_labels.push_back( v_feature[i].id - 1 ); 
		for( int j = 0; j < V.cols; ++j ) {
			for( int k = 0; k < U.cols; ++k ) {
				lda_image.at<double>( i, j * V.cols + k ) = palm_pca.at<double>( j, k );
			}
		}
	}
	this->U = U.clone();
	this->VT = VT.clone();

	printf( "Do LDA\n" );
	LDA palm_lda = LDA( lda_image, tmp_labels );
	printf( "End LDA\n" );
	
	Mat eivector = palm_lda.eigenvectors().clone();
	cout << "The eigenvector rows:" << eivector.rows << "   cols:" << eivector.cols << endl;
	fflush( stdout );
	this->LDAEIVECTOR = eivector.clone();
	
	for( int i = 0; i < v_feature.size(); ++i ) {
		Mat oneRow = Mat::zeros( 1, v_feature[i].features.rows * v_feature[i].features.cols, CV_64FC1 );
		for( int j = 0; j < v_feature[i].features.rows; ++j ) {
			for( int k = 0; k < v_feature[i].features.cols; ++k ) {
				oneRow.at<double>( 0, j * v_feature[i].features.rows + k ) = v_feature[i].features.at<double>( j, k );
			}
		}
		printf( "Begin Write Num:%d\n\n", i );	
		this->features.push_back( oneRow * eivector );
		printf( "End Write Num:%d\n\n", i );		
	}
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );	
	fflush( stdout );
	return EXIT_SUCCESS;
}

void BDPCALDA::doVerification( int dataSize )
{
	FILE *matchFile = fopen( "./feature_info/Multispectral_B_MatchScore_BDPCALDA.txt", "w" );
	FILE *tuningFile = fopen( "./feature_info/Tuning_BDPCALDA.txt", "a" );
	clock_t bt = clock(), et;
	int gen[101] = { 0 };
	int imp[101] = { 0 };
	double score = 0.0;
	int GAR = 0, FAR = 0, FRR = 0;
	Mat scoreMat = Mat::zeros( Size( (this->features).size(), (this->features).size() ), CV_64FC1 );
	for( int i = 0; i < (this->features).size(); ++i ) {
		for( int j = i + 1; j < (this->features).size(); ++j ) {	
			score = match( this->features[i], this->features[j] );
			scoreMat.at<double>( i, j ) = score;
			scoreMat.at<double>( j, i ) = score;
			printf( "Cacl ---- src1:%d  src2:%d  Score:%lf\n\n", this->labels[i], this->labels[j], score );	
			int classType = 1;
			int index = score / 0.01;
			if( this->labels[i] == this->labels[j] ) { 
				classType = 0;
				gen[index] += 1;			
			} else {
				imp[index] += 1;			
			}
			//fprintf( matchFile, "%d %d %d %lf\n", i, j , classType, score );
		}	
	}
	for( int i = 0; i < 101; ++i ) {
		fprintf( matchFile, "%.2f %lf %lf\n", i * 0.01, 100 * (double)gen[i] / ( (this->features).size() * ( this->trainNum - 1 ) ) * 2,  100 * (double)imp[i] / ( (this->features).size() * ( (this->features).size() - this->trainNum ) ) * 2 );	
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
	fprintf( tuningFile, "%d %d %lf\n", this->Krow, this->Kcol, (double)GAR / this->features.size() );
	fclose( tuningFile );
	printf( "End of Matching.Total Cost Time: %lf  Per Image Cost Time: %lf  GAR:%lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / (this->features).size(), (double)GAR / this->features.size() );
}
double BDPCALDA::match( const Mat &X, const Mat &Y )
{
	double M1 = cv::norm( X );
	double M2 = cv::norm( Y );
	double dist = X.dot ( Y ) / ( M1 * M2 );
	return dist;
}

int train_subspace( const char *trainlist )
{
	FILE *roi_list = fopen( trainlist, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", trainlist );
		return EXIT_FAILURE;
	}
	vector< features_subspace_struct > v_feature;
	vector< Mat >palm_all;

	map< int, char > class_freq;
	clock_t bt = clock();
	printf( "--------------------------------Begin Train--------------------\n" );
	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL && !feof( roi_list ); ++i ) {	
		int id = 0;
		char image_path[200];
		fscanf( roi_list, "%d %s", &id, image_path );
		map< int, char >::iterator it;
		it = class_freq.find( id );
		if( it == class_freq.end() ) {
			class_freq.insert( map< int, char >::value_type( id, 1 ) );
		}
		Mat image = imread( image_path, CV_LOAD_IMAGE_COLOR );
		Mat image_gray;
		cvtColor( image, image_gray, CV_BGR2GRAY );
		double proportion = (double)image.cols / image.rows;
		Size dsize = Size( IMAGE_HEIGHT * proportion, IMAGE_HEIGHT );
		resize(image_gray, image_gray, dsize);
		features_subspace_struct one_palm;
		one_palm.id = id;
		one_palm.imagepath = image_path;
		v_feature.push_back( one_palm );
		printf( "Push back Num:%d\n", i );
		fflush( stdout );
		Mat tmp;
		image_gray.convertTo( tmp, CV_64FC1 );
		palm_all.push_back( tmp );	
	}
	/*************************Reduce Dimension With BDPCA********************************/
	Mat U, V;
	Mat palm_pca, VT;
	DoBDPCA( palm_all, U, V, 0.5, 35, 35 );
	Mat lda_image = Mat::zeros( palm_all.size(), V.cols * U.cols, CV_64FC1 );
	vector< int >labels;
	vector< lda_struct > lda_record;
	for( size_t i = 0; i < palm_all.size(); ++i ) {
		/*char num[10];
		sprintf( num, "%c%lu", '_', i );
		const string s_num( num );
		n = feature_file[num];
		if( n.empty() ) {*/
		
		transpose( V, VT );
		palm_pca = VT * palm_all[i] * U;
		v_feature[i].features = palm_pca.clone();
		lda_struct one_lda;
		labels.push_back( v_feature[i].id - 1 ); 
		for( int j = 0; j < V.cols; ++j ) {
			for( int k = 0; k < U.cols; ++k ) {
				lda_image.at<double>( i, j * V.cols + k ) = palm_pca.at<double>( j, k );
			}
		}
		//}
			/*Mat features;
			Size block( BLOCK_SIZE );
			get_features( image_gray, FEATURES_STATISTIC_TYPE, features, block, i );

			//v_feature.push_back( features );
			feature_file << s_num;
			feature_file << "{";
			feature_file << "id" << id;
			feature_file << "imagepath" << image_path;
			feature_file << "features" << features;
			feature_file << "}";
			printf( "End Write Num:%d\n\n", i );*/
	}

	printf( "Do LDA\n" );
	LDA palm_lda = LDA( lda_image, labels );
	printf( "End LDA\n" );
	
	Mat eivector = palm_lda.eigenvectors().clone();
	cout << "The eigenvector rows:" << eivector.rows << "   cols:" << eivector.cols << endl;
	fflush( stdout );
	/*for( int i = 0; i < eivector.rows; ++i ) {
		for( int j = 0; j < eivector.cols; ++j ) {
			cout << eivector.at<double>( i, j ) << " ";
		}
		cout << endl;
	}*/

	/*int classNum = class_freq.size();
	vector< Mat > classMean( classNum );
	vector< int > setNum( classNum );

	for( int i = 0; i < classNum; ++i ) {
		classMean[i] = Mat::zeros( 1, lda_image.cols, lda_image.type() );
		setNum[i] = 0;
	}

	Mat instance;
	for( int i = 0; i < lda_image.rows; ++i ) {
		instance = lda_image.row( i );
		classMean[labels[i] - 1] += instance;
		setNum[labels[i] - 1]++;
	}
	for( int i = 0; i < classNum; ++i ) {
		classMean[i].convertTo( classMean[i], CV_64FC1, 1.0 / static_cast<double>( setNum[i] ) );
	}
	cout << endl << "eivector" << endl << eivector << endl << endl;

	for( int i = 0; i < classNum; ++i ) {
		cout<<endl<<"第" << i + 1 << "个类均值的投影:" << endl;  
		cout<<palm_lda.project( classMean[i]).at<double>(0);  
	}*/
	
	string feature_string = "feature_info/BDPCA+LDA_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	n = feature_file["U"];
	if( n.empty() ) {
		feature_file.open( feature_string, FileStorage::APPEND );	
		feature_file << "U" << U;
	}
	n = feature_file["VT"];
	if( n.empty() ) {
		feature_file.open( feature_string, FileStorage::APPEND );
		feature_file << "VT" << VT;
	}
	n = feature_file["LDA_VECTOR"];
	if( n.empty() ) {
		feature_file.open( feature_string, FileStorage::APPEND );
		feature_file << "LDA_VECTOR" << eivector;
	}
	for( int i = 0; i < v_feature.size(); ++i ) {
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		n = feature_file[num];

		if( n.empty() ) {
			feature_file.open( feature_string, FileStorage::APPEND );
			Mat oneRow = Mat::zeros( 1, v_feature[i].features.rows * v_feature[i].features.cols, CV_64FC1 );
			for( int j = 0; j < v_feature[i].features.rows; ++j ) {
				for( int k = 0; k < v_feature[i].features.cols; ++k ) {
					oneRow.at<double>( 0, j * v_feature[i].features.rows + k ) = v_feature[i].features.at<double>( j, k );
				}
			}
			printf( "Begin Write Num:%d\n\n", i );	
			v_feature[i].features = oneRow * eivector;
			feature_file << s_num;
			feature_file << "{";
			feature_file << "id" << v_feature[i].id;
			feature_file << "imagepath" << v_feature[i].imagepath;
			feature_file << "features" << v_feature[i].features;
			feature_file << "}";
			printf( "End Write Num:%d\n\n", i );		
		}	
	}
	feature_file.release();
	clock_t et = clock();
	printf( "--------------------------End Of Train----------------------------\n\n" );
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TRAIN_NUM_TOTAL );	
	fflush( stdout );
	return EXIT_SUCCESS;
}

int test_subspace( const char *testList )
{
	FILE *roi_list = fopen( testList, "r" );
	if( roi_list == NULL ) {
		printf( "%s Not Exist.\n", testList );
		return EXIT_FAILURE;
	}
	
	string feature_string = "feature_info/BDPCA+LDA_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	Mat U, VT, LDA_VECTOR;
	n = feature_file["U"];
	n >> U;
	n = feature_file["VT"];
	n >> VT;
	n = feature_file["LDA_VECTOR"];
	n >> LDA_VECTOR;
	vector< features_subspace_struct >v_feature;

	for( int i = 0; i < LIST_TRAIN_NUM_TOTAL; ++i ) {
		features_subspace_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		n = feature_file[num];
		n["id"] >> fs.id;
		n["imagepath"] >> fs.imagepath;
		n["features"] >> fs.features;
		v_feature.push_back( fs );
	}
	cout << "U" << "rows: " << U.rows << "  cols: " << U.cols << endl;
	cout << "VT" << "rows: " << VT.rows << "  cols: " << VT.cols << endl;
	cout << "Size:" << v_feature.size() << endl;
	
	feature_file.release();
	int correct = 0;
	clock_t bt = clock();
	printf( "----------------Begin Test----------------------------\n" );
	for( int i = 0; i < LIST_TEST_NUM_TOTAL; i++ ) {
		features_subspace_struct fs;
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
		/*************************extract features with BDPCA+LDA********************************/
		Mat tmp;
		image_gray.convertTo( tmp, CV_64FC1 );
		Mat palm_pca = Mat::zeros( VT.rows, U.cols, CV_64FC1 );
		palm_pca = VT * tmp * U;
		Mat oneRow = Mat::zeros( 1, VT.rows * U.cols, CV_64FC1 );	
		for( int j = 0; j < VT.rows; ++j ) {
			for( int k = 0; k < U.cols; ++k ) {
				oneRow.at<double>( 0, j * VT.rows + k ) = palm_pca.at<double>( j, k );
			}		
		}
		Mat palm_lda = oneRow * LDA_VECTOR;
		fs.features = palm_lda.clone();
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

int predict_subspace( const char *trainlist, const char *predictlist ) 
{
	
	train_subspace( trainlist );
	getchar();
	FILE *predict_file = fopen( predictlist, "r" );
	if( predict_file == NULL ) {
		printf( "%s Not Exist.\n", predictlist );
		return EXIT_FAILURE;
	}
	
	string feature_string = "feature_info/BDPCA+LDA_FEATURES.xml";
	FILE *dist_out = fopen( "feature_info/BDPCA+LDA_DIST.txt", "w" );
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	Mat U, VT, LDA_VECTOR;
	n = feature_file["U"];
	n >> U;
	n = feature_file["VT"];
	n >> VT;
	n = feature_file["LDA_VECTOR"];
	n >> LDA_VECTOR;
	vector< features_subspace_struct >v_feature;

	for( int i = 0; i < PREDICT_TRAIN_NUMBER_OF_PEOPLE * PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE; ++i ) {
		features_subspace_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
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

		features_subspace_struct fs;
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
		
		/*************************extract features with BDPCA+LDA********************************/
		Mat tmp;
		image_gray.convertTo( tmp, CV_64FC1 );
		Mat palm_pca = Mat::zeros( VT.rows, U.cols, CV_64FC1 );
		palm_pca = VT * tmp * U;
		Mat oneRow = Mat::zeros( 1, VT.rows * U.cols, CV_64FC1 );	
		for( int j = 0; j < VT.rows; ++j ) {
			for( int k = 0; k < U.cols; ++k ) {
				oneRow.at<double>( 0, j * VT.rows + k ) = palm_pca.at<double>( j, k );
			}		
		}
		Mat palm_lda = oneRow * LDA_VECTOR;
		fs.features = palm_lda.clone();
		calc_dis( fs, v_feature, correct, fr, fa, dist_out );
		printf( "End Caculate Num:%d\n\n", i );
	}
	double frr = ( double )fr / ( PREDICT_TRAIN_NUMBER_OF_PEOPLE * ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE ) );
	double far = ( double )fa / ( PREDICT_TEST_NUMBER_OF_PEOPLE * PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE );
	clock_t et = clock();
	printf( "Total Cost Time: %lf  Per Image Cost Time: %lf\n\n", ( (double)( et ) - bt ) / CLOCKS_PER_SEC, ( ( ( double )et - bt ) / CLOCKS_PER_SEC ) / LIST_TEST_NUM_TOTAL );
	printf( "\n\n-------Result : GAR : %lf FRR : %lf  FAR : %lf\n", (double)correct / ( PREDICT_TEST_NUMBER_OF_PEOPLE * PREDICT_TEST_PER_PEOPLE_NUMBER_OF_IMAGE + PREDICT_TRAIN_NUMBER_OF_PEOPLE * ( ALL_PER_PEOPLE_NUMBER_OF_IMAGE - PREDICT_TRAIN_PER_PEOPLE_NUMBER_OF_IMAGE ) ), frr, far );
	fflush( stdout );
	printf( "--------------------------End Of Predict----------------------------\n\n" );
	fclose( dist_out );
	return EXIT_SUCCESS;
}

int calcu_eer_threshold_subspace( const char *calculist )
{
	FILE *calcu_list = fopen( calculist, "r" );
	if( calcu_list == NULL ) {
		printf( "%s Not Exist.\n", calculist );
		return EXIT_FAILURE;
	}
	#undef LIST_TRAIN_NUM_TOTAL
	#define LIST_TRAIN_NUM_TOTAL CALCU_EER_NUMBER_OF_IMAGE
	train_subspace( calculist );
	getchar();
	string feature_string = "feature_info/BDPCA+LDA_FEATURES.xml";
	FileStorage feature_file( feature_string, FileStorage::READ );
	FileNode n;
	vector< features_subspace_struct > v_feature;
	for( int i = 0; i < CALCU_EER_NUMBER_OF_IMAGE; ++i ) {
		features_subspace_struct fs;
		char num[10];
		sprintf( num, "%c%d", '_', i );
		const string s_num( num );
		n = feature_file[num];
		n["id"] >> fs.id;
		n["imagepath"] >> fs.imagepath;
		n["features"] >> fs.features;
		v_feature.push_back( fs );
	}

	feature_file.release();
	
	double threshold = 0.91;
	double min_eer = 1.0;
	double min_eer_threshold = 0.0;
	printf( "----------------EER Begin Calcu----------------------------\n" );
	for( ; threshold <= 1.00; threshold += 0.005 ) {
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

void calc_dis( const features_subspace_struct &test, const vector< features_subspace_struct > &v_feature, int &correct )
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
	} else {
		getchar();	
	}
	printf( "Test ID : %d---------Cacul ID : %d----------%s\n", test.id, v_feature[min_index].id, test.id == v_feature[min_index].id ? "!!!RIGHT!!!" : "!!!WRONG!!!" );
	fflush( stdout );
}

void calc_dis( const features_subspace_struct &test, const vector< features_subspace_struct > &v_feature, int &correct, int &fr, int &fa, FILE *dist_out )
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

	fprintf( dist_out, "%d %lf\n", test.id == v_feature[index].id, dist_max );
	if( ( dist_max + 1 ) / 2 > PREDICT_THRESHOLD ) {			//accept
		//printf( "   -----Accept------     \n" );
		if( test.id == v_feature[index].id ) {
			++correct;
			printf( "Test ID : %d---------Cacul ID : %d------ CORRECT!", test.id, v_feature[index].id );
		}
		else {
			++fa;
			printf( "Test ID : %d---------Cacul ID : %d------ WRONG! FALSE ACCEPT!\n", test.id, v_feature[index].id );
		}		
	} else {		
		//printf( "   -----Reject------     \n" );				//reject
		if( test.id == v_feature[index].id ) {
			++fr;
			printf( "Test ID : %d---------Cacul ID : %d------ WRONG FALSE REJECT\n", test.id, v_feature[index].id );
		}
		else { 
			++correct;
			printf( "Test ID : %d---------Cacul ID : %d------ CORRECT!\n", test.id, v_feature[index].id );
		}
	}
	fflush( stdout );
}

void calc_eer( const features_subspace_struct &src1, const features_subspace_struct &src2, int &correct, int &fr, int &fa, double threshold )
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


