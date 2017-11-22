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
    paramsSet.insert(IMAGE_SIZE_W);
    paramsSet.insert(IMAGE_SIZE_H);
    paramsSet.insert(GABOR_KERNEL_SIZE);
    paramsSet.insert(GABOR_DIRECTIONS);
    paramsSet.insert(LAPLACE_KERNEL_SIZE);
}

int EDCC::IO::loadConfig(_IN ifstream &in)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if(!reader.parse(in, root)) {
        EDCC_Log("Parse config.json failed, please confirm the file is exists.\n");
        return EDCC_LOAD_CONFIG_FAIL;
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); 
            it != members.end(); ++it) {
        if(!root[*it].isObject() || root[*it]["default"].isNull()) {
            EDCC_Log("Parse config.json failed, you can only change the value of \
                     \"default\" label in this file.\n");
            return EDCC_LOAD_CONFIG_FAIL;
        } else {
            if(paramsSet.find(*it) != paramsSet.end()) {
                if(configMap.find(*it) != configMap.end()) {
                    configMap.at(*it) = root[*it]["default"].asInt();
                } else {
                    configMap.insert(map<string, int>::value_type(*it, root[*it]["default"].asInt()));
                }         
            } else {
                EDCC_Log("Illegal configuration parameters.\n");
                return EDCC_LOAD_CONFIG_FAIL;
            }
        }
    }

    return EDCC_SUCCESS;
}

int EDCC::IO::loadPalmprintGroup(_IN ifstream &in, _INOUT vector<PalmprintCode> &groupVec)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;

    if(!reader.parse(in, root)) {
        EDCC_Log("Parse json failed. Don't change the json format. You need to confirm the format like this.\n");
        EDCC_Log(PALMPRINT_GROUP_FORMAT"\n");
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); 
            it != members.end(); ++it) {
        if(!root[*it].isArray()) {
            EDCC_Log("Don't change the json format. You need to confirm the format like this.\n");
            EDCC_Log(PALMPRINT_GROUP_FORMAT"\n");
            return EDCC_LOAD_TAINING_SET_FAIL;
        }
        Json::Value imageList = root[*it];
        for(size_t imageIndex = 0; imageIndex < imageList.size(); ++imageIndex) {
            if(imageList[(unsigned)imageIndex].isString()) {
                PalmprintCode newOne((*it).c_str(), imageList[(unsigned)imageIndex].asString().c_str());
                groupVec.push_back(newOne);
            }
        }
    }

    return EDCC_SUCCESS;
}

int EDCC::IO::loadPalmprintFeatureData(_IN ifstream &in, _INOUT vector<PalmprintCode> &data)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if(!reader.parse(in, root)) {
        EDCC_Log("Load Palmprint Features Data failed. Don't change the json format.\n");
        return EDCC_LOAD_FEATURES_FAIL;
    }
    for(set<string>::iterator it = paramsSet.begin(); it != paramsSet.end(); ++it) {
        if(root[*it].isNull() || !root[*it].isInt()) {
            EDCC_Log("Load Palmprint Features Data failed. Make sure json file has config.\n");
            return EDCC_LOAD_CONFIG_FAIL;
        }
        if(configMap.find(*it) != configMap.end()) {
            configMap.at(*it) = root[*it].asInt();
        } else {
            configMap.insert(map<string, int>::value_type(*it, root[*it].asInt()));
        } 
    }
    members = root.getMemberNames();
    for(Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) {
        if(paramsSet.find(*it) == paramsSet.end()
           && root[*it].isObject()) {
            loadOneIdentityAllPalmprintFeatureData(*it, root[*it], data);
        }
    }

    return EDCC_SUCCESS;
}

int EDCC::IO::savePalmprintFeatureData(_IN ofstream &out, _IN vector<PalmprintCode> &data)
{
    if(!out.is_open()) {
        EDCC_Log("Output stream can't open.\n");
        return EDCC_SAVE_FEATURES_FAIL;
    }
    Json::Value root;
    for(set<string>::iterator it = paramsSet.begin(); it != paramsSet.end(); ++it) {
        if(configMap.find(*it) == configMap.end()) {
            EDCC_Log("If you want to train/predict, load config.json first.\
                    Or if you want incremental training/prediction, load trainData.json first.\n");
            return EDCC_SAVE_FEATURES_FAIL;
        } else {
            root[*it] = configMap.at(*it);
        }
    }

    for(vector<PalmprintCode>::iterator it = data.begin(); it != data.end(); ++it) {
        insert2JsonValue(*it, root);
    }
    out << root.toStyledString();

    return EDCC_SUCCESS;
}

void EDCC::IO::loadOneIdentityAllPalmprintFeatureData(_IN const string &identity,
                                                      _IN const Json::Value &value,
                                                      _INOUT vector<PalmprintCode> &data)
{
    Json::Value::Members imagePathMembers;
    Json::Value::Members::iterator it;
    imagePathMembers = value.getMemberNames();

    for(it = imagePathMembers.begin(); it != imagePathMembers.end(); ++it) {
        PalmprintCode instanceCode(identity.c_str(), (*it).c_str());
        if(getEDCCoding(value[*it], instanceCode)) {
            data.push_back(instanceCode);
        }
    }
}

bool EDCC::IO::getEDCCoding(_IN const Json::Value &value, _INOUT PalmprintCode &coding)
{
    if(value.isNull() 
       || !value.isObject()
       || value["coding"].isNull()
       || !value["coding"].isString()) {
        EDCC_Log("Load Palmprint Features Data failed. Don't change the json format.\n");
        return false;
    }
    
    int imageSizeW = configMap.at(IMAGE_SIZE_W);
    int imageSizeH = configMap.at(IMAGE_SIZE_H);

    string aesCoding = value["coding"].asString();
    stringstream aesSS;

    for(size_t i = 0; i < aesCoding.length(); i += 2) {
        string tmp = aesCoding.substr(i, 2);
        unsigned char c;
        sscanf(tmp.c_str(), "%02x", &c);
        aesSS << c;
    }
    coding.decrypt(aesSS.str());

    return true;
}

bool EDCC::IO::insert2JsonValue(_IN PalmprintCode &code, _INOUT Json::Value &value)
{
    string identity = code.identity;
    string imagePath = code.imagePath;
    if(!value[identity].isNull() 
       && !value[identity][imagePath].isNull()) {
        EDCC_Log("identity: %s\timagepath: %s\tis already exists.\n");
        return false;
    }
    Json::Value codeValue;
    CHECK_EQ_RETURN(setEDCCoding(code, codeValue), false, false);
    value[identity][imagePath] = codeValue;

    return true;
}

bool EDCC::IO::setEDCCoding(_IN PalmprintCode &coding, _INOUT Json::Value &value)
{
    size_t bufMaxLen = configMap[IMAGE_SIZE_W] * configMap[IMAGE_SIZE_H];
    unsigned char* l_pCoding = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    CHECK_POINTER_NULL_RETURN(l_pCoding, false);
    memset(l_pCoding, 0, sizeof(unsigned char) * bufMaxLen);

    size_t bufLen = coding.encrypt(l_pCoding, bufMaxLen, configMap);
    CHECK_EQ_RETURN(bufLen, 0, false);

    value["coding"] = coding.encodeToHexString();
}

