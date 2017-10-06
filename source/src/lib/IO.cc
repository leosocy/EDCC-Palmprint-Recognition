/*************************************************************************
    > File Name: IO..cc
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/26 21:29:10
 ************************************************************************/
#include <IO.h>
#include <iostream>
#include <assert.h>

#define PALMPRINT_GROUP_FORMAT  "{\n\
\"identity\" : [\n\
    \"path1\"\n\
    \"path2\"\n\
    ]\n\
}"
EDCC::IO::IO()
{
    paramsSet.insert("imageSize");
    paramsSet.insert("laplaceKernelSize");
    paramsSet.insert("gaborKernelSize");
    paramsSet.insert("gaborDirections");
}

int EDCC::IO::loadConfig(_IN ifstream &in)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if(!reader.parse(in, root)) {
        cerr << "Parse config.json failed, please confirm the format." << endl;
        return LOAD_CONFIG_FAILURE;
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); 
            it != members.end(); ++it) {
        if(root[*it]["default"].isNull()) {
            cerr << "Parse config.json failed, you can only change the value\
 of \"default\" label in this file." << endl;
            return LOAD_CONFIG_FAILURE;
        } else {
            if(paramsSet.find(*it) != paramsSet.end()) {
                configMap.insert(map<string, int>::value_type(*it, root[*it]["default"].asInt())); 
            } else {
                cerr << "Illegal configuration parameters." << endl;
                return LOAD_CONFIG_FAILURE;
            }
        }
    }

    return LOAD_CONFIG_SUCCESS;
}

int EDCC::IO::loadPalmprintGroup(_IN ifstream &in, _OUT vector<PalmprintCode> &groupVec)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;

    if(!reader.parse(in, root)) {
        cerr << "Parse json failed. Don't change the json format. You need to confirm the format like this." << endl;
        cerr << PALMPRINT_GROUP_FORMAT << endl;
        return LOAD_PALMPRINT_GROUP_FAILURE;
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); 
            it != members.end(); ++it) {
        if(!root[*it].isArray()) {
            cerr << "Don't change the json format. You need to confirm the format like this." << endl;
            cerr << PALMPRINT_GROUP_FORMAT << endl;
            return LOAD_PALMPRINT_GROUP_FAILURE;
        }
        Json::Value imageList = root[*it];
        for(size_t imageIndex = 0; imageIndex < imageList.size(); ++imageIndex) {
            PalmprintCode newOne((*it).c_str(), imageList[(unsigned)imageIndex].asString().c_str());
            groupVec.push_back(newOne);
        }
    }

    return LOAD_PALMPRINT_GROUP_SUCCESS;
}

int EDCC::IO::loadPalmprintFeatureData(_IN ifstream &in, _OUT vector<PalmprintCode> &data)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if(!reader.parse(in, root)) {
        cerr << "Parse json failed. Don't change the trainData.json format." << endl;
        return LOAD_PALMPRINT_FEATURE_DATA_FAILURE;
    }
    for(set<string>::iterator it = paramsSet.begin(); it != paramsSet.end(); ++it) {
        if(root[*it].isNull() || !root[*it].isInt()) {
            cerr << "Parse json failed. Don't change the trainData.json format." << endl;
            return LOAD_PALMPRINT_FEATURE_DATA_FAILURE;
        }
        configMap.insert(map<string, int>::value_type(*it, root[*it].asInt())); 
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); 
            it != members.end(); ++it) {
        if(paramsSet.find(*it) == paramsSet.end()) {
            loadOneIdentityAllPalmprintFeatureData(*it, root[*it], data);
        }
    }

    return LOAD_PALMPRINT_FEATURE_DATA_SUCCESS;
}

int EDCC::IO::savePalmprintFeatureData(_IN ofstream &out, _IN vector<PalmprintCode> &data)
{
    assert(out.is_open());
    Json::Value root;
    for(set<string>::iterator it = paramsSet.begin(); it != paramsSet.end(); ++it) {
        if(configMap.find(*it) == configMap.end()) {
            cerr << "If you want to train/predict, load config.json first.Or if you want incremental training/prediction, load trainData.json first." << endl;
            return SAVE_PALMPRINT_FEATURE_DATA_FAILURE;
        } else {
            root[*it] = configMap.at(*it);
        }
    }

    for(vector<PalmprintCode>::iterator it = data.begin(); it != data.end(); ++it) {
        insert2JsonValue(*it, root);
    }
    out << root.toStyledString();

    return SAVE_PALMPRINT_FEATURE_DATA_SUCCESS;
}

void EDCC::IO::loadOneIdentityAllPalmprintFeatureData(_IN const string &identity,
                                                      _IN const Json::Value &value,
                                                      _OUT vector<PalmprintCode> &data)
{
    Json::Value::Members imagePathMembers;
    imagePathMembers = value.getMemberNames();
    for(Json::Value::Members::iterator it = imagePathMembers.begin(); 
            it != imagePathMembers.end(); ++it) {
        PalmprintCode instanceCode(identity.c_str(), (*it).c_str());
        genEDCCoding(value[*it], instanceCode);
        data.push_back(instanceCode);
    }
}

void EDCC::IO::genEDCCoding(_IN const Json::Value &value, _OUT PalmprintCode &coding)
{
    assert(!value.isNull());
    Mat C(configMap.at("imageSize"), configMap.at("imageSize"), CV_8S);
    Mat Cs(configMap.at("imageSize"), configMap.at("imageSize"), CV_8S);
    
    coding.zipCodingC = value["C"].asString();
    coding.zipCodingCs = value["Cs"].asString();

    coding.zipCodingC = EDCC::toUpper(coding.zipCodingC.c_str());
    coding.zipCodingCs = EDCC::toUpper(coding.zipCodingCs.c_str());
}

bool EDCC::IO::insert2JsonValue(_IN PalmprintCode &code, _OUT Json::Value &value)
{
    string identity = code.identity;
    string imagePath = code.imagePath;
    if(!value[identity].isNull() && !value[identity][imagePath].isNull()) {
        cerr << "identity: " << identity << "\timagepath: " << imagePath 
            << "\tis already exists." << endl;
        return false;
    }
    Json::Value codeValue;
    setEDCCoding(code, codeValue);
    value[identity][imagePath] = codeValue;

    return true;
}

void EDCC::IO::setEDCCoding(_IN PalmprintCode &coding, _OUT Json::Value &value)
{
    Json::Value cValue, csValue;
    value["C"] = coding.zipCodingC;
    value["Cs"] = coding.zipCodingCs;
}

