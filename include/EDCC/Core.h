/*************************************************************************
	> File Name: Core.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/26 22:05:54
 ************************************************************************/
#ifndef __CORE_H__
#define __CORE_H__
#include <EDCC/IO.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
namespace EDCC
{
	class Palmprint {
		public:
			Palmprint( string identity, string imagePath );
			~Palmprint()
			string identity;
			string imagePath;
			void setPalmprintInfo( string identity, string imgPath );
			cv::Mat& generateImage();
			void releaseImage();
		private:
			cv::Mat *image;
	};

	class PalmprintCode {
		public:
			Palmprint *palmprint;
			void encodePalmprint();
			cv::Mat& generateCoding();
			void releaseCoding(); 
		private:
			cv::Mat *coding;		
			void enhanceImage( const cv::Mat &src, cv::Mat &dst );
			void doGaborFilter( const cv::Mat &src, cv::Mat &dst );
	};

	class GaborFilter {
		public:
			GaborFilter( const cv::Size &kernelSize, int dimension, int direction, int kernelType );
			~GaborFilter();
			void doGaborFilter( const cv::Mat &src, cv::Mat &dst  );
			
			enum type{
				GABOR_KERNEL_REAL = 0, 
				GABOR_KERNEL_IMAG, 
				GABOR_KERNEL_MAG
			};
		private:
			void getGaborKernel( cv::Mat &gaborKernel, int kernelWidth, int kernelHeight, int dimension, int direction, int kernelType, double Kmax = CV_PI / 2, double f = sqrt( 2.0 ), double sigma = 2 * CV_PI, int ktype = CV_64F );
	};
}

#endif
