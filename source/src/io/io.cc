// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "io/io.h"

#include <assert.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "edcc.h"
#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"

using namespace std;

namespace edcc
{

#define CODING_FIELD "coding"

#define PALMPRINT_GROUP_FORMAT  "{\n   \
\"identity\" : [\n                     \
    \"path1\"\n                        \
    \"path2\"\n                        \
    ]\n                                \
}"

IO::IO()
{
    params_.insert(IMAGE_SIZE_W);
    params_.insert(IMAGE_SIZE_H);
    params_.insert(GABOR_KERNEL_SIZE);
    params_.insert(GABOR_DIRECTIONS);
    params_.insert(LAPLACE_KERNEL_SIZE);

    memset(&config_, 0, sizeof(config_));
}

IO::~IO()
{

}

int IO::LoadConfig(ifstream &in)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if (!reader.parse(in, root))
    {
        EDCC_Log("IO::loadConfig Parse %s failed, please confirm the file is exists.");
        return EDCC_LOAD_CONFIG_FAIL;
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (!root[*it].isObject() || root[*it]["default"].isNull())
        {
            EDCC_Log("Parse config.json failed, you can only change the value of \
                     \"default\" label in this file.");
            return EDCC_LOAD_CONFIG_FAIL;
        }
        if (params_.find(*it) == params_.end()
            || !GenConfig(*it, root[*it]["default"].asInt()))
        {
            EDCC_Log("Illegal configuration parameters.");
            return EDCC_LOAD_CONFIG_FAIL;
        }
    }

    return EDCC_SUCCESS;
}

int IO::LoadPalmprintTrainingSet(ifstream &in,
                                 vector<PalmprintCode> *training_set)
{
    CHECK_POINTER_NULL_RETURN(training_set, EDCC_LOAD_TAINING_SET_FAIL);

    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;

    if (!reader.parse(in, root))
    {
        EDCC_Log("Parse json failed. Don't change the json format. You need to confirm the format like this.");
        EDCC_Log(PALMPRINT_GROUP_FORMAT"");
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (!root[*it].isArray())
        {
            EDCC_Log("Don't change the json format. You need to confirm the format like this.");
            EDCC_Log(PALMPRINT_GROUP_FORMAT);
            return EDCC_LOAD_TAINING_SET_FAIL;
        }
        Json::Value image_list = root[*it];
        for (size_t image_index = 0; image_index < image_list.size(); ++image_index)
        {
            if (image_list[(unsigned)image_index].isString())
            {
                PalmprintCode newOne((*it).c_str(), image_list[(unsigned)image_index].asString().c_str());
                training_set->push_back(newOne);
            }
        }
    }

    return EDCC_SUCCESS;
}

int IO::LoadPalmprintFeatureData(ifstream &in, vector<PalmprintCode> *feature_data)
{
    CHECK_POINTER_NULL_RETURN(feature_data, EDCC_LOAD_TAINING_SET_FAIL);

    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if (!reader.parse(in, root))
    {
        EDCC_Log("Load Palmprint Features Data failed. Don't change the json format.");
        return EDCC_LOAD_FEATURES_FAIL;
    }
    for (set<string>::iterator it = params_.begin(); it != params_.end(); ++it)
    {
        if (root[*it].isNull() || !root[*it].isInt())
        {
            EDCC_Log("Load EDCC config from features data failed. Make sure json file has config.");
            return EDCC_LOAD_CONFIG_FAIL;
        }
        if (!GenConfig(*it, root[*it].asInt()))
        {
            EDCC_Log("Load EDCC config from features data failed. Make sure json file has config.");
            return EDCC_LOAD_CONFIG_FAIL;
        }
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (params_.find(*it) == params_.end()
            && root[*it].isObject())
        {
            if (!LoadOneIdentityAllPalmprintFeatureData(*it, root[*it], feature_data))
            {
                EDCC_Log("Load EDCC features data failed. Make sure features data has not been changed.");
                return EDCC_LOAD_FEATURES_FAIL;
            }
        }
    }

    return EDCC_SUCCESS;
}

