#include <ft_get_edcc_coding.h>

#define CODING_BUFF_IS_CHANGE memcmp(pCoding, pCoding1, sizeof(unsigned char) * bufMaxLen) != 0

void ft_get_edcc_coding::SetUp()
{
    this->bufMaxLen = 1024 * 16;
    this->pCoding = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    this->pCoding1 = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    memset(pCoding, 0, sizeof(unsigned char) * bufMaxLen);
    memset(pCoding1, 0, sizeof(unsigned char) * bufMaxLen);
}

void ft_get_edcc_coding::TearDown()
{
    this->bufMaxLen = 0;
    if(this->pCoding != NULL) {
        free(this->pCoding);
        this->pCoding = NULL;
    }
    if(this->pCoding1 != NULL) {
        free(this->pCoding1);
        this->pCoding1 = NULL;
    }
}

TEST_F(ft_get_edcc_coding, Given_NULLPalmprintPath_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetEDCCCoding(NULL,
                            CORRECT_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_NULLConfig_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            NULL,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_NULLCodingBuf_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            CORRECT_CONFIG_PATH,
                            NULL,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_One_Palmprint_Not_Exists_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetEDCCCoding(NOT_EXISTS_PALMPRINT,
                            CORRECT_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_PALMPRINT_IMAGE_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigNotExists_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            NOT_EXISTS_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsError_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            PARAMS_ERROR_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsMiss_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            PARAMS_MISS_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Give_ConfigParamsNoDefault_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            PARAMS_NO_DEFAULT_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsOver_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            PARAMS_OVER_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_BufMaxLenNotEnough_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            CORRECT_CONFIG_PATH,
                            pCoding,
                            0,
                            bufLen);
    EXPECT_EQ(ret, EDCC_CODING_BUFF_LEN_NOT_ENOUGH);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_CorrectInputs_When_GetEDCCCoding_Then_EDCC_SUCCESS_And_BufLenNot0)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            CORRECT_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_CorrectInputs_And_TwoDiffPalmprint_When_GetEDCCCoding_Then_EDCC_SUCCESS_And_TwoCodingLenEQ)
{
    int ret = GetEDCCCoding(ID1_FIRST_PALMPRINT,
                            CORRECT_CONFIG_PATH,
                            pCoding,
                            bufMaxLen,
                            bufLen);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen, 0);
    ret = GetEDCCCoding(ID2_FIRST_PALMPRINT,
                        CORRECT_CONFIG_PATH,
                        pCoding1,
                        bufMaxLen,
                        bufLen1);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen1, 0);

    EXPECT_EQ(bufLen, bufLen1);
}