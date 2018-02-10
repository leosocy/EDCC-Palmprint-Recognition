#pragma once

#include <gtest/gtest.h>
#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <EDCC.h>
using namespace std;

#ifdef _WINDOWS

#define EXAMPLE_SRC_DIR                 "..\\test\\exampleOrigin"
#define EXAMPLE_DST_DIR                 "..\\test\\example\\"
#define TEST_DATA_DIR                   EXAMPLE_DST_DIR
#define TEST_DATA_CONFIG_DIR            TEST_DATA_DIR"config\\"
#define TEST_DATA_GROUP_DIR             TEST_DATA_DIR"group\\windows\\"
#define TEST_DATA_FEATURES_DIR          TEST_DATA_DIR"features\\windows\\"
#define TEST_DATA_PALMPRINT_IMAGE_DIR   "..\\test\\palmprint_database\\"

#else

#define EXAMPLE_SRC_DIR                 "../test/exampleOrigin"
#define EXAMPLE_DST_DIR                 "../test/example"
#define TEST_DATA_DIR                   EXAMPLE_DST_DIR"/"
#define TEST_DATA_CONFIG_DIR            TEST_DATA_DIR"config/"
#define TEST_DATA_GROUP_DIR             TEST_DATA_DIR"group/linux/"
#define TEST_DATA_FEATURES_DIR          TEST_DATA_DIR"features/linux/"
#define TEST_DATA_PALMPRINT_IMAGE_DIR   "../test/palmprint_database/"

#endif

#define CORRECT_CONFIG_PATH             TEST_DATA_CONFIG_DIR"config_correct.json"
#define INCREMENTAL_CONFIG_PATH         TEST_DATA_CONFIG_DIR"config_incremental.json"
#define NOT_EXISTS_CONFIG_PATH          TEST_DATA_CONFIG_DIR"config_not_exists.json"

#define CORRECT_GROUP_PATH              TEST_DATA_GROUP_DIR"trainingSetGroup.json"
#define INCREMENTAL_GROUP_PATH          TEST_DATA_GROUP_DIR"trainingSetGroupIncremental.json"
#define COVER_GROUP_PATH                TEST_DATA_GROUP_DIR"trainingSetGroupCover.json"
#define NOT_EXISTS_GROUP_PATH           TEST_DATA_GROUP_DIR"trainingSetGroupNotExists.json"
#define WRONG_FORMAT_GROUP_PATH         TEST_DATA_GROUP_DIR"trainingSetGroup_WrongFormat.json"
#define CONFLICT_IMAGE_GROUP_PATH       TEST_DATA_GROUP_DIR"trainingSetGroup_ConflictImage.json"
#define SOME_IMAGE_ERROR_GROUP_PATH     TEST_DATA_GROUP_DIR"trainingSetGroupSomeImageError.json"

#define FEATURES_OUTPUT_PATH                        TEST_DATA_FEATURES_DIR"features.json"
#define FEATURES_INCREMENTAL_OUTPUT_PATH            TEST_DATA_FEATURES_DIR"features_incremental.json"
#define FEATURES_OUTPUT_PATH_CANT_CREATE            TEST_DATA_FEATURES_DIR"folder/features.json"
#define FEATURES_TRAINGING_SET_EXISTS               TEST_DATA_FEATURES_DIR"trainingFeatures.json"
#define FEATURES_TRAINGING_SET_COING_BEEN_CHANGED   TEST_DATA_FEATURES_DIR"trainingFeaturesBeenChanged.json"

#define ID1_FIRST_PALMPRINT             TEST_DATA_PALMPRINT_IMAGE_DIR"001/2_01_s.bmp"
#define ID1_SECOND_PALMPRINT            TEST_DATA_PALMPRINT_IMAGE_DIR"001/2_02_s.bmp"
#define ID2_FIRST_PALMPRINT             TEST_DATA_PALMPRINT_IMAGE_DIR"002/2_01_s.bmp"
#define ID2_SECOND_PALMPRINT            TEST_DATA_PALMPRINT_IMAGE_DIR"002/2_02_s.bmp"
#define NOT_EXISTS_PALMPRINT            TEST_DATA_PALMPRINT_IMAGE_DIR"002/not_exists.bmp"

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

    virtual void SetConfigPath(const char *configPath) {
        FREE_CHAR_ARRAY(this->configPath);
        MALLOC_CHAR_ARRAY(this->configPath, configPath, PATH_LEN);
    }
    virtual void SetGroupPath(const char *groupPath) {
        FREE_CHAR_ARRAY(this->groupPath);
        MALLOC_CHAR_ARRAY(this->groupPath, groupPath, PATH_LEN);
    }
    virtual void SetFeaturePath(const char *featurePath) {
        FREE_CHAR_ARRAY(this->featurePath);
        MALLOC_CHAR_ARRAY(this->featurePath, featurePath, PATH_LEN);
    }
    virtual void SetImagePath(const char *imagePath) {
        FREE_CHAR_ARRAY(this->imagePath);
        MALLOC_CHAR_ARRAY(this->imagePath, imagePath, PATH_LEN);
    }
    virtual void SetCodingModeCompression() {
        ModifyConfigParams("codingMode", 1);
    }
    virtual void SetCodingModeFast() {
        ModifyConfigParams("codingMode", 2);
    }
    virtual void SetMatchingModeReliable() {
        ModifyConfigParams("matchingMode", 1);
    }
    virtual void SetMatchingModeSpeed() {
        ModifyConfigParams("matchingMode", 2);
    }
    virtual void SetAllParamsCorrect() {
        //Do nothing
    }

    virtual void ExcuteInterface() = 0;
    void CheckInterfaceRet(int expectRet);


    void CheckFeaturesConfigEqualConfigFile(const char *featuresFileName,
                                            const char *configFileName);

    void CheckOneIdentityImageCountInFeatures(const char *featuresFileName,
                                              const char *identity,
                                              size_t expectedCount);

    void AppendConfigParam(const char *key,
                           int value);
    void RemoveConfigParam(const char *paramName);
    void ModifyConfigParams(const char *paramName,
                            int valueToSet);

    void CheckConfigParams(const char *paramName,
                           int expectValue);
    void CheckConfigParamExists(const char *paramName);
    void CheckConfigParamNotExists(const char *paramName);
protected:
    char *configPath;
    char *groupPath;
    char *featurePath;
    char *imagePath;
    int interRet;
private:
    void ParseJsonFile(const char *configOrFeaturesFileNam);
    Json::Value* GetJsonValueByConfigParamName(const char *configOrFeaturesFileName,
                                               const char *paramName);

    Json::Value valueRoot;

    void CopyDir(const char *srcDir, const char *dstDir);
    void CopyFile(const char *srcFile, const char *dstFile);

    void DeleteDir(const char *srcDir);
    void DeleteFile(const char *srcFile);
};