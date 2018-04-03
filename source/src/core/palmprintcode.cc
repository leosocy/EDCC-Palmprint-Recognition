// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/palmprintcode.h"
#include <cmath>
#include "core/palmprint.h"
#include "core/edccoding.h"
#include "core/checker.h"
#include "core/matcher.h"
#include "util/status.h"

namespace edcc
{

using namespace std;

namespace
{

class GaborFilter
{
public:
    GaborFilter(const cv::Size &kernel_size,
                u_char directions)
    {
        assert(kernel_size.width % 2 == 1 && kernel_size.height % 2 == 1);
        assert(directions > 0);
        kernel_size_ = kernel_size;
        directions_ = directions;
    }
    void Handle(const cv::Mat &src, cv::Mat *merge);
private:
    cv::Size kernel_size_;
    u_char directions_;
    void GetGaborKernelReal(cv::Mat *kernel, int width, int height,
                            int dimension, int direction,
                            double kmax = CV_PI / 2, double f = sqrt(2.0),
                            double sigma = 2 * CV_PI, int ktype = CV_64F);
};

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
    : palmprint_(new Palmprint(identity, image_path)),
    coding_(new EDCCoding())
{
}

PalmprintCode::PalmprintCode(const PalmprintCode &other)
    : palmprint_(new Palmprint(*other.palmprint_)),
    coding_(new EDCCoding(*other.coding_))
{
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

Status PalmprintCode::Encode(const EDCC_CFG_T &config)
{
    if (!Checker::CheckConfig(config))
    {
        return Status::LoadConfigError();
    }

    GaborFilter filter(cv::Size(config.gaborSize, config.gaborSize),
                       config.directions);
    Mat *spec_img = palmprint_->GetSpecImg(cv::Size(config.imageSizeW, config.imageSizeH));
    if (spec_img == NULL)
    {
        EDCC_Log("%s not exists!", palmprint_->image_path().c_str());
        return Status::LoadPalmprintError();
    }
    Mat img_tmp = spec_img->clone();
    Mat gabor_result;
    EnhanceImage(img_tmp, img_tmp, config.laplaceSize);
    filter.Handle(img_tmp, &gabor_result);
    vector<cv::Mat> result_vec;
    split(gabor_result, result_vec);
    GenEDCCoding(result_vec,
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
    if (!s.IsOk())
    {
        *buffer_size = 0;
        return s;
    }
    return coding_->EncodeToBuffer(config, buffer_max_len, coding_buffer, buffer_size);
}

Status PalmprintCode::EncodeToHexString(const EDCC_CFG_T &config, string *hex_str)
{
    assert(coding_);
    Status s = Encode(config);
    if (!s.IsOk())
    {
        return s;
    }
    return coding_->EncodeToHexString(config, hex_str);
}

Status PalmprintCode::DecodeFromBuffer(const u_char *coding_buffer)
{
    assert(coding_);
    return coding_->DecodeFromBuffer(coding_buffer);
}

Status PalmprintCode::DecodeFromHexString(const string &hex_str)
{
    assert(coding_);
    return coding_->DecodeFromHexString(hex_str);
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
            double max_response = -DBL_MAX;
            int max_direction = -1;
            int c_left = -1, c_right = -1;
            for (u_char d = 0; d < directions; ++d)
            {
                double response = gabor_filter_result[d].at<double>(h, w);
                if (response > max_response)
                {
                    max_response = response;
                    max_direction = d;
                }
            }
            coding_->c_.at<char>(h, w) = max_direction;
            max_direction == directions - 1 ? c_left = 0 : c_left = max_direction + 1;
            max_direction == 0 ? c_right = directions - 1 : c_right = max_direction - 1;
            coding_->cs_.at<char>(h, w) = gabor_filter_result[c_left].at<double>(h, w) >=
                gabor_filter_result[c_right].at<double>(h, w) ? 1 : 0;
        }
    }
}

} // namespace edcc
