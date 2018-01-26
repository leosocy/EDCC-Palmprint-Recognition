/*************************************************************************
    > File Name: palmprint_identify.cc
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/26 21:27:26
 ************************************************************************/

#include "edcc.h"

#include "io/io.h"
#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"
#include "core/check.h"
#include "util/pub.h"

using namespace std;
using namespace edcc;

#define EDCC_DEFAULT_ID ("defaultID")
#define EDCC_DEFAULT_IMAGEPATH ("defaultImagePath")

size_t EncodeAllPalmprint(vector<PalmprintCode> &allPalmprint,
                          const EDCC_CFG_T &config);

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &originFeatures,
                                         const vector<PalmprintCode> &incrementalFeatures,
                                         vector<PalmprintCode> &allFeatures);

bool SortTopK(const PalmprintCode &onePalmrpint,
              const vector<PalmprintCode> &featuresAll,
              size_t k,
              map<size_t, MatchResult> &topKResult);


int GetEDCCCoding(const char *palmprintImagePath,
                  const char *configFileName,
                  _INOUT unsigned char *pCodingBuf,
                  size_t bufMaxLen,
                  _OUT size_t &bufLen)
{
    IO trainIO;
    ifstream configIn;
    bufLen = 0;
    Status api_ret;

    CHECK_POINTER_NULL_RETURN(palmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(pCodingBuf, EDCC_NULL_POINTER_ERROR);

    configIn.open(configFileName);
    CHECK_FALSE_RETURN(configIn.is_open(), EDCC_LOAD_CONFIG_FAIL);
    if(EDCC_SUCCESS != trainIO.LoadConfig(configIn)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    PalmprintCode onePalmprint(EDCC_DEFAULT_ID, palmprintImagePath);
    api_ret = onePalmprint.EncodeToBuffer(trainIO.config(),
                                          bufMaxLen,
                                          pCodingBuf,
                                          &bufLen);

    return api_ret;
}

int GetTwoPalmprintCodingMatchScore(const unsigned char *firstPalmprintCodingBuf,
                                    const unsigned char *secondPalmprintCodingBuf,
                                    _OUT double &score)
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintCodingBuf, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(secondPalmprintCodingBuf, EDCC_NULL_POINTER_ERROR);

    Status api_ret = 0;
    Check checkHandler;
    score = 0.0;

    PalmprintCode firstPalmprint(EDCC_DEFAULT_ID, EDCC_DEFAULT_IMAGEPATH);
    PalmprintCode secondPalmprint(EDCC_DEFAULT_ID, EDCC_DEFAULT_IMAGEPATH);
    
    api_ret = firstPalmprint.Decode(firstPalmprintCodingBuf);
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);
    api_ret = secondPalmprint.Decode(secondPalmprintCodingBuf);
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);

    if (!checkHandler.CheckTwoPalmprintCodeConfigEqualAndValid(firstPalmprint, secondPalmprint))
    {
        return EDCC_CODINGS_DIFF_CONFIG;
    }

    score = firstPalmprint.MatchWith(secondPalmprint);

    return EDCC_SUCCESS;
}

