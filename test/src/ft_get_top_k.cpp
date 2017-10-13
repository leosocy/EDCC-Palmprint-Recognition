#include <ft_get_top_k.h>

void ft_get_top_k::CheckTopKMatchScore(size_t expectK)
{
    EXPECT_EQ(topKResult.size(), expectK);

    if(topKResult.size() == 0) {
        return;
    }

    for(size_t i = 0; i < topKResult.size() && i < expectK; ++i) {
        size_t currectRank = topKResult.at(i).rank;
        EXPECT_EQ(currectRank, i);
    }
    for(size_t i = 0; i < topKResult.size() - 1 && i < expectK - 1; ++i) {
        double scoreCurrent = topKResult.at(i).score;
        double scoreNext = topKResult.at(i + 1).score;

        EXPECT_GE(scoreCurrent, scoreNext);
    }
}

TEST_F(ft_get_top_k, Given_ImagePathNULL_When_GetTop10MatchScore_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTopKMatchScore(NULL,
                                CORRECT_GROUP_PATH,
                                CORRECT_CONFIG_PATH,
                                false,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_GroupPathNULL_When_GetTop10MatchScore_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                NULL,
                                CORRECT_CONFIG_PATH,
                                false,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_ConfigNULL_When_GetTop10MatchScore_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                CORRECT_GROUP_PATH,
                                NULL,
                                false,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintNotExists_When_GetTop10MatchScore_Then_EDCC_PALMPRINT_IMAGE_NOT_EXISTS)
{
    int ret = GetTopKMatchScore(NOT_EXISTS_PALMPRINT,
                                CORRECT_GROUP_PATH,
                                CORRECT_CONFIG_PATH,
                                false,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_PALMPRINT_IMAGE_NOT_EXISTS);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_ConfigNotExists_When_GetTop10MatchScore_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CORRECT_GROUP_PATH,
                                      NOT_EXISTS_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_ConfigParamsError_When_GetTop10MatchScore_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CORRECT_GROUP_PATH,
                                      PARAMS_ERROR_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_ConfigParamsMiss_When_GetTop10MatchScore_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CORRECT_GROUP_PATH,
                                      PARAMS_MISS_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Give_PalmprintGroup_And_ConfigParamsNoDefault_When_GetTop10MatchScore_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CORRECT_GROUP_PATH,
                                      PARAMS_NO_DEFAULT_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_ConfigParamsOver_When_GetTop10MatchScore_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CORRECT_GROUP_PATH,
                                      PARAMS_OVER_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupWrongFormat_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      WRONG_FORMAT_GROUP_PATH,
                                      CORRECT_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupNotExists_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      WRONG_FORMAT_GROUP_PATH,
                                      CORRECT_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupConflictImage_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = EDCC::GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                      CONFLICT_IMAGE_GROUP_PATH,
                                      CORRECT_CONFIG_PATH,
                                      false,
                                      10,
                                      topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_When_GetTop10MatchScore_Then_EDCC_SUCCESS)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                CORRECT_GROUP_PATH,
                                CORRECT_CONFIG_PATH,
                                false,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckTopKMatchScore(10);
}

TEST_F(ft_get_top_k, Given_FeaturesNotExists_When_GetTop10MatchScoreUseFeatures_Then_EDCC_LOAD_FEATURES_FAIL)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                FEATURES_OUTPUT_PATH_CANT_CREATE,
                                CORRECT_CONFIG_PATH,
                                true,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_FEATURES_FAIL);
}

TEST_F(ft_get_top_k, Given_FeaturesCorrect_When_GetTop10MatchScoreUseFeatures_Then_EDCC_SUCCESS)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                FEATURES_TRAINGING_SET_EXISTS,
                                CORRECT_CONFIG_PATH,
                                true,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckTopKMatchScore(10);
}

TEST_F(ft_get_top_k, Given_FeaturesInvalidC_When_GetTop10MatchScoreUseFeatures_Then_EDCC_LOAD_FEATURES_FAIL)
{
    int ret = GetTopKMatchScore(ID1_FIRST_PALMPRINT,
                                FEATURES_TRAINGING_SET_INVALID_C,
                                CORRECT_CONFIG_PATH,
                                true,
                                10,
                                topKResult);
    EXPECT_EQ(ret, EDCC_LOAD_FEATURES_FAIL);
}