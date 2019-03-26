#pragma once

#include <ft_edcc_base.h>

class ft_get_edcc_coding : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();
    void SetCodingBuff(unsigned char *pCoding) {
        this->pCoding = pCoding;
    }
    void SetBuffMaxLen(size_t maxLen) {
        this->bufMaxLen = maxLen;
    }
    unsigned char *pCoding;
    size_t bufMaxLen;
    size_t bufLen;

    unsigned char *pCoding1;
    unsigned char *pCoding2;
};