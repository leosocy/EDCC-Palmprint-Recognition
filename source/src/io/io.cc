// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "io/io.h"
#include <assert.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "core/edccoding.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"
#include "core/config.h"
#include "util/status.h"

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

Status IO::LoadConfig(ifstream &in)
{
    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if (!reader.parse(in, root))
    {
        EDCC_Log("IO::loadConfig Parse %s failed, please confirm the file is exists.");
        return Status::LoadConfigError();
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (!root[*it].isObject()
            || root[*it]["default"].isNull()
            || !cm_.SetConfigValue(*it, root[*it]["default"].asInt()))
        {
            EDCC_Log("Parse config.json failed, you can only change the value of \
                     \"default\" label in this file.");
            return Status::LoadConfigError();
        }
    }

    return Status::Ok();
}

Status IO::LoadPalmprintTrainingSet(ifstream &in,
                                    vector<PalmprintCode> *training_set)
{
    CHECK_POINTER_NULL_RETURN(training_set, Status::NullPtrError());

    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;

    if (!reader.parse(in, root))
    {
        EDCC_Log("Parse json failed. Don't change the json format. You need to confirm the format like this.");
        EDCC_Log(PALMPRINT_GROUP_FORMAT"");
        return Status::LoadTrainingsetError();
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        if (!root[*it].isArray())
        {
            EDCC_Log("Don't change the json format. You need to confirm the format like this.");
            EDCC_Log(PALMPRINT_GROUP_FORMAT);
            return Status::LoadTrainingsetError();
        }
        Json::Value image_list = root[*it];
        for (size_t image_index = 0; image_index < image_list.size(); ++image_index)
        {
            if (image_list[(unsigned)image_index].isString())
            {
                PalmprintCode instance((*it).c_str(), image_list[(unsigned)image_index].asString().c_str());
                training_set->push_back(instance);
            }
        }
    }

    return Status::Ok();
}

Status IO::LoadPalmprintFeatureData(ifstream &in, vector<PalmprintCode> *feature_data)
{
    CHECK_POINTER_NULL_RETURN(feature_data, Status::LoadFeaturesError());

    Json::Value root;
    Json::Reader reader;
    Json::Value::Members members;
    if (!reader.parse(in, root))
    {
        EDCC_Log("Load Palmprint Features Data failed. Don't change the json format.");
        return Status::LoadFeaturesError();
    }
    for (set<string>::const_iterator it = cm_.params().begin();
         it != cm_.params().end();
         ++it)
    {
        if (root[*it].isNull()
            || !root[*it].isInt()
            || !cm_.SetConfigValue(*it, root[*it].asInt()))
        {
            EDCC_Log("Load EDCC config from features data failed. Make sure json file has config.");
            return Status::LoadConfigError();
        }
    }
    members = root.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (!cm_.IsKeyInConfigParams(*it)
            && root[*it].isObject())
        {
            if (!LoadOneIdentityAllPalmprintFeatureData(*it, root[*it], feature_data))
            {
                EDCC_Log("Load EDCC features data failed. Make sure features data has not been changed.");
                return Status::LoadFeaturesError();
            }
        }
    }

    return Status::Ok();
}

Status IO::SavePalmprintFeatureData(ofstream &out, vector<PalmprintCode> &feature_data)
{
    if (!out.is_open())
    {
        EDCC_Log("Output stream can't open.");
        return Status::SaveFeaturesError();
    }
    Json::Value root;
    for (set<string>::iterator it = cm_.params().begin(); it != cm_.params().end(); ++it)
    {
        int value = 0;
        if (!cm_.GetConfigValue(*it, &value))
        {
            EDCC_Log("If you want to train/predict, load config.json first.\
                    Or if you want incremental training/prediction, load trainData.json first.");
            return Status::SaveFeaturesError();
        }
        root[*it] = value;
    }

    for (vector<PalmprintCode>::iterator it = feature_data.begin();
         it != feature_data.end(); ++it)
    {
        InsertCoding2JsonValue(*it, &root);
    }
    out << root.toStyledString();

    return Status::Ok();
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
    Status s = instance->DecodeFromHexString(hex_coding);
    return s.IsOk() ? true : false;
}

bool IO::SetEDCCoding(PalmprintCode &coding, Json::Value *value)
{
    string hex_coding("");
    Status s = coding.EncodeToHexString(config(), &hex_coding);
    if (!s.IsOk())
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
    if (!SetEDCCoding(code, &coding_value))
    {
        return false;
    }
    (*value)[identity][image_path] = coding_value;

    return true;
}

} // namespace edcc
