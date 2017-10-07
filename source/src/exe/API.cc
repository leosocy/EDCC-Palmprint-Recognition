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
                          const map< string, int > &configMap);

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &originFeatures,
                                         const vector<PalmprintCode> &incrementalFeatures,
                                         vector<PalmprintCode> &allFeatures);

int EDCC::GetTrainingSetFeatures(const char *trainingSetPalmprintGroupFileName,
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
    Check checkHanler;
    bool bCheckValid = true;
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
        CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_LOAD_FEATURES_FAIL);
    }
    if(!checkHanler.checkConfigValid(trainIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    ifstream trainingSetIn;
    trainingSetIn.open(trainingSetPalmprintGroupFileName);
    retCode = trainIO.loadPalmprintGroup(trainingSetIn, featuresAll);
    if(retCode != EDCC_SUCCESS || !checkHanler.checkPalmprintGroupValid(featuresAll)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    EncodeAllPalmprint(featuresAll, trainIO.configMap);
    if(isIncremental) {
        BuildUpAllFeaturesWhenIncremental(featuresOrigin, featuresAll, featuresAll);
    }
    if(!checkHanler.checkPalmprintFeatureData(featuresAll)) {
        return EDCC_LOAD_FEATURES_FAIL;
    }

    ofstream featuresOutStream;
    featuresOutStream.open(featuresOutputFileName);
    retCode = trainIO.savePalmprintFeatureData(featuresOutStream, featuresAll);
    CHECK_NE_RETURN(retCode, EDCC_SUCCESS, EDCC_SAVE_FEATURES_FAIL);

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
                cout << "--Cover\t" << pcIt->identity << " : " << pcIt->imagePath << endl;
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

int EDCC::GetTwoPalmprintMatchScore(const char *firstPalmprintImagePath,
                                    const char *secondPalmprintImagePath,
                                    const char *configFileName,
                                    double &score)
{
    CHECK_POINTER_NULL_RETURN(firstPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(secondPalmprintImagePath, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);

    return EDCC_SUCCESS;
}

int EDCC::GetTopKMatchScore(const char *onePalmprintImagePath,
                            const char *trainingSetFeaturesOrPalmprintGroupFileName,
                            const char *configFileName,
                            size_t K,
                            map<int, MatchResult> &topKResult)
{
    return EDCC_SUCCESS;
}

int parse_cmd(int argc, const char **argv)
{
    IO trainIO;
    vector<PalmprintCode> data;
    ifstream dataOut;
    dataOut.open( "../example/trainData.json" );
    trainIO.loadPalmprintFeatureData(dataOut, data);
    Check checkHanler;
    bool bValid = true;
    bValid = bValid && checkHanler.checkConfigValid(trainIO.configMap);
    bValid = bValid && checkHanler.checkPalmprintGroupValid(data);
    bValid = bValid && checkHanler.checkPalmprintFeatureData(data);

    if(!bValid) {
        return EXIT_FAILURE;
    }
    for(size_t inner = 0; inner < data.size(); ++inner) {
        double maxScore = -DBL_MAX;
        size_t maxIndex = -1;
        for(size_t outter = 0; outter < data.size(); ++outter) {
            if(data[inner].matchWith(data[outter]) > maxScore && inner != outter) {
                maxScore = data[inner].matchWith(data[outter]);
                maxIndex = outter;
            }
        }
        cout << "--------------------------------------------" << endl;
        cout << "instance_1 : " << data[inner].imagePath << endl << "Match : " << data[maxIndex].imagePath << endl;
        cout << "score: " << maxScore << endl;
        cout << "--------------------------------------------" << endl << endl;
    }

    return 0;
}

