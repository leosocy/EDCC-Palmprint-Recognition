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
#include <EDCC.h>
using namespace std;
using namespace cv;

#define LOAD_PALMPRINT_GROUP_FAILURE 1
#define LOAD_PALMPRINT_GROUP_SUCCESS 0

#define LOAD_PALMPRINT_FEATURE_DATA_FAILURE 2
#define LOAD_PALMPRINT_FEATURE_DATA_SUCCESS 0

#define SAVE_PALMPRINT_FEATURE_DATA_FAILURE 3
#define SAVE_PALMPRINT_FEATURE_DATA_SUCCESS 0

namespace EDCC
{
    //=========================================================
    /*
     *Input & Output Module 
     */
    class IO {
    public:
        IO();
        ~IO();

        EDCC_CFG_T config;

        int loadConfig(_IN ifstream &in);
        int loadPalmprintGroup(_IN ifstream &in, _INOUT vector<PalmprintCode> &groupVec);
        int loadPalmprintFeatureData(_IN ifstream &in, _INOUT vector<PalmprintCode> &data);
        int savePalmprintFeatureData(_IN ofstream &out, _IN vector<PalmprintCode> &data);
    private:
        set<string> paramsSet;
        bool genConfig(const string &configKey, int configValue);
        bool getConfig(const string &configKey, int &configValue);
        bool loadOneIdentityAllPalmprintFeatureData(_IN const string &identity,
                                                    _IN const Json::Value &value,
                                                    _INOUT vector<PalmprintCode> &data);
        bool getEDCCoding(_IN const Json::Value &value, _INOUT PalmprintCode &coding);
        bool insert2JsonValue(_IN PalmprintCode &code, _INOUT Json::Value &value);
        bool setEDCCoding(_IN PalmprintCode &coding, _INOUT Json::Value &value);
    };
    //=========================================================
}

#endif 

