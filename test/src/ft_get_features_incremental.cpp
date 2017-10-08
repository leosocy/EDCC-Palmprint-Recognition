#include <ft_get_features_incremental.h>

TEST_F(ft_get_features_incremental, Given_Features_And_IncrementalGroup_When_Train_With_Incremental_Then_EDCC_SUCCESS_And_Config_Count_Correct)
{
    int ret = EDCC::GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                           CORRECT_CONFIG_PATH,
                                           FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);

    ret = EDCC::GetTrainingSetFeatures(INCREMENTAL_GROUP_PATH,
                                       INCREMENTAL_CONFIG_PATH,
                                       FEATURES_OUTPUT_PATH,
                                       true);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "3", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}

TEST_F(ft_get_features_incremental, Given_Features_And_IncrementalGroup_Has_Cover_When_Train_With_Incremental_Then_EDCC_SUCCESS_And_Config_Count_Correct_And_Features_Cover)
{
    int ret = EDCC::GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                           CORRECT_CONFIG_PATH,
                                           FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);

    ret = EDCC::GetTrainingSetFeatures(COVER_GROUP_PATH,
                                       INCREMENTAL_CONFIG_PATH,
                                       FEATURES_OUTPUT_PATH,
                                       true);
    EXPECT_EQ(ret, EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 12);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}