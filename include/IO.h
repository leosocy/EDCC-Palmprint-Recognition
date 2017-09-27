/*************************************************************************
    > File Name: IO.h
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/23 18:02:11
 ************************************************************************/

#ifndef __IO_H__
#define __IO_H__
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <json.h>
#include <Core.h>
using namespace std;
using namespace cv;

#define LOAD_CONFIG_FAILURE 1
#define LOAD_CONFIG_SUCCESS 0

#define LOAD_PALMPRINT_GROUP_FAILURE 1
#define LOAD_PALMPRINT_GROUP_SUCCESS 0

#define LOAD_PALMPRINT_FEATURE_DATA_FAILURE 1
#define LOAD_PALMPRINT_FEATURE_DATA_SUCCESS 0

#define SAVE_PALMPRINT_FEATURE_DATA_FAILURE 1
#define SAVE_PALMPRINT_FEATURE_DATA_SUCCESS 0

namespace EDCC
{
    //=========================================================
    /*
     *Input & Output Module 
     */
    class IO {
        public:
            map<string, int> configMap;
            
            IO();

            int loadConfig(ifstream &in  );
            int loadPalmprintGroup(ifstream &in, vector<PalmprintCode> &groupVec);
            int loadPalmprintFeatureData(ifstream &in, vector<PalmprintCode> &data);
            int savePalmprintFeatureData(ofstream &out, vector<PalmprintCode> &data);
        private:
            set< string > paramsSet;
            int loadOneIdentityAllPalmprintFeatureData(const string &identity, const Json::Value &value, vector<PalmprintCode> &data);			
            void genEDCCoding(const Json::Value &value, PalmprintCode &coding);
            bool insert2JsonValue(PalmprintCode &code, Json::Value &value);
            void setEDCCoding(PalmprintCode &coding, Json::Value &value);
    };
    //=========================================================
}

#endif 

