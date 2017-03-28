/*************************************************************************
	> File Name: dft.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/03/04 16:41:26
 ************************************************************************/
#include "dft.h"

void DFT( const Mat &src, Mat &dst )

{
	Mat padded;
    int m = getOptimalDFTSize( src.rows );  // Return size of 2^x that suite for FFT
    int n = getOptimalDFTSize( src.cols );
    // Padding 0, result is @padded
    copyMakeBorder( src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0) );

    // Create planes to storage REAL part and IMAGE part, IMAGE part init are 0
    Mat planes[] = { Mat_<float>( padded ), Mat::zeros( padded.size(), CV_32FC1 ) };
    Mat complexI;
    merge( planes, 2, complexI );

    dft( complexI, complexI );

	 // compute the magnitude and switch to logarithmic scale
    split( complexI, planes );
    magnitude( planes[0], planes[0], planes[1] );
    Mat magI = planes[0];

   	 // => log(1+sqrt(Re(DFT(I))^2+Im(DFT(I))^2))
    magI += Scalar::all(1);
    log(magI, magI);

    // crop the spectrum
    magI = magI( Rect( 0, 0, magI.cols & (-2), magI.rows & (-2) ) );
    Mat _magI = magI.clone();
    normalize( _magI, _magI, 0, 1, CV_MINMAX );

    // rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0( magI, Rect( 0, 0, cx, cy ) );    // Top-Left
    Mat q1( magI, Rect( cx, 0, cx, cy ) );   // Top-Right
   	Mat q2( magI, Rect( 0, cy, cx, cy ) );   // Bottom-Left
   	Mat q3( magI, Rect( cx, cy, cx,cy ) );  // Bottom-Right

	 // exchange Top-Left and Bottom-Right
    Mat tmp;
    q0.copyTo( tmp );
    q3.copyTo( q0 );
	tmp.copyTo( q3 );

    // exchange Top-Right and Bottom-Left
    q1.copyTo( tmp );
    q2.copyTo( q1 );
    tmp.copyTo( q2 );

    normalize( magI, magI, 0, 1, CV_MINMAX );

    imshow("Spectrum magnitude before shift frequency", _magI);
    imshow("Spectrum magnitude after shift frequency", magI);

}
