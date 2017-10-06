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

        int loadConfig(_IN ifstream &in);
        int loadPalmprintGroup(_IN ifstream &in, _OUT vector<PalmprintCode> &groupVec);
        int loadPalmprintFeatureData(_IN ifstream &in, _OUT vector<PalmprintCode> &data);
        int savePalmprintFeatureData(_IN ofstream &out, _IN vector<PalmprintCode> &data);
    private:
        set< string > paramsSet;
        void loadOneIdentityAllPalmprintFeatureData(_IN const string &identity,
                                                    _IN const Json::Value &value,
                                                    _OUT vector<PalmprintCode> &data);
        void genEDCCoding(_IN const Json::Value &value, _OUT PalmprintCode &coding);
        bool insert2JsonValue(_IN PalmprintCode &code, _OUT Json::Value &value);
        void setEDCCoding(_IN PalmprintCode &coding, _OUT Json::Value &value);
    };
    //=========================================================
}

#endif 

