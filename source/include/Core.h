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
        Palmprint(_IN const char *identity, _IN const char *imagePath);
        virtual ~Palmprint();

        Palmprint& operator =(_IN const Palmprint &other);
        Palmprint(_IN const Palmprint &other);
        virtual bool operator==(_IN const Palmprint &p) const;

        string getIdentity() const;
        string getImagePath() const;

        cv::Mat* genOriImg();
        cv::Mat* genSpecImg(_IN const cv::Size &imgSize, _IN bool isGray = true);
    private:
        string identity;
        string imagePath;
        cv::Mat image;
    };

    typedef struct tag_EDCC_CFG_T
    {
        #define IMAGE_SIZE_W "imageSizeW"
        u_short imageSizeW;
        #define IMAGE_SIZE_H "imageSizeH"
        u_short imageSizeH;
        #define GABOR_KERNEL_SIZE "gaborKernelSize"
        u_short gaborSize;
        #define LAPLACE_KERNEL_SIZE "laplaceKernelSize"
        u_char laplaceSize;
        #define GABOR_DIRECTIONS "gaborDirections"
        u_char directions;
    } EDCC_CFG_T;

    typedef struct
    {
        EDCC_CFG_T cfg;
        unsigned int codingBuffLen;
        unsigned char pCodingBuff[0];
    } EDCC_CODING_T;

    class EDCCoding
    {
    public:
        EDCCoding();
        virtual ~EDCCoding();

        EDCCoding& operator =(_IN const EDCCoding &other);
        EDCCoding(_IN const EDCCoding &other);

        size_t encrypt(_INOUT unsigned char *pCodingBuf, 
                       _IN size_t bufMaxLen, 
                       _IN const EDCC_CFG_T &config);
        size_t encrypt(_IN const EDCC_CFG_T &config);
        bool decrypt(_IN const unsigned char *pCodingBuf);

        string encodeToHexString(_IN const EDCC_CFG_T &config);
        bool decodeFromHexString(_IN const string &hexString);  
        //string zipCodingC;
        //string zipCodingCs;
        
        EDCC_CFG_T cfg;
    protected:
        Mat C;
        Mat Cs;

        EDCC_CODING_T *ptCoding;
        #define MAGIC_KEY_LEN sizeof(int)
        int magicKey;
        bool initPtCoding(_IN const EDCC_CFG_T &config);
        
        //void compressCoding();
        void genCodingBytes();
    private:
        friend Check;
        friend Match;

        void freeCoding();
    };
    
    class PalmprintCode : public Palmprint, public EDCCoding
    {
    public:
        PalmprintCode(_IN const char *identity, _IN const char *imagePath) :
            Palmprint(identity, imagePath) {};
        virtual ~PalmprintCode();

        PalmprintCode& operator =(_IN const PalmprintCode &other);
        PalmprintCode(_IN const PalmprintCode &other) : Palmprint(other), EDCCoding(other) {}

        bool encodePalmprint(_IN const cv::Size &imgSize,
                             _IN u_short gabKerSize,
                             _IN u_char numOfDirections,
                             _IN u_char lapKerSize);
        bool encodePalmprint(_IN const EDCC_CFG_T &config);
        double matchWith(_IN const PalmprintCode &cmp) const;
    private:
        void enhanceImage(_IN const cv::Mat &src,
                          _INOUT cv::Mat &dst,
                          _IN u_char lapKerSize);
        void genEDCCoding(_IN const vector<cv::Mat> &filterResult,
                          _IN const Size &imgSize,
                          _IN u_char numOfDirections);
    };

    class GaborFilter 
    {
    public:
        GaborFilter(_IN const cv::Size &kernelSize, 
                    _IN u_char numOfDirections);
        virtual ~GaborFilter();
        void doGaborFilter(_IN const cv::Mat &src, _INOUT cv::Mat &dstMerge);
    private:
        cv::Size kernelSize;
        u_char numOfDirections;
        int kernelType;
        void getGaborKernelReal(_INOUT cv::Mat &gaborKernel, _IN int kernelWidth, _IN int kernelHeight,
                                _IN int dimension, _IN int direction,
                                _IN double Kmax = CV_PI / 2, _IN double f = sqrt(2.0),
                                _IN double sigma = 2 * CV_PI, _IN int ktype = CV_64F);
    };
}

#endif
