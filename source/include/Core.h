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
#pragma warning(disable : 4200)

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
        EDCCoding();
        ~EDCCoding();

        typedef struct
        {
            int imageSizeW;
            int imageSizeH;
            int gaborSize;
            int laplaceSize;
            int directions;

            unsigned int codingBuffLen;
            unsigned char pCodingBuff[0];
        } EDCC_CODING_T;

        size_t encrypt(_INOUT unsigned char *pCodingBuf, _IN size_t bufMaxLen);
        bool decrypt(_IN unsigned char *pCodingBuf);
    protected:
        Mat C;
        Mat Cs;
        string zipCodingC;
        string zipCodingCs;
        EDCC_CODING_T *ptCoding;
        #define MAGIC_KEY_LEN sizeof(int)
        int magicKey;

        bool initPtCoding(_IN const cv::Size &imgSize,
                          _IN int gabKerSize,
                          _IN int numOfDirections,
                          _IN int lapKerSize);
        void compressCoding();
    };
    
    class PalmprintCode : public Palmprint, public EDCCoding
    {
    public:
        PalmprintCode(_IN const char *identity, _IN const char *imagePath) :
            Palmprint(identity, imagePath) {};
        ~PalmprintCode();
        PalmprintCode& operator =(_IN const PalmprintCode &src);
        bool encodePalmprint(_IN const cv::Size &imgSize,
                             _IN int gabKerSize,
                             _IN int numOfDirections,
                             _IN int lapKerSize);
        bool encodePalmprint(_IN const map<string, int> &configMap);
        double matchWith(_IN const PalmprintCode &cmp) const;
    private:
        void enhanceImage(_IN const cv::Mat &src,
                          _INOUT cv::Mat &dst,
                          _IN int lapKerSize);
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
        void doGaborFilter(_IN const cv::Mat &src, _INOUT cv::Mat &dstMerge);
    private:
        cv::Size kernelSize;
        int numOfDirections;
        int kernelType;
        void getGaborKernelReal(_INOUT cv::Mat &gaborKernel, _IN int kernelWidth, _IN int kernelHeight,
                                _IN int dimension, _IN int direction,
                                _IN double Kmax = CV_PI / 2, _IN double f = sqrt(2.0),
                                _IN double sigma = 2 * CV_PI, _IN int ktype = CV_64F);
    };
}

#endif
