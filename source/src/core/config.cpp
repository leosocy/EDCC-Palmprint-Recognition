// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "core/config.h"
#include <string.h>

namespace edcc
{

ConfigManager::ConfigManager()
{
    params_.insert(config::kImageWidth);
    params_.insert(config::kImageHeight);
    params_.insert(config::kGaborKernelSize);
    params_.insert(config::kGaborDirections);
    params_.insert(config::kLaplaceKernelSize);
    params_.insert(config::kCodingMode);
    params_.insert(config::kMatchingMode);
    memset(&cfg_, 0, sizeof(cfg_));
}

bool ConfigManager::GetConfigValue(const std::string &key, int *value)
{
    if (key == config::kImageWidth)
    {
        *value = cfg_.imageSizeW;
    }
    else if (key == config::kImageHeight)
    {
        *value = cfg_.imageSizeH;
    }
    else if (key == config::kGaborKernelSize)
    {
        *value = cfg_.gaborSize;
    }
    else if (key == config::kLaplaceKernelSize)
    {
        *value = cfg_.laplaceSize;
    }
    else if (key == config::kGaborDirections)
    {
        *value = cfg_.directions;
    }
    else if (key == config::kCodingMode)
    {
        *value = cfg_.codingMode;
    }
    else if (key == config::kMatchingMode)
    {
        *value = cfg_.matchingMode;
    }
    else
    {
        return false;
    }
    return true;
}

bool ConfigManager::SetConfigValue(const std::string &key, int value)
{
    if (key == config::kImageWidth)
    {
        cfg_.imageSizeW = value;
    }
    else if (key == config::kImageHeight)
    {
        cfg_.imageSizeH = value;
    }
    else if (key == config::kGaborKernelSize)
    {
        cfg_.gaborSize = value;
    }
    else if (key == config::kLaplaceKernelSize)
    {
        cfg_.laplaceSize = value;
    }
    else if (key == config::kGaborDirections)
    {
        cfg_.directions = value;
    }
    else if (key == config::kCodingMode)
    {
        cfg_.codingMode = value;
    }
    else if (key == config::kMatchingMode)
    {
        cfg_.matchingMode = value;
    }
    else
    {
        return false;
    }
    return true;
}

} // namespace edcc
