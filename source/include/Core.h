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
#include <Check.h>
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
        Palmprint(_IN const char *identity, _IN const char *imagePath);
        virtual ~Palmprint();	
        virtual Palmprint& operator =(_IN const Palmprint &src);
        bool setPalmprintInfo(_IN const char *identity, _IN const char *imagePath);
        cv::Mat* genOriImg();
        cv::Mat* genSpecImg(_IN const cv::Size &imgSize, _IN bool isGray = true);
    private:    
        cv::Mat image;
    };

    class EDCCoding
    {
    public:
        string zipCodingC;
        string zipCodingCs;
        void compressCoding();
    protected:
        Mat C;
        Mat Cs;
    };
    
    class PalmprintCode : public Palmprint, public EDCCoding
    {
    public:
        PalmprintCode(_IN const char *identity, _IN const char *imagePath) :
            Palmprint(identity, imagePath) {};
        ~PalmprintCode();
        PalmprintCode& operator =(_IN const PalmprintCode &src);
        bool encodePalmprint(_IN const cv::Size &imgSize,
                             _IN const cv::Size &gabKerSize,
                             _IN int numOfDirections,
                             _IN const cv::Size &lapKerSize);
        bool encodePalmprint(_IN const map<string, int> &configMap);
        double matchWith(_IN const PalmprintCode &cmp) const;
    private:
        void enhanceImage(_IN const cv::Mat &src,
                          _OUT cv::Mat &dst,
                          _IN const cv::Size &lapKerSize);
        void genEDCCoding(_IN const vector<cv::Mat> &filterResult,
                          _IN const Size &imgSize,
                          _IN int numOfDirections);
    };

    class GaborFilter 
    {
    public:
        GaborFilter(_IN const cv::Size &kernelSize, 
                    _IN int numOfDirections);
        ~GaborFilter();
        void doGaborFilter(_IN const cv::Mat &src, _OUT cv::Mat &dstMerge);
    private:
        cv::Size kernelSize;
        int numOfDirections;
        int kernelType;
        void getGaborKernelReal(_OUT cv::Mat &gaborKernel, _IN int kernelWidth, _IN int kernelHeight,
                            _IN int dimension, _IN int direction,
                            _IN double Kmax = CV_PI / 2, _IN double f = sqrt(2.0),
                            _IN double sigma = 2 * CV_PI, _IN int ktype = CV_64F);
    };
}

#endif
