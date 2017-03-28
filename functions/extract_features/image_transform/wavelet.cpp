/*************************************************************************
	> File Name: wavelet.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/02/25 22:31:58
 ************************************************************************/
#include <stdio.h>
#include "wavelet.h"

void wavelet( const string &w_name, Mat &lowFilter, Mat &highFilter );
void waveletDecompose( const Mat &src, Mat &dst, const Mat &lowFilter, const Mat &highFilter );
void waveletReconstruct( const Mat &src, Mat &dst, const Mat &lowFilter, const Mat &highFilter);

void WDT( const Mat &src, Mat &dst, const string &w_name, const int level )
{
	//int reValue = THID_ERR_NONE;
	Mat _src = Mat_<float>( src );
	dst = Mat::zeros( _src.rows, _src.cols, _src.type() ); 
    int N = _src.rows;
    int D = _src.cols;

	Mat lowFilter; 
   	Mat highFilter;
   	wavelet( w_name, lowFilter, highFilter );

	int t = 1;
    int row = N;
    int col = D;
 
	while( t <= level )
    {
        for( int i = 0; i < row; i++ ) 
        {
        	Mat oneRow = Mat::zeros( 1, col, _src.type() );
            for ( int j = 0; j < col; j++ )
            {
            	oneRow.at<float>( 0, j ) = _src.at<float>( i, j );
        	}
        	waveletDecompose( oneRow, oneRow, lowFilter, highFilter );
            for ( int j = 0; j < col; j++ )
            {	//此处需要将灰度值归一化到0-255，不然后期计算图片相似度会导致差异化减小
		if( oneRow.at<float>( 0, j ) < 0 ) {
            		dst.at<float>( i, j ) = 0;
		} else if( oneRow.at<float>( 0, j ) > 1) {
			dst.at<float>( i, j ) = 255;		
		} else {
			dst.at<float>( i, j ) = oneRow.at<float>( 0, j ) * 255;
		}
            }
		}
 
#if 0
        normalize( dst, dst, 0, 255, NORM_MINMAX );
        IplImage dstImg1 = IplImage(dst); 
        cvSaveImage( "dst.jpg", &dstImg1 );
#endif
        for ( int j = 0; j < col; j++ )
        {
            Mat oneCol = Mat::zeros( row, 1, _src.type() );
            for ( int i = 0; i < row; i++ )
            {
                oneCol.at<float>( i, 0 ) = dst.at<float>( i, j );
            }
			Mat oneCol_t = Mat::zeros( 1, row, _src.type() );
            waveletDecompose( oneCol.t(), oneCol_t, lowFilter, highFilter );
			oneCol = oneCol_t.t();
        
            for ( int i = 0; i < row; i++ )
            {
                dst.at<float>( i, j ) = oneCol.at<float>( i, 0 );
            }
        }
	
 
        row /= 2;
        col /= 2;
        t++;
        _src = dst;
    }
}

///  小波逆变换
void IWDT( const Mat &src, Mat &dst, const string &w_name, const int level )
{
    //int reValue = THID_ERR_NONE;
    Mat _src = Mat_<float>( src );
    dst = Mat::zeros( _src.rows, _src.cols, _src.type() ); 
    int N = _src.rows;
    int D = _src.cols;
 
    /// 高通低通滤波器
    Mat lowFilter; 
    Mat highFilter;
    wavelet( w_name, lowFilter, highFilter );
 
    /// 小波变换
    //int t = 1;
    int row = N / std::pow( 2., level - 1 );
    int col = D / std::pow( 2., level - 1 );

    while ( row <= N && col <= D )
    {
        /// 小波列逆变换
        for ( int j = 0; j < col; j++ )
        {
            /// 取出src数据的一行输入
            Mat oneCol = Mat::zeros( row, 1, _src.type() );
            for ( int i = 0; i < row; i++ )
            {
				oneCol.at<float>( i, 0 ) = _src.at<float>( i, j );
            }
			Mat oneCol_t = Mat::zeros( 1, row, _src.type() );
            waveletReconstruct( oneCol.t(), oneCol_t, lowFilter, highFilter );
			oneCol = oneCol_t.t();
 
            for ( int i = 0; i < row; i++ )
            {
                dst.at<float>( i, j ) = oneCol.at<float>( i, 0 );
            } 
        }
 
#if 0
        //normalize( dst, dst, 0, 255, NORM_MINMAX );
        IplImage dstImg2 = IplImage(dst); 
        cvSaveImage( "dst.jpg", &dstImg2 );
#endif
        ///行小波逆变换
        for( int i = 0; i < row; i++ ) 
        {
            /// 取出src中要处理的数据的一行
            Mat oneRow = Mat::zeros( 1, col, _src.type() );
            for ( int j = 0; j < col; j++ )
            {
                oneRow.at<float>( 0, j ) = dst.at<float>( i, j );
            }
            waveletReconstruct( oneRow, oneRow, lowFilter, highFilter );
            /// 将src这一行置为oneRow中的数据
            for ( int j = 0; j < col; j++ )
            {
                dst.at<float>( i, j ) = oneRow.at<float>( 0, j );
            }
        }
 
#if 0
        //normalize( dst, dst, 0, 255, NORM_MINMAX );
        IplImage dstImg1 = IplImage(dst); 
        cvSaveImage( "dst.jpg", &dstImg1 );
#endif
 
        row *= 2;
        col *= 2;
        _src = dst;
    }
}
 
 
////////////////////////////////////////////////////////////////////////////////////////////
 
