#include <ft_get_two_palmprint_match_score.h>

TEST_F(ft_get_two_palmprint_match_score, Given_Some_Param_NULL_When_Get_Score_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(NULL,
                                              ID1_SECOND_PALMPRINT,
                                              CORRECT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_two_palmprint_match_score, Given_ConfigNotExists_When_Get_Score_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              NOT_EXISTS_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_two_palmprint_match_score, Given_ConfigParamsError_When_Get_Score_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              PARAMS_ERROR_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_two_palmprint_match_score, Given_ConfigParamsMiss_When_Get_Score_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              PARAMS_MISS_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_two_palmprint_match_score, Give_ConfigParamsNoDefault_When_Get_Score_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              PARAMS_NO_DEFAULT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_two_palmprint_match_score, Given_ConfigParamsOver_When_Get_Score_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              PARAMS_OVER_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_two_palmprint_match_score, Given_One_Palmprint_Not_Exists_When_Get_Score_Then_EDCC_PALMPRINT_IMAGE_NOT_EXISTS)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              NOT_EXISTS_PALMPRINT,
                                              CORRECT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_PALMPRINT_IMAGE_NOT_EXISTS);
}

TEST_F(ft_get_two_palmprint_match_score, Given_Two_Same_Image_When_Get_Score_Then_EDCC_SUCCESS_And_MatchScore_EQ1)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_FIRST_PALMPRINT,
                                              CORRECT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_EQ(matchScore, 1.0);
}

TEST_F(ft_get_two_palmprint_match_score, Given_Two_Diff_Image_When_Get_Score_Then_EDCC_SUCCESS_And_MatchScore_NE1)
{
    int ret = EDCC::GetTwoPalmprintMatchScore(ID1_FIRST_PALMPRINT,
                                              ID1_SECOND_PALMPRINT,
                                              CORRECT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    EXPECT_NE(matchScore, 1.0);
}