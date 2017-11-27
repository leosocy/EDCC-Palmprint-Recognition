#pragma once

#include <gtest/gtest.h>
#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <EDCC.h>
using namespace std;

#ifdef _WINDOWS

#define EXAMPLE_SRC_DIR "..\\test\\exampleOrigin"
#define EXAMPLE_DST_DIR "..\\test\\example\\"

#define CORRECT_CONFIG_PATH "..\\test\\example\\config\\config_correct.json"
#define INCREMENTAL_CONFIG_PATH "..\\test\\example\\config\\config_incremental.json"
#define NOT_EXISTS_CONFIG_PATH "..\\test\\example\\config\\config.json"
#define PARAMS_ERROR_CONFIG_PATH "..\\test\\example\\config\\config_params_error.json"
#define PARAMS_MISS_CONFIG_PATH "..\\test\\example\\config\\config_params_miss.json"
#define PARAMS_OVER_CONFIG_PATH "..\\test\\example\\config\\config_params_over.json"
#define PARAMS_NO_DEFAULT_CONFIG_PATH "..\\test\\example\\config\\config_params_no_default.json"

#define CORRECT_GROUP_PATH "..\\test\\example\\group\\trainingSetGroup_Windows.json"
#define INCREMENTAL_GROUP_PATH "..\\test\\example\\group\\trainingSetGroupIncremental_Windows.json"
#define COVER_GROUP_PATH  "..\\test\\example\\group\\trainingSetGroupCover_Windows.json"
#define NOT_EXISTS_GROUP_PATH "..\\test\\example\\group\\trainingSetGroupNOT_Windows.json"
#define WRONG_FORMAT_GROUP_PATH "..\\test\\example\\group\\trainingSetGroup_WrongFormat_Windows.json"
#define CONFLICT_IMAGE_GROUP_PATH "..\\test\\example\\group\\trainingSetGroup_ConflictImage_Windows.json"
#define SOME_IMAGE_ERROR_GROUP_PATH "..\\test\\example\\group\\trainingSetGroupSomeImageError_Windows.json"

#define FEATURES_OUTPUT_PATH "..\\test\\example\\features\\features_Windows.json"
#define FEATURES_INCREMENTAL_OUTPUT_PATH "..\\test\\example\\features\\features_incremental_Windows.json"
#define FEATURES_OUTPUT_PATH_CANT_CREATE "..\\test\\example\\features\\folder\\features_Windows.json"
#define FEATURES_TRAINGING_SET_EXISTS "..\\test\\example\\features\\trainingFeatures_Windows.json"
#define FEATURES_TRAINGING_SET_COING_BEEN_CHANGED  "..\\test\\example\\features\\trainingFeaturesBeenChanged_Windows.json"

#define ID1_FIRST_PALMPRINT "..\\test\\palmprint_database\\001\\2_01_s.bmp"
#define ID1_SECOND_PALMPRINT "..\\test\\palmprint_database\\001\\2_02_s.bmp"
#define ID2_FIRST_PALMPRINT "..\\test\\palmprint_database\\002\\2_01_s.bmp"
#define ID2_SECOND_PALMPRINT "..\\test\\palmprint_database\\002\\2_02_s.bmp"
#define NOT_EXISTS_PALMPRINT "..\\test\\palmprint_database\\002\\not_exists.bmp"

#else

#define EXAMPLE_SRC_DIR "../test/exampleOrigin"
#define EXAMPLE_DST_DIR "../test/example"

#define CORRECT_CONFIG_PATH "../test/example/config/config_correct.json"
#define INCREMENTAL_CONFIG_PATH  "../test/example/config/config_incremental.json"
#define NOT_EXISTS_CONFIG_PATH "../test/example/config/config.json"
#define PARAMS_ERROR_CONFIG_PATH "../test/example/config/config_params_error.json"
#define PARAMS_MISS_CONFIG_PATH "../test/example/config/config_params_miss.json"
#define PARAMS_OVER_CONFIG_PATH "../test/example/config/config_params_over.json"
#define PARAMS_NO_DEFAULT_CONFIG_PATH "../test/example/config/config_params_no_default.json"

