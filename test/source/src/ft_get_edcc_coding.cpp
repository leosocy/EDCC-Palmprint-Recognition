#include <ft_get_edcc_coding.h>

#define CODING_BUFF_IS_CHANGE memcmp(pCoding1, pCoding2, sizeof(unsigned char) * bufMaxLen) != 0

void ft_get_edcc_coding::SetUp()
{ 
    SetConfigPath(CORRECT_CONFIG_PATH);
    SetGroupPath(CORRECT_GROUP_PATH);
    SetFeaturePath(FEATURES_OUTPUT_PATH);
    SetImagePath(ID1_FIRST_PALMPRINT);
    this->bufMaxLen = 1024 * 16;
    this->pCoding1 = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    this->pCoding2 = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    memset(pCoding1, 0, sizeof(unsigned char) * bufMaxLen);
    memset(pCoding2, 0, sizeof(unsigned char) * bufMaxLen);

    SetPTCodingBuff(this->pCoding1);
}

void ft_get_edcc_coding::TearDown()
{
    this->bufMaxLen = 0;
    if(this->pCoding1 != NULL) {
        free(this->pCoding);
        this->pCoding1 = NULL;
    }
    if(this->pCoding2 != NULL) {
        free(this->pCoding1);
        this->pCoding2 = NULL;
    }
    this->pCoding = NULL;
}

void ft_get_edcc_coding::ExcuteInterface()
{
    interRet = GetEDCCCoding(this->imagePath,
                             this->configPath,
                             this->pCoding,
                             this->bufMaxLen,
                             this->bufLen);
}

TEST_F(ft_get_edcc_coding, Given_NULLPalmprintPath_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    SetImagePath(NULL);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_NULLConfig_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    SetConfigPath(NULL);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_NULLCodingBuf_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    SetPTCodingBuff(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_One_Palmprint_Not_Exists_When_GetEDCCCoding_Then_EDCC_NULL_POINTER_ERROR)
{
    SetImagePath(NOT_EXISTS_PALMPRINT);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_PALMPRINT_IMAGE_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigNotExists_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(NOT_EXISTS_CONFIG_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsError_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(PARAMS_ERROR_CONFIG_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsMiss_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(PARAMS_MISS_CONFIG_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Give_ConfigParamsNoDefault_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(PARAMS_NO_DEFAULT_CONFIG_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_ConfigParamsOver_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(PARAMS_OVER_CONFIG_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_BufMaxLenNotEnough_When_GetEDCCCoding_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetBuffMaxLen(0);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_CODING_BUFF_LEN_NOT_ENOUGH);
    EXPECT_FALSE(CODING_BUFF_IS_CHANGE);
    EXPECT_EQ(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_CorrectInputs_When_GetEDCCCoding_Then_EDCC_SUCCESS_And_BufLenNot0)
{
    SetAllParamsCorrect();

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen, 0);
}

TEST_F(ft_get_edcc_coding, Given_CorrectInputs_And_TwoDiffPalmprint_When_GetEDCCCoding_Then_EDCC_SUCCESS_And_TwoCodingLenEQ)
{
    size_t bufLenTmp;
    SetPTCodingBuff(pCoding1);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen, 0);
    bufLenTmp = bufLen;

    SetPTCodingBuff(pCoding2);
    SetImagePath(ID2_FIRST_PALMPRINT);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    EXPECT_TRUE(CODING_BUFF_IS_CHANGE);
    EXPECT_NE(bufLen, 0);

    EXPECT_EQ(bufLenTmp, bufLen);
}