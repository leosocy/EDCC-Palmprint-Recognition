#pragma once

#include <ft_edcc_base.h>

class ft_get_edcc_coding : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();
    unsigned char *pCoding;
    size_t bufMaxLen;
    size_t bufLen;

    unsigned char *pCoding1;
    size_t codingLen1;
    size_t bufLen1;
};