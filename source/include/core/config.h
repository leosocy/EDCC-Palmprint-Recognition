// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <set>
#include "util/pub.h"

namespace edcc
{

namespace config
{

static const char* const kImageWidth = "imageSizeW";
static const char* const kImageHeight = "imageSizeH";
static const char* const kGaborKernelSize = "gaborKernelSize";
static const char* const kLaplaceKernelSize = "laplaceKernelSize";
static const char* const kGaborDirections = "gaborDirections";

} // namespace config

typedef struct tag_EDCC_CFG_T
{
    u_short imageSizeW;
    u_short imageSizeH;
    u_short gaborSize;
    u_char laplaceSize;
    u_char directions;
} EDCC_CFG_T;

class ConfigManager
{
public:
    ConfigManager();
    bool GetConfigValue(const std::string &key, int *value);
    bool SetConfigValue(const std::string &key, int value);
    bool IsKeyInConfigParams(const std::string &key)
    { 
        return params_.find(key) != params_.end();
    }
    const EDCC_CFG_T& config() const { return cfg_; }
    const std::set<std::string>& params() const { return params_; }
private:
    EDCC_CFG_T cfg_;
    std::set<std::string> params_;
};

} // namespace edcc

#endif // !__CONFIG_H__
