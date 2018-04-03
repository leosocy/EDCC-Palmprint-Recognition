// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __PALMPRINT_CODE_H__
#define __PALMPRINT_CODE_H__

#include <string>
#include <opencv2/opencv.hpp>
#include "util/pub.h"

namespace edcc
{

using std::string;
using cv::Mat;
using cv::Size;

class Palmprint;
class EDCCoding;
class Status;
typedef struct tag_EDCC_CFG_T EDCC_CFG_T;

class PalmprintCode
{
public:
    PalmprintCode(const char *identity, const char *image_path);
    PalmprintCode(const PalmprintCode &other);
    PalmprintCode& operator =(const PalmprintCode &other);
    ~PalmprintCode();
    Status Encode(const EDCC_CFG_T &config);
    Status EncodeToBuffer(const EDCC_CFG_T &config,
                          size_t buffer_max_len,
                          u_char *coding_buffer,
                          size_t *buffer_size);
    Status EncodeToHexString(const EDCC_CFG_T &config, string *hex_str);
    Status DecodeFromBuffer(const u_char *coding_buffer);
    Status DecodeFromHexString(const string &hex_str);
    const Palmprint* palmprint() const { return palmprint_; };
    const EDCCoding* coding() const { return coding_; };
private:
    void EnhanceImage(const cv::Mat &src,
                      cv::Mat &dst,
                      u_char lapKerSize);
    void GenEDCCoding(const std::vector<cv::Mat> &filterResult,
                      const Size &imgSize,
                      u_char numOfDirections);

    Palmprint *palmprint_;
    EDCCoding *coding_;
};

} // namespace edcc

#endif // __PALMPRINT_CODE_H__