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

    #define EDCC_LOAD_CONFIG_FAIL 100
    #define EDCC_SAVE_CONFIG_FAIL 101

    #define EDCC_LOAD_TAINING_SET_FAIL 200
    #define EDCC_SAVE_TAINING_SET_FAIL 201

    #define EDCC_LOAD_FEATURES_FAIL 300
    #define EDCC_SAVE_FEATURES_FAIL 301

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
