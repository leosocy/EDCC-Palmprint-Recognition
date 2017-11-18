#include <ft_get_features.h>

TEST_F(ft_get_features, Given_NULLGroup_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTrainingSetFeatures(NULL,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_NULLConfig_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     NULL,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_NULLOutput_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     NULL);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_Correct_Params_When_Train_Without_Incremental_Then_EDCC_SUCCESS)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
}

TEST_F(ft_get_features, Given_ConfigNotExists_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     NOT_EXISTS_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorImageSize_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams(CORRECT_CONFIG_PATH, "imageSizeW", 11);
    CheckConfigParams(CORRECT_CONFIG_PATH, "imageSizeH", 11);
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorLaplace_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams(CORRECT_CONFIG_PATH, "laplaceKernelSize", 30);
    CheckConfigParams(CORRECT_CONFIG_PATH, "laplaceKernelSize", 30);
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorGaborSize_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams(CORRECT_CONFIG_PATH, "gaborKernelSize", 30);
    CheckConfigParams(CORRECT_CONFIG_PATH, "gaborKernelSize", 30);
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorDirections_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams(CORRECT_CONFIG_PATH, "gaborDirections", 18);
    CheckConfigParams(CORRECT_CONFIG_PATH, "gaborDirections", 18);
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsMiss_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     PARAMS_MISS_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Give_ConfigParamsNoDefault_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     PARAMS_NO_DEFAULT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsOver_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     PARAMS_OVER_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_GroupWrongFormat_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = GetTrainingSetFeatures(WRONG_FORMAT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupNotExists_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = GetTrainingSetFeatures(NOT_EXISTS_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);

    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupConflictImage_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    int ret = GetTrainingSetFeatures(CONFLICT_IMAGE_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);

    EXPECT_EQ(ret, EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupSomeImageError_When_Train_Without_Incremental_Then_EDCC_SUCCESS_And_Count_Correct)
{
    int ret = GetTrainingSetFeatures(SOME_IMAGE_ERROR_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);

    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 5);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 4);
}

TEST_F(ft_get_features, Given_Correct_Params_When_Features_Has_Data_And_Train_Without_Incremental_Then_EDCC_SUCCESS_And_Data_Cover)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);

    ret = GetTrainingSetFeatures(INCREMENTAL_GROUP_PATH,
                                 INCREMENTAL_CONFIG_PATH,
                                 FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, INCREMENTAL_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "3", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}

TEST_F(ft_get_features, Given_OutputCantCreate_When_Train_Without_Incremental_Then_EDCC_SAVE_FEATURES_FAIL)
{
    int ret = GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                     CORRECT_CONFIG_PATH,
                                     FEATURES_OUTPUT_PATH_CANT_CREATE);
    EXPECT_EQ(ret, EDCC_SAVE_FEATURES_FAIL);
}