#define CORRECT_GROUP_PATH "../test/example/group/trainingSetGroup_Linux.json"
#define INCREMENTAL_GROUP_PATH  "../test/example/group/trainingSetGroupIncremental_Linux.json"
#define COVER_GROUP_PATH  "../test/example/group/trainingSetGroupCover_Linux.json"
#define NOT_EXISTS_GROUP_PATH "../test/example/group/trainingSetGroupNOT_Linux.json"
#define WRONG_FORMAT_GROUP_PATH "../test/example/group/trainingSetGroup_WrongFormat_Linux.json"
#define CONFLICT_IMAGE_GROUP_PATH "../test/example/group/trainingSetGroup_ConflictImage_Linux.json"
#define SOME_IMAGE_ERROR_GROUP_PATH "../test/example/group/trainingSetGroupSomeImageError_Linux.json"

#define FEATURES_OUTPUT_PATH "../test/example/features/features_Linux.json"
#define FEATURES_INCREMENTAL_OUTPUT_PATH "../test/example/features/features_incremental_Linux.json"
#define FEATURES_OUTPUT_PATH_CANT_CREATE "../test/example/features/folder/features_incremental_Linux.json"
#define FEATURES_TRAINGING_SET_EXISTS "../test/example/features/trainingFeatures_Linux.json"
#define FEATURES_TRAINGING_SET_COING_BEEN_CHANGED "../test/example/features/trainingFeaturesBeenChanged_Linux.json"

#define ID1_FIRST_PALMPRINT "../test/palmprint_database/001/1_01_s.bmp"
#define ID1_SECOND_PALMPRINT "../test/palmprint_database/001/1_02_s.bmp"
#define ID2_FIRST_PALMPRINT "../test/palmprint_database/002/1_01_s.bmp"
#define ID2_SECOND_PALMPRINT "../test/palmprint_database/002/1_02_s.bmp"
#define NOT_EXISTS_PALMPRINT "../test/palmprint_database/002/not_exists.bmp"

#endif

#define PATH_LEN 128
#define FREE_CHAR_ARRAY(pArray) do {\
    if((pArray) != NULL) {\
        free((pArray));\
        (pArray) = NULL;\
    }\
}while(0)
#define MALLOC_CHAR_ARRAY(pDstArray, pSrcArray, len) do {\
    if((pSrcArray) == NULL) {\
        (pDstArray) = NULL;\
        return;\
    }\
    (pDstArray) = (char*)malloc((len) * sizeof(char));\
    strncpy((pDstArray), (pSrcArray), (len));\
}while(0)

class ft_edcc_base : public testing::Test {
public:
    ft_edcc_base();
    virtual ~ft_edcc_base();

    virtual void SetUp();
    virtual void TearDown();

    void SetConfigPath(const char *configPath) {
        FREE_CHAR_ARRAY(this->configPath);
        MALLOC_CHAR_ARRAY(this->configPath, configPath, PATH_LEN);
    }
    void SetGroupPath(const char *groupPath) {
        FREE_CHAR_ARRAY(this->groupPath);
        MALLOC_CHAR_ARRAY(this->groupPath, groupPath, PATH_LEN);
    }
    void SetFeaturePath(const char *featurePath) {
        FREE_CHAR_ARRAY(this->featurePath);
        MALLOC_CHAR_ARRAY(this->featurePath, featurePath, PATH_LEN);
    }
    void SetImagePath(const char *imagePath) { 
        FREE_CHAR_ARRAY(this->imagePath);
        MALLOC_CHAR_ARRAY(this->imagePath, imagePath, PATH_LEN);
    }
    void SetAllParamsCorrect() {
        //Do nothing
    }

    virtual void ExcuteInterface() = 0;
    void CheckInterfaceRet(int expectRet);


    void CheckFeaturesConfigEqualConfigFile(const char *featuresFileName,
                                            const char *configFileName);

    void CheckOneIdentityImageCountInFeatures(const char *featuresFileName,
                                              const char *identity,
                                              int expectedCount);

    void ModifyConfigParams(const char *configOrFeaturesFileName,
                            const char *paramName,
                            int valueToSet);

    void CheckConfigParams(const char *configOrFeaturesFileName,
                           const char *paramName,
                           int expectValue);
protected:
    char *configPath;
    char *groupPath;
    char *featurePath;
    char *imagePath;
    int interRet;
private:
    Json::Value* GetJsonValueByConfigParamName(const char *configOrFeaturesFileName,
                                               const char *paramName);

    Json::Value valueRoot;

    void CopyDir(const char *srcDir, const char *dstDir);
    void CopyFile(const char *srcFile, const char *dstFile);

    void DeleteDir(const char *srcDir);
    void DeleteFile(const char *srcFile);
};