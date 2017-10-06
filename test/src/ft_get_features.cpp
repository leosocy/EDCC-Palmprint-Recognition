#include <ft_get_features.h>

TEST_F(ft_get_features, Given_NULL_Group_And_Correct_Config_And_Output_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = EDCC::GetTrainingSetFeatures(NULL,
                                           CORRECT_CONFIG_PATH,
                                           FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_Correct_Group_And_NULL_Config_And_Correct_Output_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = EDCC::GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                           NULL,
                                           FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_Correct_Group_And_Config_And_NULL_Output_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    int ret = EDCC::GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                           CORRECT_CONFIG_PATH,
                                           NULL);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_Correct_Group_And_Config_And_Output_When_Train_Without_Incremental_Then_EDCC_SUCCESS)
{
    int ret = EDCC::GetTrainingSetFeatures(CORRECT_GROUP_PATH,
                                           CORRECT_CONFIG_PATH,
                                           FEATURES_OUTPUT_PATH);
    EXPECT_EQ(ret, EDCC_SUCCESS);
}
