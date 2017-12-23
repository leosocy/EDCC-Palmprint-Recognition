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


int GetEDCCCoding(_IN const char *palmprintImagePath,
                  _IN const char *configFileName,
                  _INOUT unsigned char *pCodingBuf,
                  _IN size_t bufMaxLen,
                  _OUT size_t &bufLen)
{
    IO trainIO;
    ifstream configIn;
    Check checkHandler;
    bufLen = 0;

    CHECK_POINTER_NULL_RETURN(palmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(pCodingBuf, EDCC_NULL_POINTER_ERROR);

    configIn.open(configFileName);
    if(EDCC_SUCCESS != trainIO.loadConfig(configIn)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!checkHandler.checkConfigValid(trainIO.config)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    PalmprintCode onePalmprint(EDCC_DEFAULT_ID, palmprintImagePath);
    if(!onePalmprint.encodePalmprint(trainIO.config)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }
    bufLen = onePalmprint.encrypt(pCodingBuf, bufMaxLen, trainIO.config);
    CHECK_EQ_RETURN(bufLen, 0, EDCC_CODING_BUFF_LEN_NOT_ENOUGH);

    return EDCC_SUCCESS;
}

int GetTwoPalmprintCodingMatchScore(_IN const unsigned char *firstPalmprintCodingBuf,
                                    _IN const unsigned char *secondPalmprintCodingBuf,
                                    _OUT double &score)
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintCodingBuf, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(secondPalmprintCodingBuf, EDCC_NULL_POINTER_ERROR);

    int retCode = 0;
    Check checkHandler;
    score = 0.0;

    PalmprintCode firstPalmprint(EDCC_DEFAULT_ID, EDCC_DEFAULT_IMAGEPATH);
    PalmprintCode secondPalmprint(EDCC_DEFAULT_ID, EDCC_DEFAULT_IMAGEPATH);
    
    retCode = firstPalmprint.decrypt(firstPalmprintCodingBuf);
    CHECK_FALSE_RETURN(retCode, EDCC_CODING_INVALID);
    retCode = secondPalmprint.decrypt(secondPalmprintCodingBuf);
    CHECK_FALSE_RETURN(retCode, EDCC_CODING_INVALID);

    retCode = checkHandler.checkTwoPalmprintCodeConfigEQAndValid(firstPalmprint, secondPalmprint);
    CHECK_FALSE_RETURN(retCode, EDCC_CODINGS_DIFF_CONFIG);

    score = firstPalmprint.matchWith(secondPalmprint);

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
    ifstream configIn;
    Check checkHandler;
    score = 0.0;

    configIn.open(configFileName);
    if(EDCC_SUCCESS != matchIO.loadConfig(configIn)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!checkHandler.checkConfigValid(matchIO.config)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
   
    PalmprintCode firstPalmprint(EDCC_DEFAULT_ID, firstPalmprintImagePath);
    PalmprintCode secondPalmprint(EDCC_DEFAULT_ID, secondPalmprintImagePath);
    if(!firstPalmprint.encodePalmprint(matchIO.config)
       || !secondPalmprint.encodePalmprint(matchIO.config)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }
    
    firstPalmprint.encrypt(matchIO.config);
    secondPalmprint.encrypt(matchIO.config);

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
           || !checkHandler.checkPalmprintFeatureData(featuresOrigin, trainIO.config)) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    }
    if(!checkHandler.checkConfigValid(trainIO.config)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    
    ifstream trainingSetIn;
    trainingSetIn.open(trainingSetPalmprintGroupFileName);
    retCode = trainIO.loadPalmprintGroup(trainingSetIn, featuresAll);
    if(retCode != EDCC_SUCCESS 
       || !checkHandler.checkPalmprintGroupValid(featuresAll)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    
    EncodeAllPalmprint(featuresAll, trainIO.config);
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
        if(retCode != EDCC_SUCCESS
           || !checkHandler.checkPalmprintFeatureData(featuresAll, matchIO.config)) {
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
       || !checkHandler.checkConfigValid(matchIO.config)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!isFeatures) {
        EncodeAllPalmprint(featuresAll, matchIO.config);
    }
    PalmprintCode onePalmprint("identity", palmprintImagePath);
    if(!onePalmprint.encodePalmprint(matchIO.config)) {
        return EDCC_LOAD_PALMPRINT_IMAGE_FAIL;
    }
    onePalmprint.encrypt(matchIO.config);

    SortTopK(onePalmprint, featuresAll, K, topKResult);

    return EDCC_SUCCESS;
}

size_t EncodeAllPalmprint(vector<PalmprintCode> &allPalmprint,
                          const EDCC_CFG_T &config)
{
    vector<PalmprintCode>::iterator pcIt, pcItTmp;
    for(pcIt = allPalmprint.begin(); pcIt != allPalmprint.end();) {
        bool bRet;
        bRet = pcIt->encodePalmprint(config);
        if(!bRet) {
            pcItTmp = pcIt;
            pcIt = allPalmprint.erase(pcItTmp);
            continue;
        }
        pcIt->encrypt(config);
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
            if((*pcIt) == (*pcItTmp)) {
                EDCC_Log("----Cover\t%s: %s\n", pcIt->getIdentity().c_str(), pcIt->getImagePath().c_str());
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
        oneResult.identity = featuresAll.at(i).getIdentity();
        oneResult.imagePath = featuresAll.at(i).getImagePath();
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
