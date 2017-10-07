#pragma once

#include <gtest/gtest.h>
#include <json.h>
#include <fstream>
#include <EDCC.h>
using namespace std;

#ifdef _WINDOWS

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

#define ID1_FIRST_PALMPRINT "..\\test\\example\\database\\001\\1_01_s.bmp"
#define ID1_SECOND_PALMPRINT "..\\test\\example\\database\\001\\1_02_s.bmp"
#define ID2_FIRST_PALMPRINT "..\\test\\example\\database\\002\\1_01_s.bmp"
#define ID2_SECOND_PALMPRINT "..\\test\\example\\database\\002\\1_02_s.bmp"

#else

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

#endif

class ft_edcc_base : public testing::Test {
public:
    void CheckFeaturesConfigEqualConfigFile(const char *featuresFileName,
                                            const char *configFileName);
    void CheckOneIdentityImageCountInFeatures(const char *featuresFileName,
                                              const char *identity,
                                              int expectedCount);
};