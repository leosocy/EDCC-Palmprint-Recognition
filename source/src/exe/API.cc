/*************************************************************************
    > File Name: palmprint_identify.cc
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/26 21:27:26
 ************************************************************************/
#include <IO.h>
#include <Core.h>
#include <Check.h>
#include <EDCC.h>
using namespace EDCC;

size_t EncodeAllPalmprint(vector<PalmprintCode> &allPalmprint,
                          const map<string, int> &configMap);

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &originFeatures,
                                         const vector<PalmprintCode> &incrementalFeatures,
                                         vector<PalmprintCode> &allFeatures);

bool SortTopK(const PalmprintCode &onePalmrpint,
              const vector<PalmprintCode> &featuresAll,
              size_t k,
              map<size_t, MatchResult> &topKResult);


int GetEDCCCoding(_IN const char *palmprintImagePath,
                  _IN const char *configFileName,
                  _INOUT unsigned char *pCodingBuf,
                  _IN size_t bufMaxLen,
                  _OUT size_t &bufLen)
{
    CHECK_POINTER_NULL_RETURN(palmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    IO trainIO;
    int retCode = 0;
    ifstream configIn;
    Check checkHandler;
    pCodingBuff = NULL;
    buffLen = 0;

    configIn.open(configFileName);
    retCode = trainIO.loadConfig(configIn);
    CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_LOAD_CONFIG_FAIL);
    if(!checkHandler.checkConfigValid(trainIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    PalmprintCode onePalmprint("identity", palmprintImagePath);
    if(!onePalmprint.encodePalmprint(trainIO.configMap)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }
    coding = onePalmprint.encrypt();
    onePalmprint.decrypt(coding);

    return EDCC_SUCCESS;
}

int GetTwoPalmprintCodingMatchScore(_IN const char *firstPalmprintCoding,
                                    _IN const char *secondPalmprintCoding,
                                    _OUT double &score)
{
    return EDCC_SUCCESS;
}

int GetTwoPalmprintMatchScore(_IN const char *firstPalmprintImagePath,
                              _IN const char *secondPalmprintImagePath,
                              _IN const char *configFileName,
                              _OUT double &score)
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(secondPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    IO matchIO;
    int retCode = 0;
    ifstream configIn;
    Check checkHandler;
    score = 0.0;

    configIn.open(configFileName);
    retCode = matchIO.loadConfig(configIn);
    CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_LOAD_CONFIG_FAIL);
    if(!checkHandler.checkConfigValid(matchIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    PalmprintCode firstPalmprint("identity", firstPalmprintImagePath);
    PalmprintCode secondPalmprint("identity", secondPalmprintImagePath);
    if(!firstPalmprint.encodePalmprint(matchIO.configMap)
       || !secondPalmprint.encodePalmprint(matchIO.configMap)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }

    score = firstPalmprint.matchWith(secondPalmprint);

    return EDCC_SUCCESS;
}



int GetTrainingSetFeatures(_IN const char *trainingSetPalmprintGroupFileName,
                           _IN const char *configFileName,
                           _IN const char *featuresOutputFileName,
                           _IN bool isIncremental)
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
        retCode = trainIO.loadConfig(configIn);
        CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_LOAD_CONFIG_FAIL);
    } else {
        ifstream featuresIn;
        featuresIn.open(featuresOutputFileName);
        retCode = trainIO.loadPalmprintFeatureData(featuresIn, featuresOrigin);
        if(retCode != EDCC_SUCCESS
           || !checkHandler.checkPalmprintFeatureData(featuresOrigin, trainIO.configMap)) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    }
    if(!checkHandler.checkConfigValid(trainIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    
    ifstream trainingSetIn;
    trainingSetIn.open(trainingSetPalmprintGroupFileName);
    retCode = trainIO.loadPalmprintGroup(trainingSetIn, featuresAll);
    if(retCode != EDCC_SUCCESS || !checkHandler.checkPalmprintGroupValid(featuresAll)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    
    EncodeAllPalmprint(featuresAll, trainIO.configMap);
    if(isIncremental) {
        BuildUpAllFeaturesWhenIncremental(featuresOrigin, featuresAll, featuresAll);
    }

    ofstream featuresOutStream;
    featuresOutStream.open(featuresOutputFileName);
    retCode = trainIO.savePalmprintFeatureData(featuresOutStream, featuresAll);
    CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_SAVE_FEATURES_FAIL);

    return EDCC_SUCCESS;
}

int GetTopKMatchScore(_IN const char *palmprintImagePath,
                      _IN const char *trainingSetFeaturesOrPalmprintGroupFileName,
                      _IN const char *configFileName,
                      _IN bool isFeatures,
                      _IN size_t K,
                      _OUT std::map<size_t, MatchResult> &topKResult)
{
    CHECK_POINTER_NULL_RETURN(palmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(trainingSetFeaturesOrPalmprintGroupFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    IO matchIO;
    int retCode = 0;
    ifstream featuresOrGroupIn;
    vector<PalmprintCode> featuresAll;
    Check checkHandler;

    featuresOrGroupIn.open(trainingSetFeaturesOrPalmprintGroupFileName);
    if(isFeatures) {
        retCode = matchIO.loadPalmprintFeatureData(featuresOrGroupIn, featuresAll);
        if(!checkHandler.checkPalmprintFeatureData(featuresAll, matchIO.configMap)) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    } else {
        retCode = matchIO.loadPalmprintGroup(featuresOrGroupIn, featuresAll);
        if(retCode != EDCC_SUCCESS
           || !checkHandler.checkPalmprintGroupValid(featuresAll)) {
            return EDCC_LOAD_TAINING_SET_FAIL;
        }

        ifstream configIn;
        configIn.open(configFileName);
        retCode = matchIO.loadConfig(configIn);
    }
    if(retCode != EDCC_SUCCESS
       || !checkHandler.checkConfigValid(matchIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!isFeatures) {
        EncodeAllPalmprint(featuresAll, matchIO.configMap);
    }
    PalmprintCode onePalmprint("identity", palmprintImagePath);
    if(!onePalmprint.encodePalmprint(matchIO.configMap)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }

    SortTopK(onePalmprint, featuresAll, K, topKResult);

    return EDCC_SUCCESS;
}

size_t EncodeAllPalmprint(vector<PalmprintCode> &allPalmprint,
                          const map< string, int > &configMap)
{
    vector<PalmprintCode>::iterator pcIt, pcItTmp;
    for(pcIt = allPalmprint.begin(); pcIt != allPalmprint.end();) {
        bool bRet;
        bRet = pcIt->encodePalmprint(configMap);
        if(!bRet) {
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
            if(pcIt->imagePath == pcItTmp->imagePath
               && pcIt->identity == pcIt->identity) {
                EDCC_Log("----Cover\t%s: %s", pcIt->identity.c_str(), pcIt->imagePath.c_str());
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
        oneResult.identity = featuresAll.at(i).identity;
        oneResult.imagePath = featuresAll.at(i).imagePath;
        oneResult.score = featuresAll.at(i).matchWith(onePalmrpint);
        results.push_back(oneResult);
    }
    sort(results.begin(), results.end(), cmp);

    for(size_t i = 0; i < k && i < featuresAll.size(); ++i) {
        results.at(i).rank = i;
        topKResult.insert(map<size_t, MatchResult>::value_type(i, results.at(i)));
    }

    return true;
}
