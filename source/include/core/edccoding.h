// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __EDCCODING_H__
#define __EDCCODING_H__

#include <string>
#include <opencv2/opencv.hpp>
#include "core/config.h"
#include "util/pub.h"

#pragma warning(disable : 4200)

namespace edcc
{

using std::string;
using cv::Mat;
using cv::Size;

class PalmprintCode;

typedef struct tag_EDCC_CODING_T
{
    EDCC_CFG_T cfg;
    u_int len;
    u_char data[0];
} EDCC_CODING_T;

class EDCCoding
{
public:
    static const size_t kMagicKeyLen = sizeof(int);
    EDCCoding();
    explicit EDCCoding(const EDCCoding &rhs);
    EDCCoding& operator =(const EDCCoding &rhs);
    ~EDCCoding();
    Status Encode(const EDCC_CFG_T &config,
                  size_t buffer_max_len,
                  u_char *coding_buffer,
                  size_t *buffer_size);
    Status EncodeToHexString(const EDCC_CFG_T &config, string *hex_str);
    Status Decode(const u_char *coding_buffer);
    Status DecodeFromHexString(const string &hex_str);
    size_t buffer_len() const { return buffer_->len + sizeof(EDCC_CODING_T); }
    int magic_key() const { return magic_key_; }
    const EDCC_CODING_T* buffer() const { return buffer_; }
private:
    friend PalmprintCode;
    void GenCodingBytes();
    void MallocCodingBuffer(const EDCC_CFG_T &config, EDCC_CODING_T **buffer);
    void FreeCodingBuffer(EDCC_CODING_T **buffer);
    Status Encode(const EDCC_CFG_T &config, size_t *buffer_size);

    cv::Mat c_;
    cv::Mat cs_;
    EDCC_CODING_T *buffer_;
    int magic_key_;
};

} // namespace edcc

#endif // !__EDCCODING_H__
