// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/palmprint_code.h"
#include <cmath>
#include "core/palmprint.h"
#include "core/edccoding.h"
#include "core/check.h"
#include "core/match.h"
#include "edcc.h"

namespace edcc
{

namespace
{

class GaborFilter 
{
public:
    GaborFilter(const cv::Size &kernel_size, 
                u_char directions);
    virtual ~GaborFilter();
    void Handle(const cv::Mat &src, cv::Mat *merge);
private:
    cv::Size kernel_size_;
    u_char directions_;
    void GetGaborKernelReal(cv::Mat *kernel, int width, int height,
                            int dimension, int direction,
                            double kmax = CV_PI / 2, double f = sqrt(2.0),
                            double sigma = 2 * CV_PI, int ktype = CV_64F);
};

GaborFilter::GaborFilter(const cv::Size &kernel_size,
                         u_char directions)
{
    assert(kernel_size.width % 2 == 1 && kernel_size.height % 2 == 1);
    assert(directions > 0);

    kernel_size_ = kernel_size;
    directions_ = directions;
}

GaborFilter::~GaborFilter()
{
}

void GaborFilter::Handle(const cv::Mat &src, cv::Mat *merge)
{
    assert(merge);
    vector<cv::Mat> dest_vec;
    Mat dst;
    Mat gabor_kernel;
    for (u_char direction = 0; direction < directions_; ++direction)
    {
        GetGaborKernelReal(&gabor_kernel, kernel_size_.width, kernel_size_.height, 0, direction);
        filter2D(src, dst, CV_64F, gabor_kernel);
        dest_vec.push_back(dst.clone());
    }
    cv::merge(dest_vec, *merge);
    cv::normalize(*merge, *merge, 0, 1, CV_MINMAX);
}

void GaborFilter::GetGaborKernelReal(cv::Mat *kernel,
                                     int width, int height,
                                     int dimension, int direction,
                                     double kmax, double f,
                                     double sigma, int ktype)
{
    assert(ktype == CV_32F || ktype == CV_64F);

    int half_width = width / 2;
    int half_height = height / 2;
    double Qu = CV_PI * direction / directions_;
    double sqsigma = sigma * sigma;
    double Kv = kmax / pow(f, dimension);
    double postmean = exp(-sqsigma / 2);
    kernel->create(width, height, ktype);
    //Mat kernel(width, height, ktype);
    double tmp1, tmp2;
    for (int i = -half_height; i <= half_height; ++i)
    {
        for (int j = -half_width; j <= half_width; ++j)
        {
            tmp1 = exp(-(Kv * Kv * (i * i + j * j)) / (2 * sqsigma));
            tmp2 = cos(Kv * cos(Qu) * j + Kv * sin(Qu) * i) - postmean;
            if (ktype == CV_32F)
            {
                kernel->at<float>(i + half_height, j + half_width) =
                    (float)(Kv * Kv * tmp1 * tmp2 / sqsigma);
            }
            else
            {
                kernel->at<double>(i + half_height, j + half_width) =
                    (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
            }
        }
    }
    //gaborKernel = kernel.clone();
}

} // namespace

PalmprintCode::PalmprintCode(const char *identity, const char *image_path)
{
    Init(identity, image_path);
}

PalmprintCode::~PalmprintCode()
{
    if (palmprint_)
    {
        delete palmprint_;
        palmprint_ = NULL;
    }
    if (coding_)
    {
        delete coding_;
        coding_ = NULL;
    }
}

PalmprintCode& PalmprintCode::operator =(const PalmprintCode &other)
{
    assert(palmprint_ && coding_
           && other.palmprint_ && other.coding_);
    if (this != &other)
    {
        *palmprint_ = *other.palmprint_;
        *coding_ = *other.coding_;
    }
    return *this;
}

PalmprintCode::PalmprintCode(const PalmprintCode &other)
{
    assert(other.palmprint_ && other.coding_);
    Init(other.palmprint_->identity().c_str(), 
         other.palmprint_->image_path().c_str());
    *palmprint_ = *other.palmprint_;
    *coding_ = *other.coding_;
}

Status PalmprintCode::Encode(const EDCC_CFG_T &config)
{
    Check checker;
    if (!checker.CheckConfig(config))
    {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    
    GaborFilter filter(cv::Size(config.gaborSize, config.gaborSize),
                       config.directions);
    Mat *imgRet = palmprint_->GenSpecImg(cv::Size(config.imageSizeW, config.imageSizeH));
    if (imgRet == NULL)
    {
        EDCC_Log("%s not exists!", palmprint_->image_path().c_str());
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }
    Mat palmprintImage = *imgRet;
    Mat gaborResult;
    EnhanceImage(palmprintImage, palmprintImage, config.laplaceSize);
    filter.Handle(palmprintImage, &gaborResult);
    vector<cv::Mat> resultVec;
    split(gaborResult, resultVec);
    GenEDCCoding(resultVec,
                 cv::Size(config.imageSizeW, config.imageSizeH),
                 config.directions);

    size_t buffer_size = 0;
    return coding_->Encode(config, &buffer_size);
}

Status PalmprintCode::EncodeToBuffer(const EDCC_CFG_T &config,
                                     size_t buffer_max_len,
                                     u_char *coding_buffer,
                                     size_t *buffer_size)
{
    assert(coding_);
    Status s = Encode(config);
    if (s != EDCC_SUCCESS)
    {
        *buffer_size = 0;
        return s;
    }
    return coding_->Encode(config, buffer_max_len, coding_buffer, buffer_size);
}

Status PalmprintCode::EncodeToHexString(const EDCC_CFG_T &config, string *hex_str)
{
    assert(coding_);
    Status s = Encode(config);
    if (s != EDCC_SUCCESS)
    {
        return s;
    }
    return coding_->EncodeToHexString(config, hex_str);
}

Status PalmprintCode::Decode(const u_char *coding_buffer)
{
    assert(coding_);
    return coding_->Decode(coding_buffer);
}

Status PalmprintCode::DecodeFromHexString(const string &hex_str)
{
    assert(coding_);
    return coding_->DecodeFromHexString(hex_str);
}

void PalmprintCode::Init(const char *identity, const char *image_path)
{
    palmprint_ = NULL;
    coding_ = NULL;
    palmprint_ = new Palmprint(identity, image_path);
    coding_ = new EDCCoding();
}

void PalmprintCode::EnhanceImage(const cv::Mat &src,
                                 cv::Mat &dst,
                                 u_char lapKerSize)
{
    Mat gaussian;
    GaussianBlur(src, gaussian, Size(5, 5), 0, 0, cv::BORDER_DEFAULT);
    Laplacian(gaussian, dst, CV_64F, lapKerSize);
}

void PalmprintCode::GenEDCCoding(const vector<cv::Mat> &gabor_filter_result,
                                 const Size &img_size,
                                 u_char directions)
{
    coding_->c_ = Mat(img_size, CV_8S);
    coding_->cs_ = Mat(img_size, CV_8S);
    for (int h = 0; h < img_size.height; ++h)
    {
        for (int w = 0; w < img_size.width; ++w)
        {
            double maxResponse = -DBL_MAX;
            int maxDirection = -1;
            int Cleft = -1, Cright = -1;
            for (u_char d = 0; d < directions; ++d)
            {
                double response = gabor_filter_result[d].at<double>(h, w);
                if (response > maxResponse)
                {
                    maxResponse = response;
                    maxDirection = d;
                }
            }
            coding_->c_.at<char>(h, w) = maxDirection;
            maxDirection == directions - 1 ? Cleft = 0 : Cleft = maxDirection + 1;
            maxDirection == 0 ? Cright = directions - 1 : Cright = maxDirection - 1;
            coding_->cs_.at<char>(h, w) = gabor_filter_result[Cleft].at<double>(h, w) >=
                gabor_filter_result[Cright].at<double>(h, w) ? 1 : 0;
        }
    }
}

double PalmprintCode::MatchWith(const PalmprintCode &another) const
{
    Match matcher;
    return matcher.MatchPoint2Point(*this, another);
}

} // namespace edcc
