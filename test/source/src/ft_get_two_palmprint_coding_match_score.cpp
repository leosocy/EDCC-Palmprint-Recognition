#include <ft_get_two_palmprint_coding_match_score.h>

void ft_get_two_palmprint_coding_match_score::SetUp()
{
    ft_edcc_base::SetUp();

    bufMaxLen = 1024 * 16;
    this->firstCodingBuff = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    this->secondCodingBuff = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    memset(firstCodingBuff, 0, sizeof(unsigned char) * bufMaxLen);
    memset(secondCodingBuff, 0, sizeof(unsigned char) * bufMaxLen);

    this->matchScoreWithPalmprint = 0;
    this->matchScoreWithPalmprintCoding = 0;
    this->firstImagePath = NULL;
    this->secondImagePath = NULL;

    SetConfigPath(CORRECT_CONFIG_PATH);
    GenFirstPalmprintCodingBuff(ID1_FIRST_PALMPRINT);
    GenSecondPalmprintCodingBuff(ID1_SECOND_PALMPRINT);
}
void ft_get_two_palmprint_coding_match_score::TearDown()
{
    ft_edcc_base::TearDown();

    FREE_CHAR_ARRAY(this->firstCodingBuff);
    FREE_CHAR_ARRAY(this->secondCodingBuff);
    FREE_CHAR_ARRAY(this->firstImagePath);
    FREE_CHAR_ARRAY(this->secondImagePath);
}

void ft_get_two_palmprint_coding_match_score::ExcuteInterface()
{
    interRet = GetTwoPalmprintCodingMatchScore(this->firstCodingBuff,
                                               this->secondCodingBuff,
                                               &this->matchScoreWithPalmprintCoding);

    int lRet = GetTwoPalmprintMatchScore(this->firstImagePath,
                                         this->secondImagePath,
                                         this->configPath,
                                         &this->matchScoreWithPalmprint);
    EXPECT_EQ(lRet, EDCC_SUCCESS);
}

void ft_get_two_palmprint_coding_match_score::GenFirstPalmprintCodingBuff(const char *firstPalmprintImagePath)
{
    EXPECT_NE((size_t)firstCodingBuff, (size_t)NULL);
    if(firstCodingBuff == NULL) {
        return;
    }
    FREE_CHAR_ARRAY(this->firstImagePath);
    MALLOC_CHAR_ARRAY(this->firstImagePath, firstPalmprintImagePath, PATH_LEN);

    int ret = GetEDCCCoding(this->firstImagePath,
                            this->configPath,
                            this->bufMaxLen,
                            this->firstCodingBuff,
                            &this->bufLen);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_NE(bufLen, 0);
}
void ft_get_two_palmprint_coding_match_score::GenSecondPalmprintCodingBuff(const char *secondPalmprintImagePath)
{
    EXPECT_NE((size_t)secondCodingBuff, (size_t)NULL);
    if(secondCodingBuff == NULL) {
        return;
    }
    FREE_CHAR_ARRAY(this->secondImagePath);
    MALLOC_CHAR_ARRAY(this->secondImagePath, secondPalmprintImagePath, PATH_LEN);

    int ret = GetEDCCCoding(secondPalmprintImagePath,
                            this->configPath,
                            this->bufMaxLen,
                            this->secondCodingBuff,
                            &this->bufLen);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_NE(bufLen, 0);
}

void ft_get_two_palmprint_coding_match_score::SetFirstPalmprintCoding(unsigned char *codingBuff)
{
    if(this->firstCodingBuff != NULL) {
        free(this->firstCodingBuff);
        this->firstCodingBuff = NULL;
    }
    this->firstCodingBuff = codingBuff;
}
void ft_get_two_palmprint_coding_match_score::SetSecondPalmprintCoding(unsigned char *codingBuff)
{
    if(this->secondCodingBuff != NULL) {
        free(this->secondCodingBuff);
        this->secondCodingBuff = NULL;
    }
    this->secondCodingBuff = codingBuff;
}

void ft_get_two_palmprint_coding_match_score::ChangePalmprintCoding()
{
    EXPECT_NE((size_t)secondCodingBuff, (size_t)NULL);
    if(secondCodingBuff == NULL) {
        return;
    }
    memset(secondCodingBuff + bufLen - 8, 0, 8);
}

void ft_get_two_palmprint_coding_match_score::CheckMatchScoreEQ(double expectScore)
{
    EXPECT_EQ(this->matchScoreWithPalmprintCoding, expectScore);
}
void ft_get_two_palmprint_coding_match_score::CheckMatchScoreNE(double expectScore)
{
    EXPECT_NE(this->matchScoreWithPalmprintCoding, expectScore);
}
void ft_get_two_palmprint_coding_match_score::CheckMatchScoreEQToPalmprint()
{
    EXPECT_EQ(this->matchScoreWithPalmprintCoding, this->matchScoreWithPalmprint);
}
void ft_get_two_palmprint_coding_match_score::CheckMatchScoreNEToPalmprint()
{
    EXPECT_NE(this->matchScoreWithPalmprintCoding, this->matchScoreWithPalmprint);
}

TEST_F(ft_get_two_palmprint_coding_match_score, Given_NULL_PalmprintCodingBuff_When_Get_Score_Then_EDCC_NULL_POINTER_ERROR)
{
    SetFirstPalmprintCoding(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_two_palmprint_coding_match_score, Given_Two_Diff_PalmprintCoding_When_Get_Score_Then_EDCC_SUCCESS_And_MatchScoreNE0)
{
    SetAllParamsCorrect();

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckMatchScoreNE(0.0);
    CheckMatchScoreEQToPalmprint();
}

TEST_F(ft_get_two_palmprint_coding_match_score, Given_Two_Same_PalmprintCoding_When_Get_Score_Then_EDCC_SUCCESS_And_MatchScoreEQ1)
{
    GenFirstPalmprintCodingBuff(ID1_FIRST_PALMPRINT);
    GenSecondPalmprintCodingBuff(ID1_FIRST_PALMPRINT);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckMatchScoreEQ(1.0);
    CheckMatchScoreEQToPalmprint();
}

TEST_F(ft_get_two_palmprint_coding_match_score, Given_Two_PalmprintCoding_With_Diff_Config_When_Get_Score_Then_EDCC_CODINGS_DIFF_CONFIG)
{
    GenFirstPalmprintCodingBuff(ID1_FIRST_PALMPRINT);
    SetConfigPath(INCREMENTAL_CONFIG_PATH);
    GenSecondPalmprintCodingBuff(ID1_SECOND_PALMPRINT);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_CODINGS_DIFF_CONFIG);
    CheckMatchScoreEQ(0.0);
}

TEST_F(ft_get_two_palmprint_coding_match_score, Given_PalmprintCoding_Invalid_Config_When_Get_Score_Then_EDCC_CODING_INVALID)
{
    ChangePalmprintCoding();

    ExcuteInterface();

    CheckInterfaceRet(EDCC_CODING_INVALID);
    CheckMatchScoreEQ(0.0);
}