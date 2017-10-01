/*************************************************************************
    > File Name: palmprint_identify.cc
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/26 21:27:26
 ************************************************************************/
#include <EDCC.h>
using namespace EDCC;
int main(int argc, const char **argv)
{
    /*ifstream configIn; 
    configIn.open("config.json");
    IO trainIO;
    trainIO.loadConfig(configIn );

    ifstream groupIn;
    groupIn.open("palmprintTrainGroup.json");
    vector<PalmprintCode> data;
    trainIO.loadPalmprintGroup(groupIn, data);
    for(int index = 0; index < data.size(); ++index) {
        data[index].encodePalmprint(trainIO.paramsMap);
    }*/

    IO trainIO;
    vector<PalmprintCode> data;
    ifstream dataOut;
    dataOut.open( "../trainData.json" );
    trainIO.loadPalmprintFeatureData(dataOut, data);
    Check checkHanler;
    checkHanler.checkValid(trainIO.configMap, data);
    for(size_t index = 0; index < data.size(); ++index) {
        cout << data[index].zipCodingCs << endl;
    }
    return 0;
}