int GetTwoPalmprintMatchScore(const char *firstPalmprintImagePath,
                              const char *secondPalmprintImagePath,
                              const char *configFileName,
                              _OUT double &score)
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(secondPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    Status api_ret = 0;
    IO matchIO;
    ifstream configIn;
    Check checkHandler;
    score = 0.0;

    configIn.open(configFileName);
    CHECK_FALSE_RETURN(configIn.is_open(), EDCC_LOAD_CONFIG_FAIL);
    if(EDCC_SUCCESS != matchIO.LoadConfig(configIn)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
   
    PalmprintCode firstPalmprint(EDCC_DEFAULT_ID, firstPalmprintImagePath);
    PalmprintCode secondPalmprint(EDCC_DEFAULT_ID, secondPalmprintImagePath);
    api_ret = firstPalmprint.Encode(matchIO.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);
    api_ret = secondPalmprint.Encode(matchIO.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);

    score = firstPalmprint.MatchWith(secondPalmprint);

    return EDCC_SUCCESS;
}



int GetTrainingSetFeatures(const char *trainingSetPalmprintGroupFileName,
                           const char *configFileName,
                           const char *featuresOutputFileName,
                           bool isIncremental)
{
    CHECK_POINTER_NULL_RETURN(trainingSetPalmprintGroupFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(featuresOutputFileName, EDCC_NULL_POINTER_ERROR);

    IO trainIO;
    vector<PalmprintCode> featuresAll;
    vector<PalmprintCode> featuresOrigin;
    Check checkHandler;
    int retCode = 0;

    if(!isIncremental) {
        ifstream configIn;
        configIn.open(configFileName);
        CHECK_FALSE_RETURN(configIn.is_open(), EDCC_LOAD_CONFIG_FAIL);
        retCode = trainIO.LoadConfig(configIn);
        CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_LOAD_CONFIG_FAIL);
    } else {
        ifstream featuresIn;
        featuresIn.open(featuresOutputFileName);
        CHECK_FALSE_RETURN(featuresIn.is_open(), EDCC_LOAD_FEATURES_FAIL);
        retCode = trainIO.LoadPalmprintFeatureData(featuresIn, &featuresOrigin);
        if(retCode != EDCC_SUCCESS
           || !checkHandler.CheckFeatureData(featuresOrigin, trainIO.config())) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    }
    if(!checkHandler.CheckConfig(trainIO.config())) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    
    ifstream trainingSetIn;
    trainingSetIn.open(trainingSetPalmprintGroupFileName);
    CHECK_FALSE_RETURN(trainingSetIn.is_open(), EDCC_LOAD_TAINING_SET_FAIL);
    retCode = trainIO.LoadPalmprintTrainingSet(trainingSetIn, &featuresAll);
    if(retCode != EDCC_SUCCESS 
       || !checkHandler.CheckTrainingSet(featuresAll)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    
    EncodeAllPalmprint(featuresAll, trainIO.config());
    if(isIncremental) {
        BuildUpAllFeaturesWhenIncremental(featuresOrigin, featuresAll, featuresAll);
    }

    ofstream featuresOutStream;
    featuresOutStream.open(featuresOutputFileName);
    CHECK_FALSE_RETURN(featuresOutStream.is_open(), EDCC_SAVE_FEATURES_FAIL);
    retCode = trainIO.SavePalmprintFeatureData(featuresOutStream, featuresAll);
    CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_SAVE_FEATURES_FAIL);

    return EDCC_SUCCESS;
}

int GetTopKMatchScore(const char *palmprintImagePath,
                      const char *trainingSetFeaturesOrPalmprintGroupFileName,
                      const char *configFileName,
                      bool isFeatures,
                      size_t K,
                      _OUT std::map<size_t, MatchResult> &topKResult)
{
    CHECK_POINTER_NULL_RETURN(palmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(trainingSetFeaturesOrPalmprintGroupFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    Status api_ret = 0;
    IO matchIO;
    int retCode = 0;
    ifstream featuresOrGroupIn;
    vector<PalmprintCode> featuresAll;
    Check checkHandler;
    
    featuresOrGroupIn.open(trainingSetFeaturesOrPalmprintGroupFileName);
    CHECK_FALSE_RETURN(featuresOrGroupIn.is_open(), EDCC_LOAD_FEATURES_FAIL);
    if(isFeatures) {
        retCode = matchIO.LoadPalmprintFeatureData(featuresOrGroupIn, &featuresAll);
        if(retCode != EDCC_SUCCESS
           || !checkHandler.CheckFeatureData(featuresAll, matchIO.config())) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    } else {
        retCode = matchIO.LoadPalmprintTrainingSet(featuresOrGroupIn, &featuresAll);
        if(retCode != EDCC_SUCCESS
           || !checkHandler.CheckTrainingSet(featuresAll)) {
            return EDCC_LOAD_TAINING_SET_FAIL;
        }

        ifstream configIn;
        configIn.open(configFileName);
        CHECK_FALSE_RETURN(configIn.is_open(), EDCC_LOAD_CONFIG_FAIL);
        retCode = matchIO.LoadConfig(configIn);
    }
    if(retCode != EDCC_SUCCESS
       || !checkHandler.CheckConfig(matchIO.config())) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!isFeatures) {
        EncodeAllPalmprint(featuresAll, matchIO.config());
    }
    PalmprintCode onePalmprint("identity", palmprintImagePath);
    api_ret = onePalmprint.Encode(matchIO.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);

    SortTopK(onePalmprint, featuresAll, K, topKResult);

    return EDCC_SUCCESS;
}

size_t EncodeAllPalmprint(vector<PalmprintCode> &allPalmprint,
                          const EDCC_CFG_T &config)
{
    vector<PalmprintCode>::iterator pcIt, pcItTmp;
    for(pcIt = allPalmprint.begin(); pcIt != allPalmprint.end();) {
        Status s;
        s = pcIt->Encode(config);
        if(s != EDCC_SUCCESS) {
            pcItTmp = pcIt;
            pcIt = allPalmprint.erase(pcItTmp);
            continue;
        }
        ++pcIt;
    }

    return allPalmprint.size();
}

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &originFeatures,
                                         const vector<PalmprintCode> &incrementalFeatures,
                                         vector<PalmprintCode> &allFeatures)
{
    vector<PalmprintCode>::const_iterator pcIt, pcItTmp;
    allFeatures = incrementalFeatures;

    for(pcIt = originFeatures.begin(); pcIt != originFeatures.end(); ++pcIt) {
        bool isExists = false;
        for(pcItTmp = incrementalFeatures.begin();
            pcItTmp != incrementalFeatures.end();
            ++pcItTmp) {
            if((*pcIt->palmprint()) == (*pcItTmp->palmprint())) {
                EDCC_Log("----Cover\t%s: %s", 
                         pcIt->palmprint()->identity().c_str(), 
                         pcIt->palmprint()->image_path().c_str());
                isExists = true;
                break;
            }
        }
        if(!isExists) {
            allFeatures.push_back(*pcIt);
        }
    }

    return allFeatures.size();
}

bool cmp(const MatchResult &result1, const MatchResult &result2)
{
    return result1.score > result2.score;
}

bool SortTopK(const PalmprintCode &onePalmrpint,
              const vector<PalmprintCode> &featuresAll,
              size_t k,
              map<size_t, MatchResult> &topKResult)
{
    vector<MatchResult> results;

    for(size_t i = 0; i < featuresAll.size(); ++i) {
        MatchResult oneResult;
        oneResult.identity = featuresAll.at(i).palmprint()->identity();
        oneResult.imagePath = featuresAll.at(i).palmprint()->image_path();
        oneResult.score = featuresAll.at(i).MatchWith(onePalmrpint);
        results.push_back(oneResult);
    }
    sort(results.begin(), results.end(), cmp);

    for(size_t i = 0; i < k && i < featuresAll.size(); ++i) {
        results.at(i).rank = i;
        topKResult.insert(map<size_t, MatchResult>::value_type(i, results.at(i)));
    }

    return true;
}
