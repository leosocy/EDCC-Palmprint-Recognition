// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __EDCCODING_H__
#define __EDCCODING_H__

#include <string>
#include <opencv2/opencv.hpp>
#include "util/pub.h"

#pragma warning(disable : 4200)

namespace edcc
{

using std::string;
using cv::Mat;
using cv::Size;

class PalmprintCode;

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
    u_int len;
    u_char data[0];
} EDCC_CODING_T;

class EDCCoding
{
public:
    static const size_t kMagicKeyLen = sizeof(int);
    EDCCoding() : coding_buffer_(NULL), magic_key_(0x0622520a) { }
    explicit EDCCoding(const EDCCoding &rhs);
    EDCCoding& operator =(const EDCCoding &rhs);
    virtual ~EDCCoding();
    Status Encode(const EDCC_CFG_T &config,
                  size_t buffer_max_len,
                  u_char *coding_buffer,
                  size_t *buffer_size);
    Status EncodeToHexString(const EDCC_CFG_T &config, string *hex_str);
    Status Decode(const u_char *coding_buffer);
    Status DecodeFromHexString(const string &hex_str);
    size_t buffer_len() const { return coding_buffer_->len + sizeof(EDCC_CODING_T); }
    int magic_key() const { return magic_key_; }
    const EDCC_CODING_T* coding_buffer() const { return coding_buffer_; }
private:
    friend PalmprintCode;
    void GenCodingBytes();
    void MallocCodingBuffer(const EDCC_CFG_T &config, EDCC_CODING_T **buffer);
    void FreeCodingBuffer(EDCC_CODING_T **buffer);
    Status Encode(const EDCC_CFG_T &config, size_t *buffer_size);

    cv::Mat c_;
    cv::Mat cs_;
    EDCC_CODING_T *coding_buffer_;
    int magic_key_;
};

} // namespace edcc

#endif // !__EDCCODING_H__
