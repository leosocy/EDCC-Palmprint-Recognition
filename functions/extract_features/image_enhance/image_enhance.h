/*************************************************************************
	> File Name: image_enhancement.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/04/15 22:17:07
 ************************************************************************/

#ifndef __IMAGE_ENHANCEMENT_H__
#define __IMAGE_ENHANCEMENT_H__

#include <opencv2/opencv.hpp>
using namespace cv;
#include <vector>
using namespace std;

class ImageEnhance
{
public:
	ImageEnhance();
	~ImageEnhance();

	int enhanceWithHistogram( const Mat &src, Mat &dst );
	int enhanceWithLaplace( const Mat &src, Mat &dst, int aperture_size = 3 );
	int enhanceWithSobel( const Mat &src, Mat &dst );
	int enhanceWithCanny( const Mat &src, Mat &dst, double threshold1, double threshold2 );
	int enhanceWithLog( const Mat &src, Mat &dst, int v );
	int enhanceWithGamma( const Mat &src, Mat &dst, double gamma );
};


#endif /* end of image_enhancement.h*/
