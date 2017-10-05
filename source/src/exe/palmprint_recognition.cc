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

int parse_cmd(int argc, const char **argv)
{
    /*ifstream configIn; 
    configIn.open("../config.json");
    IO trainIO;
    trainIO.loadConfig(configIn);

    ifstream groupIn;
    groupIn.open("../palmprintTrainGroup.json");
    vector<PalmprintCode> data;
    trainIO.loadPalmprintGroup(groupIn, data);
    for(size_t index = 0; index < data.size(); ++index) {
        data[index].encodePalmprint(trainIO.configMap);
    }
    ofstream dataOut;
    dataOut.open("../trainData.json");
    trainIO.savePalmprintFeatureData(dataOut, data);*/

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

