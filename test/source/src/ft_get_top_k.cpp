#include <ft_get_top_k.h>

void ft_get_top_k::SetUp()
{
    ft_edcc_base::SetUp();

    SetConfigPath(CORRECT_CONFIG_PATH);
    SetGroupPath(CORRECT_GROUP_PATH);
    SetFeaturePath(FEATURES_OUTPUT_PATH);
    SetImagePath(ID1_FIRST_PALMPRINT);
    SetTopKNum(10);
    SetMode(GROUP);
}

void ft_get_top_k::TearDown()
{
    ft_edcc_base::TearDown();
}

void ft_get_top_k::ExcuteInterface()
{
    if(this->mode == GROUP) {
        interRet = GetTopKMatchScore(this->imagePath,
                                     this->groupPath,
                                     this->configPath,
                                     this->mode,
                                     this->topKNum,
                                     &topKResult);
    } else {
        interRet = GetTopKMatchScore(this->imagePath,
                                     this->featurePath,
                                     this->configPath,
                                     this->mode,
                                     this->topKNum,
                                     &topKResult);
    }
    
}

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
    SetImagePath(NULL);

    ExcuteInterface();
    
    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_GroupPathNULL_When_GetTop10MatchScore_Then_EDCC_NULL_POINTER_ERROR)
{
    SetGroupPath(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_ConfigNULL_When_GetTop10MatchScore_Then_EDCC_NULL_POINTER_ERROR)
{
    SetConfigPath(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintNotExists_When_GetTop10MatchScore_Then_EDCC_PALMPRINT_IMAGE_NOT_EXISTS)
{
    SetImagePath(NOT_EXISTS_PALMPRINT);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_PALMPRINT_IMAGE_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupWrongFormat_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(WRONG_FORMAT_GROUP_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupNotExists_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(WRONG_FORMAT_GROUP_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_And_GroupConflictImage_When_GetTop10MatchScore_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(CONFLICT_IMAGE_GROUP_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
    CheckTopKMatchScore(0);
}

TEST_F(ft_get_top_k, Given_PalmprintGroup_When_GetTop10MatchScore_Then_EDCC_SUCCESS)
{
    SetAllParamsCorrect();
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckTopKMatchScore(10);
}

TEST_F(ft_get_top_k, Given_FeaturesNotExists_When_GetTop10MatchScoreUseFeatures_Then_EDCC_LOAD_FEATURES_FAIL)
{
    SetMode(FEATURE);
    SetFeaturePath(FEATURES_OUTPUT_PATH_CANT_CREATE);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_FEATURES_FAIL);
}

TEST_F(ft_get_top_k, Given_FeaturesCorrect_When_GetTop10MatchScoreUseFeatures_Then_EDCC_SUCCESS)
{
    SetMode(FEATURE);
    
    SetFeaturePath(FEATURES_TRAINGING_SET_EXISTS);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckTopKMatchScore(10);
}

TEST_F(ft_get_top_k, Given_CodingInvalid_When_GetTop10MatchScoreUseFeatures_Then_EDCC_LOAD_FEATURES_FAIL)
{
    SetMode(FEATURE);
    SetFeaturePath(FEATURES_TRAINGING_SET_CODING_BEEN_CHANGED);
    
    ExcuteInterface();
    
    CheckInterfaceRet(EDCC_LOAD_FEATURES_FAIL);
}