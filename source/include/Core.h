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
#include <cmath>
#include <Pub.h>
#include <Match.h>
using namespace std;
using namespace cv;

namespace EDCC
{
    class Palmprint
    {
        public:
            string identity;
            string imagePath;
            Palmprint();
            Palmprint(const char *identity, const char *imagePath);
            virtual ~Palmprint();	
            virtual Palmprint& operator =(const Palmprint &src);
            bool setPalmprintInfo(const char *identity, const char *imagePath);
            cv::Mat* genOriImg();
            cv::Mat* genSpecImg(const cv::Size &imgSize, bool isGray = true);
        private:    
            cv::Mat image;
    };

    class EDCCoding
    {
        public:
            string zipCodingC;
            string zipCodingCs;
            void compressCoding();
            bool decompressCoding(const char *codingC, const char *codingCs);
        protected:
            Mat C;
            Mat Cs;
    };
    
    class PalmprintCode : public Palmprint, public EDCCoding
    {
        public:
            PalmprintCode(const char *identity, const char *imagePath) : 
                                Palmprint(identity, imagePath){};
            ~PalmprintCode();
            PalmprintCode& operator =(const PalmprintCode &src);
            bool encodePalmprint(const cv::Size &imgSize,
                                        const cv::Size &gabKerSize,
                                        int numOfDirections,
                                        const cv::Size &lapKerSize);
            bool encodePalmprint(const map<string, int> &configMap);
            double matchWith(const PalmprintCode &cmp);
        private:
            void enhanceImage(const cv::Mat &src, cv::Mat &dst, const cv::Size &lapKerSize);
            void genEDCCoding(const vector<cv::Mat> &filterResult, 
                                    const Size &imgSize, 
                                    int numOfDirections);
    };

    class GaborFilter 
    {
        public:
            GaborFilter(const cv::Size &kernelSize, int numOfDirections, int kernelType);
            ~GaborFilter();
            void doGaborFilter(const cv::Mat &src, cv::Mat &dstMerge);
            
            enum {
                GABOR_KERNEL_REAL = 0,
                GABOR_KERNEL_IMAG,
                GABOR_KERNEL_MAG
            };
        private:
            cv::Size kernelSize;
            int numOfDirections;
            int kernelType;
            void getGaborKernel(cv::Mat &gaborKernel, int kernelWidth, int kernelHeight,
                                        int dimension, int direction,
                                        int kernelType,
                                        double Kmax = CV_PI / 2, double f = sqrt( 2.0 ),
                                        double sigma = 2 * CV_PI, int ktype = CV_64F);
    };
}

#endif
