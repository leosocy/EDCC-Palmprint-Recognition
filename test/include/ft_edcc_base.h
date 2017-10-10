#pragma once

#include <gtest/gtest.h>
#include <json.h>
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

#define FEATURES_OUTPUT_PATH "..\\test\\example\\features\\features.json"
#define FEATURES_INCREMENTAL_OUTPUT_PATH "..\\test\\example\\features\\features_incremental.json"
#define FEATURES_OUTPUT_PATH_CANT_CREATE "..\\test\\example\\features\\folder\\features.json"
#define FEATURES_TRAINGING_SET_EXISTS "..\\test\\example\\features\\trainingFeatures.json"
#define FEATURES_TRAINGING_SET_INVALID_C  "..\\test\\example\\features\\trainingFeaturesInvalidC.json"
#define FEATURES_TRAINGING_SET_INVALID_CS  "..\\test\\example\\features\\trainingFeaturesInvalidCs.json"

#define ID1_FIRST_PALMPRINT "..\\test\\palmprint_database\\001\\2_01_s.bmp"
#define ID1_SECOND_PALMPRINT "..\\test\\palmprint_database\\001\\2_02_s.bmp"
#define ID2_FIRST_PALMPRINT "..\\test\\palmprint_database\\002\\2_01_s.bmp"
#define ID2_SECOND_PALMPRINT "..\\test\\palmprint_database\\002\\2_02_s.bmp"
#define NOT_EXISTS_PALMPRINT "..\\test\\palmprint_database\\002\\not_exists.bmp"

#else

#define EXAMPLE_SRC_DIR "../test/exampleOrigin"
#define EXAMPLE_DST_DIR "../test/example/"

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

#define FEATURES_OUTPUT_PATH "../test/example/features/features.json"
#define FEATURES_INCREMENTAL_OUTPUT_PATH "../test/example/features/features_incremental.json"
#define FEATURES_OUTPUT_PATH_CANT_CREATE "../test/example/features/folder/features_incremental.json"
#define FEATURES_TRAINGING_SET_EXISTS "../test/example/features/trainingFeatures.json"
#define FEATURES_TRAINGING_SET_INVALID_C "../test/example/features/trainingFeaturesInvalidC.json"
#define FEATURES_TRAINGING_SET_INVALID_CS "../test/example/features/trainingFeaturesInvalidCs.json"

#define ID1_FIRST_PALMPRINT "../test/example/database/001/1_01_s.bmp"
#define ID1_SECOND_PALMPRINT "../test/example/database/001/1_02_s.bmp"
#define ID2_FIRST_PALMPRINT "../test/example/database/002/1_01_s.bmp"
#define ID2_SECOND_PALMPRINT "../test/example/database/002/1_02_s.bmp"
#define NOT_EXISTS_PALMPRINT "../test/example/database/002/not_exists.bmp"

#endif

class ft_edcc_base : public testing::Test {
public:
    ft_edcc_base();
    ~ft_edcc_base();

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
private:
    Json::Value* GetJsonValueByConfigParamName(const char *configOrFeaturesFileName,
                                               const char *paramName);
    Json::Value valueRoot;

    void CopyDir(const char *srcDir, const char *dstDir);
    void CopyFile(const char *srcFile, const char *dstFile);

    void DeleteDir(const char *srcDir);
    void DeleteFile(const char *srcFile);
};