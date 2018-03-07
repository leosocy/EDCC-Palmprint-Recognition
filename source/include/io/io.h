// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __IO_H__
#define __IO_H__

#include <fstream>
#include <vector>
#include <string>
#include "json.h"
#include "core/config.h"

namespace edcc
{

class PalmprintCode;
class Status;

class IO
{
public:
    Status LoadConfig(std::ifstream &in);
    Status LoadPalmprintTrainingSet(std::ifstream &in, std::vector<PalmprintCode> *training_set);
    Status LoadPalmprintFeatureData(std::ifstream &in, std::vector<PalmprintCode> *feature_data);
    Status SavePalmprintFeatureData(std::ofstream &out, std::vector<PalmprintCode> &feature_data);

    const EDCC_CFG_T& config() const { return cm_.config(); }
private:
    bool LoadOneIdentityAllPalmprintFeatureData(const std::string &identity,
                                                const Json::Value &value,
                                                std::vector<PalmprintCode> *feature_data);
    bool GetEDCCoding(const Json::Value &value, PalmprintCode *coding);
    bool SetEDCCoding(PalmprintCode &coding, Json::Value *value);
    bool InsertCoding2JsonValue(PalmprintCode &code, Json::Value *value);

    ConfigManager cm_;
};

} // namespace edcc

#endif 

