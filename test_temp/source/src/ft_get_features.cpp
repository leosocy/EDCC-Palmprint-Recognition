#include <ft_get_features.h>

void ft_get_features::SetUp()
{
    ft_edcc_base::SetUp();

    SetConfigPath(CORRECT_CONFIG_PATH);
    SetGroupPath(CORRECT_GROUP_PATH);
    SetFeaturePath(FEATURES_OUTPUT_PATH);
    SetImagePath(ID1_FIRST_PALMPRINT);
}

void ft_get_features::TearDown()
{
    ft_edcc_base::TearDown();
}

void ft_get_features::ExcuteInterface()
{
    interRet = GetTrainingSetFeatures(this->groupPath,
                                      this->configPath,
                                      this->featurePath);
}

TEST_F(ft_get_features, Given_NULLGroup_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    SetGroupPath(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_NULLConfig_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    SetConfigPath(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_NULLOutput_When_Train_Without_Incremental_Then_EDCC_NULL_POINTER_ERROR)
{
    SetFeaturePath(NULL);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_NULL_POINTER_ERROR);
}

TEST_F(ft_get_features, Given_Correct_Params_When_Train_Without_Incremental_Then_EDCC_SUCCESS)
{
    SetAllParamsCorrect();

    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, CORRECT_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 6);
}

TEST_F(ft_get_features, Given_ConfigNotExists_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    SetConfigPath(NOT_EXISTS_CONFIG_PATH);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorImageSize_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams("imageSizeW", 11);
    CheckConfigParams("imageSizeW", 11);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorLaplace_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams("laplaceKernelSize", 30);
    CheckConfigParams("laplaceKernelSize", 30);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorGaborSize_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams("gaborKernelSize", 30);
    CheckConfigParams("gaborKernelSize", 30);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_ConfigParamsErrorDirections_When_Train_Without_Incremental_Then_EDCC_LOAD_CONFIG_FAIL)
{
    ModifyConfigParams("gaborDirections", 18);
    CheckConfigParams("gaborDirections", 18);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_CONFIG_FAIL);
}

TEST_F(ft_get_features, Given_GroupWrongFormat_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(WRONG_FORMAT_GROUP_PATH);

    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupNotExists_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(NOT_EXISTS_GROUP_PATH);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupConflictImage_When_Train_Without_Incremental_Then_EDCC_LOAD_TAINING_SET_FAIL)
{
    SetGroupPath(CONFLICT_IMAGE_GROUP_PATH);

    ExcuteInterface();
    
    CheckInterfaceRet(EDCC_LOAD_TAINING_SET_FAIL);
}

TEST_F(ft_get_features, Given_GroupSomeImageError_When_Train_Without_Incremental_Then_EDCC_SUCCESS_And_Count_Correct)
{
    SetGroupPath(SOME_IMAGE_ERROR_GROUP_PATH);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SUCCESS);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "1", 5);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "2", 4);
}

TEST_F(ft_get_features, Given_Correct_Params_When_Features_Has_Data_And_Train_Without_Incremental_Then_EDCC_SUCCESS_And_Data_Cover)
{
    SetAllParamsCorrect();
    ExcuteInterface();
    CheckInterfaceRet(EDCC_SUCCESS);
    SetGroupPath(INCREMENTAL_GROUP_PATH);
    SetConfigPath(INCREMENTAL_CONFIG_PATH);

    ExcuteInterface();
    
    CheckInterfaceRet(EDCC_SUCCESS);
    CheckFeaturesConfigEqualConfigFile(FEATURES_OUTPUT_PATH, INCREMENTAL_CONFIG_PATH);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "3", 6);
    CheckOneIdentityImageCountInFeatures(FEATURES_OUTPUT_PATH, "4", 6);
}

TEST_F(ft_get_features, Given_OutputCantCreate_When_Train_Without_Incremental_Then_EDCC_SAVE_FEATURES_FAIL)
{
    SetFeaturePath(FEATURES_OUTPUT_PATH_CANT_CREATE);
    
    ExcuteInterface();

    CheckInterfaceRet(EDCC_SAVE_FEATURES_FAIL);
}