int IO::SavePalmprintFeatureData(ofstream &out, vector<PalmprintCode> &feature_data)
{
    if (!out.is_open())
    {
        EDCC_Log("Output stream can't open.");
        return EDCC_SAVE_FEATURES_FAIL;
    }
    Json::Value root;
    for (set<string>::iterator it = params_.begin(); it != params_.end(); ++it)
    {
        int value = 0;
        if (!GetConfig(*it, &value))
        {
            EDCC_Log("If you want to train/predict, load config.json first.\
                    Or if you want incremental training/prediction, load trainData.json first.");
            return EDCC_SAVE_FEATURES_FAIL;
        }
        else
        {
            root[*it] = value;
        }
    }

    for (vector<PalmprintCode>::iterator it = feature_data.begin();
         it != feature_data.end(); ++it)
    {
        InsertCoding2JsonValue(*it, &root);
    }
    out << root.toStyledString();

    return EDCC_SUCCESS;
}

bool IO::GenConfig(const string &config_key, int config_value)
{
    if (config_key == IMAGE_SIZE_W)
    {
        config_.imageSizeW = config_value;
    }
    else if (config_key == IMAGE_SIZE_H)
    {
        config_.imageSizeH = config_value;
    }
    else if (config_key == GABOR_KERNEL_SIZE)
    {
        config_.gaborSize = config_value;
    }
    else if (config_key == LAPLACE_KERNEL_SIZE)
    {
        config_.laplaceSize = config_value;
    }
    else if (config_key == GABOR_DIRECTIONS)
    {
        config_.directions = config_value;
    }
    else
    {
        return false;
    }
    return true;
}

bool IO::GetConfig(const string &config_key, int *config_value)
{
    if (config_key == IMAGE_SIZE_W)
    {
        *config_value = config_.imageSizeW;
    }
    else if (config_key == IMAGE_SIZE_H)
    {
        *config_value = config_.imageSizeH;
    }
    else if (config_key == GABOR_KERNEL_SIZE)
    {
        *config_value = config_.gaborSize;
    }
    else if (config_key == LAPLACE_KERNEL_SIZE)
    {
        *config_value = config_.laplaceSize;
    }
    else if (config_key == GABOR_DIRECTIONS)
    {
        *config_value = config_.directions;
    }
    else
    {
        return false;
    }
    return true;
}

bool IO::LoadOneIdentityAllPalmprintFeatureData(const string &identity,
                                                const Json::Value &value,
                                                vector<PalmprintCode> *feature_data)
{
    Json::Value::Members image_path_list;
    image_path_list = value.getMemberNames();

    for (Json::Value::Members::iterator it = image_path_list.begin(); 
         it != image_path_list.end(); ++it)
    {
        PalmprintCode instance(identity.c_str(), (*it).c_str());
        if (GetEDCCoding(value[*it], &instance))
        {
            feature_data->push_back(instance);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool IO::GetEDCCoding(const Json::Value &value, PalmprintCode *instance)
{
    if (value.isNull()
        || !value.isObject()
        || value[CODING_FIELD].isNull()
        || !value[CODING_FIELD].isString())
    {
        EDCC_Log("Load Palmprint Features Data failed. Don't change the json format.");
        return false;
    }

    string hex_coding = value[CODING_FIELD].asString();
    CHECK_STR_EMPTY_RETURN(hex_coding, false);
    if (EDCC_SUCCESS != instance->DecodeFromHexString(hex_coding))
    {
        return false;
    }

    return true;
}

bool IO::SetEDCCoding(PalmprintCode &coding, Json::Value *value)
{
    string hex_coding = "";
    Status s = coding.EncodeToHexString(config_, &hex_coding);
    if (s != EDCC_SUCCESS)
    {
        return false;
    }
    (*value)[CODING_FIELD] = hex_coding;

    return true;
}

bool IO::InsertCoding2JsonValue(PalmprintCode &code, Json::Value *value)
{
    string identity = code.palmprint()->identity();
    string image_path = code.palmprint()->image_path();
    if (!(*value)[identity].isNull()
        && !(*value)[identity][image_path].isNull())
    {
        EDCC_Log("identity: %s\timagepath: %s\tis already exists.", identity.c_str(), image_path.c_str());
        return false;
    }
    Json::Value coding_value;
    CHECK_EQ_RETURN(SetEDCCoding(code, &coding_value), false, false);
    (*value)[identity][image_path] = coding_value;

    return true;
}

} // namespace edcc
