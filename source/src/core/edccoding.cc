// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/edccoding.h"
#include "core/checker.h"
#include "core/matcher.h"
#include "util/status.h"

namespace edcc
{

using namespace std;

EDCCoding::EDCCoding()
    :buffer_(NULL)
{
}

EDCCoding::EDCCoding(const EDCCoding &rhs)
    : c_(rhs.c_.clone()),
    cs_(rhs.cs_.clone()),
    buffer_(NULL)
{
    if (rhs.buffer_ != NULL)
    {
        buffer_ = (EDCC_CODING_T*)malloc(rhs.buffer_len());
        memcpy(buffer_, rhs.buffer_, rhs.buffer_len());
    }
}

EDCCoding& EDCCoding::operator =(const EDCCoding &rhs)
{
    if (this != &rhs)
    {
        c_ = rhs.c_.clone();
        cs_ = rhs.cs_.clone();
        FreeCodingBuffer(&buffer_);
        if (rhs.buffer_ != NULL)
        {
            buffer_ = (EDCC_CODING_T*)malloc(rhs.buffer_len());
            memcpy(buffer_, rhs.buffer_, rhs.buffer_len());
        }
    }
    return *this;
}

EDCCoding::~EDCCoding()
{
    FreeCodingBuffer(&buffer_);
}

Status EDCCoding::EncodeToBuffer(const EDCC_CFG_T &config,
                                 size_t buffer_max_len,
                                 u_char *coding_buffer,
                                 size_t *buffer_size)
{
    if (coding_buffer == NULL)
    {
        *buffer_size = 0;
        return Status::NullPtrError();
    }
    if (buffer_ != NULL)
    {
        if (buffer_max_len < buffer_len())
        {
            EDCC_Log("EDCCoding::encrypt bufMaxLen smaller than the real space occupied!");
            *buffer_size = 0;
            return Status::CodingBufferLenNotEnough();
        }
        memcpy(coding_buffer, buffer_, buffer_len());
        *buffer_size = buffer_len();
        return Status::Ok();
    }

    Status s = Encode(config, buffer_size);
    if (!s.IsOk())
    {
        return s;
    }
    if (buffer_max_len < *buffer_size)
    {
        EDCC_Log("EDCCoding::encrypt bufMaxLen smaller than the real space occupied!");
        *buffer_size = 0;
        return Status::CodingBufferLenNotEnough();
    }
    memcpy(coding_buffer, buffer_, *buffer_size);

    return s;
}

Status EDCCoding::EncodeToHexString(const EDCC_CFG_T &config, string *hex_str)
{
    assert(hex_str);
    size_t buffer_size = 0;
    Status s = Encode(config, &buffer_size);
    if (!s.IsOk())
    {
        hex_str->clear();
        return s;
    }

    size_t pos = 0;
    stringstream hex_string_stream;
    while (pos < buffer_size)
    {
        char hex_c[3];
        sprintf(hex_c, "%02x", ((unsigned char*)buffer_)[pos]);
        hex_string_stream << hex_c;
        ++pos;
    }
    *hex_str = hex_string_stream.str();

    return Status::Ok();
}

Status EDCCoding::Encode(const EDCC_CFG_T &config, size_t *buffer_size)
{
    if (buffer_ != NULL)
    {
        *buffer_size = buffer_len();
        return Status::Ok();
    }
    *buffer_size = CalcCodingBufferSizeByConfig(config);
    MallocCodingBuffer(*buffer_size, &buffer_);
    if (buffer_ == NULL)
    {
        *buffer_size = 0;
        return Status::NullPtrError();
    }
    memcpy(&buffer_->cfg, &config, sizeof(EDCC_CFG_T));
    buffer_->len = *buffer_size - sizeof(EDCC_CODING_T);
    GenCodingBytesProcess(config.codingMode);

    return Status::Ok();
}

Status EDCCoding::DecodeFromBuffer(const u_char *coding_buffer)
{
    CHECK_POINTER_NULL_RETURN(coding_buffer, Status::NullPtrError());

    const EDCC_CODING_T *coding = (EDCC_CODING_T*)coding_buffer;
    if (!Checker::CheckConfig(coding->cfg))
    {
        return Status::CodingInvalid();
    }
    size_t coding_buffer_size = coding->len + sizeof(EDCC_CODING_T);
    MallocCodingBuffer(coding_buffer_size, &buffer_);
    if (buffer_ == NULL)
    {
        EDCC_Log("EDCCoding::decrypt failed!");
        return Status::NullPtrError();
    }
    memcpy(buffer_, coding, coding_buffer_size);

    return Checker::CheckCoding(*this) ? Status::Ok() : Status::CodingInvalid();
}

Status EDCCoding::DecodeFromHexString(const string &hex_str)
{
    size_t coding_len = hex_str.length() / 2;
    if (coding_len == 0)
    {
        return Status::CodingInvalid();
    }
    u_char* coding_buffer = (unsigned char*)malloc(sizeof(unsigned char) * coding_len);
    CHECK_POINTER_NULL_RETURN(coding_buffer, Status::NullPtrError());
    memset(coding_buffer, 0, sizeof(unsigned char) * coding_len);

    for (size_t i = 0; i < coding_len; ++i)
    {
        string hex_c = hex_str.substr(i * 2, 2);
        sscanf(hex_c.c_str(), "%02x", reinterpret_cast<int*>(coding_buffer + i));
    }
    Status s = DecodeFromBuffer(coding_buffer);
    free(coding_buffer);

    return s;
}

void EDCCoding::GenCodingBytesProcess(u_char coding_mode)
{
    switch (coding_mode)
    {
        case COMPRESSION_CODING_MODE:
        {
            GenCodingBytesCompressionMode();
            break;
        }
        case FAST_CODING_MODE:
        {
            GenCodingBytesFastMode();
            break;
        }
        default:
            break;
    }
}

void EDCCoding::GenCodingBytesCompressionMode()
{
    assert(buffer_);
    assert(c_.rows == cs_.rows);
    assert(c_.cols == cs_.cols);
    memset(buffer_->data, 0, buffer_->len);

    size_t offset = 0;
    int counter = 0;
    for (int h = 0; h < c_.rows; ++h)
    {
        for (int w = 0; w < c_.cols; ++w)
        {
            ++counter;
            unsigned char coding_c = c_.at<char>(h, w);
            if (counter % 2 != 0)
            {
                coding_c <<= 4;
                coding_c &= 0xf0;
            }
            *(buffer_->data + offset) |= coding_c;
            if (counter == c_.rows*c_.cols
                || counter % 2 == 0)
            {
                ++offset;
            }
        }
    }

    counter = 0;
    for (int h = 0; h < cs_.rows; ++h)
    {
        for (int w = 0; w < cs_.cols; ++w)
        {
            unsigned char coding_cs = cs_.at<char>(h, w);
            *(buffer_->data + offset) |= (coding_cs << (7 - (counter % 8)));
            ++counter;
            if (counter == cs_.rows*cs_.cols
                || counter % 8 == 0)
            {
                ++offset;
            }
        }
    }
    memcpy(buffer_->data + offset, &kMagicKey, kMagicKeyLen);
}

void EDCCoding::GenCodingBytesFastMode()
{
    assert(buffer_);
    assert(c_.rows == cs_.rows);
    assert(c_.cols == cs_.cols);
    memset(buffer_->data, 0, buffer_->len);
    size_t offset = 0;

    for (int h = 0; h < c_.rows; ++h)
    {
        for (int w = 0; w < c_.cols; ++w)
        {
            unsigned char coding_c = c_.at<char>(h, w);
            unsigned char coding_cs = cs_.at<char>(h, w);
            *(buffer_->data + offset) |= (coding_c << 4);
            *(buffer_->data + offset) |= (coding_cs);
            ++offset;
        }
    }
    memcpy(buffer_->data + offset, &kMagicKey, kMagicKeyLen);
}

size_t EDCCoding::CalcCodingBufferSizeByConfig(const EDCC_CFG_T &config)
{
    size_t buffer_len = 0;
    switch (config.codingMode)
    {
        case COMPRESSION_CODING_MODE:
        {
            size_t image_size = config.imageSizeW*config.imageSizeH;
            size_t c_len = (size_t)ceil(image_size / 2.0);
            size_t cs_len = (size_t)ceil(image_size / 8.0);
            buffer_len = sizeof(EDCC_CODING_T) + c_len + cs_len + kMagicKeyLen;
            break;
        }
        case FAST_CODING_MODE:
        {
            size_t image_size = config.imageSizeW*config.imageSizeH;
            buffer_len = sizeof(EDCC_CODING_T) + image_size + kMagicKeyLen;
            break;
        }
        default:
            break;
    }

    return buffer_len;
}

void EDCCoding::MallocCodingBuffer(size_t buffer_size, EDCC_CODING_T **buffer)
{
    CHECK_EQ_RETURN_VOID(buffer_size, 0);
    FreeCodingBuffer(buffer);

    *buffer = (EDCC_CODING_T *)malloc(buffer_size);
    CHECK_POINTER_NULL_RETURN_VOID(*buffer);
    memset(*buffer, 0, buffer_size);
}

void EDCCoding::FreeCodingBuffer(EDCC_CODING_T **buffer)
{
    if (*buffer != NULL)
    {
        free(*buffer);
        *buffer = NULL;
    }
}

} // namespace edcc