/// 调用函数
 
/// 生成不同类型的小波，现在只有haar，sym2
void wavelet( const string &w_name, Mat &lowFilter, Mat &highFilter )
{
    if ( w_name=="haar" || w_name=="db1" )
    {
        int N = 2;
        lowFilter = Mat::zeros( 1, N, CV_32F );
        highFilter = Mat::zeros( 1, N, CV_32F );
        
        lowFilter.at<float>( 0, 0 ) =  1.0 / sqrtf( N ); 
        lowFilter.at<float>( 0, 1 ) = 1.0 /  sqrtf ( N ); 
 
        highFilter.at<float>( 0, 0 ) = -1.0 / sqrtf ( N ); 
        highFilter.at<float>( 0, 1 ) = 1.0 /  sqrtf( N ); 
    }
    if ( w_name =="sym2" )
    {
        int N = 4;
        float h[] = {-0.483, 0.836, -0.224, -0.129 };
        float l[] = {-0.129, 0.224, 0.837, 0.483 };
 
		lowFilter = Mat::zeros( 1, N, CV_32F );
        highFilter = Mat::zeros( 1, N, CV_32F );
 
        for ( int i = 0; i < N; i++ )
        {
            lowFilter.at<float>( 0, i ) = l[i]; 
            highFilter.at<float>( 0, i ) = h[i]; 
        }
 
    }
}
 
/// 小波分解
void waveletDecompose( const Mat &src, Mat &dst, const Mat &lowFilter, const Mat &highFilter )
{
    assert( src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1 );
    assert( src.cols >= lowFilter.cols && src.cols >= highFilter.cols );
    Mat _src = Mat_<float>( src );
 
    int D = _src.cols;
    
    Mat _lowFilter = Mat_<float>( lowFilter );
    Mat _highFilter = Mat_<float>( highFilter );
 
 
    /// 频域滤波，或时域卷积；ifft( fft(x) * fft(filter)) = cov(x,filter) 
    Mat dst1 = Mat::zeros( 1, D, _src.type() );
    Mat dst2 = Mat::zeros( 1, D, _src.type()  );
 
    filter2D( _src, dst1, -1, _lowFilter );
    filter2D( _src, dst2, -1, _highFilter );
 
    /// 下采样
    Mat downDst1 = Mat::zeros( 1, D / 2, _src.type() );
    Mat downDst2 = Mat::zeros( 1, D / 2, _src.type() );
 
    resize( dst1, downDst1, downDst1.size() );
    resize( dst2, downDst2, downDst2.size() );
	
    /// 数据拼接
    for ( int i = 0; i < D / 2; i++ )
    {
        _src.at<float>( 0, i ) = downDst1.at<float>( 0, i );
        _src.at<float>( 0, i + D / 2 ) = downDst2.at<float>( 0, i );
    }
 
    dst = _src.clone();
}
 
/// 小波重建
void waveletReconstruct( const Mat &src, Mat &dst, const Mat &lowFilter, const Mat &highFilter )
{
    assert( src.rows == 1 && lowFilter.rows == 1 && highFilter.rows == 1 );
    assert( src.cols >= lowFilter.cols && src.cols >= highFilter.cols );
    Mat _src = Mat_<float>( src );
 
    int D = _src.cols;
 
    Mat _lowFilter = Mat_<float>( lowFilter );
    Mat _highFilter = Mat_<float>( highFilter );
 
    /// 插值;
    Mat Up1 = Mat::zeros( 1, D, _src.type() );
    Mat Up2 = Mat::zeros( 1, D, _src.type() );
 
    /// 插值为0
    //for ( int i=0, cnt=1; i<D/2; i++,cnt+=2 )
    //{
    //    Up1.at<float>( 0, cnt ) = src.at<float>( 0, i );     ///< 前一半
    //    Up2.at<float>( 0, cnt ) = src.at<float>( 0, i+D/2 ); ///< 后一半
    //}
 
    /// 线性插值
    Mat roi1( _src, Rect( 0, 0, D / 2, 1 ) );
    Mat roi2( _src, Rect( D / 2, 0, D / 2, 1 ) );
    resize( roi1, Up1, Up1.size(), 0, 0, INTER_CUBIC );
    resize( roi2, Up2, Up2.size(), 0, 0, INTER_CUBIC );
 
    /// 前一半低通，后一半高通
    Mat dst1 = Mat::zeros( 1, D, _src.type() );
    Mat dst2 = Mat::zeros( 1, D, _src.type() );
    filter2D( Up1, dst1, -1, _lowFilter );
	filter2D( Up2, dst2, -1, _highFilter );

    /// 结果相加
    dst1 = dst1 + dst2;
 
	dst = dst1.clone(); 
}
