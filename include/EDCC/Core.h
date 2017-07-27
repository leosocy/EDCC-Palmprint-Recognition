/*************************************************************************
	> File Name: Core.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/26 22:05:54
 ************************************************************************/
#ifndef __CORE_H__
#define __CORE_H__
#include <string>
#include <opencv2/opencv.hpp>
namespace EDCC
{
	class Palmprint {
		public:
			Palmprint( string identity, string imagePath );
			~Palmprint()
			string identity;
			string imagePath;
			void setPalmprintInfo( string identity, string imgPath );
		private:
			cv::Mat *image;
	};

	class PalmprintCode {
		public:
			Palmprint palmprint;

			void encodePalmprint( );
		private:
			void enhanceImage( const cv::Mat &src, cv::Mat &dst );
			void doGaborFilter( const cv::Mat &src, cv::Mat &dst );
	};

	class GaborFilter {
		public:
			void doGaborFilter( const cv::Mat &src, cv::Mat &dst );
		private:
			getGaborKernel();
	};
}

#endif
