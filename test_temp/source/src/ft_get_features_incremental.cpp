#include <ft_get_features_incremental.h>

void ft_get_features_incremental::SetUp()
{
    ft_edcc_base::SetUp();

    SetConfigPath(CORRECT_CONFIG_PATH);
    SetGroupPath(CORRECT_GROUP_PATH);
    SetFeaturePath(FEATURES_OUTPUT_PATH);
    SetImagePath(ID1_FIRST_PALMPRINT);
    this->bIncremental = false;
}

void ft_get_features_incremental::TearDown()
{
    ft_edcc_base::TearDown();
}

void ft_get_features_incremental::ExcuteInterface()
{
    interRet = GetTrainingSetFeatures(this->groupPath,
                                      this->configPath,
                                      this->featurePath,
                                      this->bIncremental);
}

void ft_get_features_incremental::GenBaseData()
{
    SetAllParamsCorrect();

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
}

TEST_F(ft_get_features_incremental, Given_Features_And_IncrementalGroup_When_Train_With_Incremental_Then_EDCC_SUCCESS_And_Config_Count_Correct)
{
    GenBaseData();
    SetIncremental();
    SetGroupPath(INCREMENTAL_GROUP_PATH);
    SetConfigPath(INCREMENTAL_CONFIG_PATH);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "3", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}

TEST_F(ft_get_features_incremental, Given_Features_And_IncrementalGroup_Has_Cover_When_Train_With_Incremental_Then_EDCC_SUCCESS_And_Config_Count_Correct_And_Features_Cover)
{
    GenBaseData();
    SetIncremental();
    SetGroupPath(COVER_GROUP_PATH);
    SetConfigPath(INCREMENTAL_CONFIG_PATH);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 12);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}

/*TEST_F(ft_get_features_incremental, Given_CodingHasBeenChanged_And_IncrementalGroup_When_Train_With_Incremental_Then_EDCC_LOAD_FEATURES_FAIL)
{
    SetIncremental();
    SetGroupPath(COVER_GROUP_PATH);
    SetConfigPath(INCREMENTAL_CONFIG_PATH);
    SetFeaturePath(FEATURES_TRAINGING_SET_COING_BEEN_CHANGED);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_FEATURES_FAIL);
}*/
