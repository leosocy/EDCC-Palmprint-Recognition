// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __IO_H__
#define __IO_H__

#include <fstream>
#include <vector>
#include <set>
#include <string>

#include "json.h"
#include "core/config.h"

#define LOAD_PALMPRINT_GROUP_FAILURE 1
#define LOAD_PALMPRINT_GROUP_SUCCESS 0

#define LOAD_PALMPRINT_FEATURE_DATA_FAILURE 2
#define LOAD_PALMPRINT_FEATURE_DATA_SUCCESS 0

#define SAVE_PALMPRINT_FEATURE_DATA_FAILURE 3
#define SAVE_PALMPRINT_FEATURE_DATA_SUCCESS 0

namespace edcc
{

class PalmprintCode;

class IO
{
public:
    int LoadConfig(std::ifstream &in);
    int LoadPalmprintTrainingSet(std::ifstream &in, std::vector<PalmprintCode> *training_set);
    int LoadPalmprintFeatureData(std::ifstream &in, std::vector<PalmprintCode> *feature_data);
    int SavePalmprintFeatureData(std::ofstream &out, std::vector<PalmprintCode> &feature_data);

    const EDCC_CFG_T& config() const { return cm.config(); }
private:
    bool LoadOneIdentityAllPalmprintFeatureData(const std::string &identity,
                                                const Json::Value &value,
                                                std::vector<PalmprintCode> *feature_data);
    bool GetEDCCoding(const Json::Value &value, PalmprintCode *coding);
    bool SetEDCCoding(PalmprintCode &coding, Json::Value *value);
    bool InsertCoding2JsonValue(PalmprintCode &code, Json::Value *value);

    ConfigManager cm;
};

} // namespace edcc

#endif 

