/*************************************************************************
	> File Name: EDCC.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/30 14:32:50
 ************************************************************************/
#ifndef __EDCC_H__
#define __EDCC_H__

#include <string>
#include <map>
#include <vector>
using namespace std;

namespace EDCC {
    #define EDCC_SUCCESS 0
    #define EDCC_NULL_POINTER_ERROR 1

    #define EDCC_FAIL_CODE_CONFIG_BASE 100
    #define EDCC_CONFIG_PARAMS_ERROR (EDCC_FAIL_CODE_CONFIG_BASE + 1)
    #define EDCC_CONFIG_PARAMS_MISS (EDCC_FAIL_CODE_CONFIG_BASE + 2)
    #define EDCC_CONFIG_PARAMS_OVER (EDCC_FAIL_CODE_CONFIG_BASE + 3)
    #define EDCC_CONFIG_PARAMS_NO_DEFAULT (EDCC_FAIL_CODE_CONFIG_BASE + 4)

    #define EDCC_FAIL_CODE_TAINING_SET_BASE 200

    typedef struct  {
        string identity;
        string imagePath;
        double score;
        int rank;
    } MatchResult;

    int GetTrainingSetFeatures(const char *trainingSetPalmprintGroupFileName,
                               const char *configFileName,
                               const char *featuresOutputFileName,
                               bool isIncremental = false);

    int GetTwoPalmprintMatchScore(const char *firstPalmprintImagePath,
                                  const char *secondPalmprintImagePath,
                                  const char *configFileName,
                                  double &score);

    int GetTopKMatchScore(const char *onePalmprintImagePath,
                          const char *trainingSetFeaturesOrPalmprintGroupFileName,
                          size_t K,
                          map<int, MatchResult> &topKResult);
}


#endif
