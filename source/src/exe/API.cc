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

int EDCC::GetTrainingSetFeatures(const char *trainingSetPalmprintGroupFileName,
                                 const char *configFileName,
                                 const char *featuresOutputFileName,
                                 bool isIncremental)
{
    CHECK_POINTER_NULL_RETURN(trainingSetPalmprintGroupFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(configFileName, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(featuresOutputFileName, EDCC_NULL_POINTER_ERROR);

    IO trainIO;
    vector<PalmprintCode> dataAll;
    vector<PalmprintCode> dataIncremental;
    vector<PalmprintCode>::iterator pcIt;
    vector<PalmprintCode>::iterator pcItTmp;
    Check checkHanler;
    bool bCheckValid = true;
    int retCode = 0;

    if(!isIncremental) {
        ifstream configIn;
        configIn.open(configFileName);
        retCode = trainIO.loadConfig(configIn);
        if(retCode != EDCC_SUCCESS) {
            return EDCC_LOAD_CONFIG_FAIL;
        }
    } else {
        ifstream featuresIn;
        featuresIn.open(featuresOutputFileName);
        retCode = trainIO.loadPalmprintFeatureData(featuresIn, dataIncremental);
        if(retCode != EDCC_SUCCESS) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    }
    if(!checkHanler.checkConfigValid(trainIO.configMap)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    ifstream trainingSetIn;
    trainingSetIn.open(trainingSetPalmprintGroupFileName);
    retCode = trainIO.loadPalmprintGroup(trainingSetIn, dataAll);
    if(retCode != EDCC_SUCCESS || !checkHanler.checkPalmprintGroupValid(dataAll)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }

    for(pcIt = dataAll.begin(); pcIt != dataAll.end();) {
        bool bRet;
        bRet = pcIt->encodePalmprint(trainIO.configMap);
        if(!bRet) {
            pcItTmp = pcIt;
            pcIt = dataAll.erase(pcItTmp);
            continue;
        }
        ++pcIt;
    }
    if(isIncremental) {
        for(size_t i = 0; i < dataIncremental.size(); ++i) {
            dataAll.push_back(dataIncremental[i]);
        }
    }
    if(!checkHanler.checkPalmprintFeatureData(dataAll)) {
        return EDCC_LOAD_FEATURES_FAIL;
    }

    ofstream featuresOut;
    featuresOut.open(featuresOutputFileName);
    retCode = trainIO.savePalmprintFeatureData(featuresOut, dataAll);
    if(retCode != EDCC_SUCCESS) {
        return EDCC_SAVE_FEATURES_FAIL;
    }

    return EDCC_SUCCESS;
}

int EDCC::GetTwoPalmprintMatchScore(const char *firstPalmprintImagePath,
                                    const char *secondPalmprintImagePath,
                                    const char *configFileName,
                                    double &score)
{
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

