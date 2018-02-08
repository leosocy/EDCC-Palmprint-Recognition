// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#include "edcc.h"

#include "io/io.h"
#include "core/config.h"
#include "core/palmprint.h"
#include "core/palmprint_code.h"
#include "core/check.h"
#include "util/pub.h"

using namespace std;
using namespace edcc;

const char* const kDefaultId = "default_id";
const char* const kDefaultImagePath = "default_image_path";

int GetEDCCCoding(const char *palmprint_image_path,
                  const char *config_file_name,
                  size_t buffer_max_len,
                  unsigned char *coding_buffer,
                  size_t *buffer_len)
{
    *buffer_len = 0;

    CHECK_POINTER_NULL_RETURN(palmprint_image_path, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(config_file_name, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(coding_buffer, EDCC_NULL_POINTER_ERROR);

    IO train_io;
    ifstream config_in;
    config_in.open(config_file_name);
    if(config_in.is_open()
       && EDCC_SUCCESS != train_io.LoadConfig(config_in)) {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    PalmprintCode onePalmprint(kDefaultId, palmprint_image_path);
    Status s;
    s = onePalmprint.EncodeToBuffer(train_io.config(),
                                          buffer_max_len,
                                          coding_buffer,
                                          buffer_len);

    return s;
}

int GetTwoPalmprintCodingMatchScore(const unsigned char *lhs_coding_buffer,
                                    const unsigned char *rhs_coding_buffer,
                                    double *score)
{
    *score = 0.0;

    CHECK_POINTER_NULL_RETURN(lhs_coding_buffer, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(rhs_coding_buffer, EDCC_NULL_POINTER_ERROR); 

    PalmprintCode lhs(kDefaultId, kDefaultImagePath);
    PalmprintCode rhs(kDefaultId, kDefaultImagePath);
    
    Status s = 0;
    s = lhs.Decode(lhs_coding_buffer);
    CHECK_NE_RETURN(s, EDCC_SUCCESS, s);
    s = rhs.Decode(rhs_coding_buffer);
    CHECK_NE_RETURN(s, EDCC_SUCCESS, s);

    if (!Check::CheckTwoPalmprintCodeConfigEqual(lhs, rhs))
    {
        return EDCC_CODINGS_DIFF_CONFIG;
    }
    *score = lhs.MatchWith(rhs);
    //*score = Match::MatchFastMode(lhs_coding_buffer, rhs_coding_buffer);

    return EDCC_SUCCESS;
}

int GetTwoPalmprintMatchScore(const char *lhs_image_path,
                              const char *rhs_image_path,
                              const char *config_file_name,
                              double *score)
{
    *score = 0.0;

    CHECK_POINTER_NULL_RETURN(lhs_image_path, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(rhs_image_path, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(config_file_name, EDCC_NULL_POINTER_ERROR);

    IO match_io;
    ifstream config_in;
    config_in.open(config_file_name);
    if (config_in.is_open()
        && EDCC_SUCCESS != match_io.LoadConfig(config_in))
    {
        return EDCC_LOAD_CONFIG_FAIL;
    }

    PalmprintCode lhs(kDefaultId, lhs_image_path);
    PalmprintCode rhs(kDefaultId, rhs_image_path);

    Status api_ret = 0;
    api_ret = lhs.Encode(match_io.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);
    api_ret = rhs.Encode(match_io.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);

    *score = lhs.MatchWith(rhs);

    return EDCC_SUCCESS;
}

size_t EncodeAllPalmprint(const EDCC_CFG_T &config,
                          vector<PalmprintCode> *all_palmprint_code);

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &original_features,
                                         const vector<PalmprintCode> &incremental_features,
                                         vector<PalmprintCode> *all_features);

bool SortTopK(const PalmprintCode &instance,
              const vector<PalmprintCode> &all_features,
              size_t k,
              map<size_t, MatchResult> *top_k_result);

int GetTrainingSetFeatures(const char *trainingset_palmprint_group_file_name,
                           const char *config_file_name,
                           const char *features_output_file_name,
                           bool is_incremental)
{
    CHECK_POINTER_NULL_RETURN(trainingset_palmprint_group_file_name, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(config_file_name, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(features_output_file_name, EDCC_NULL_POINTER_ERROR);

    IO train_io;
    vector<PalmprintCode> all_features;
    vector<PalmprintCode> original_features;
    int ret_value = 0;

    if(!is_incremental) {
        ifstream config_in;
        config_in.open(config_file_name);
        CHECK_FALSE_RETURN(config_in.is_open(), EDCC_LOAD_CONFIG_FAIL);
        ret_value = train_io.LoadConfig(config_in);
        CHECK_NE_RETURN(ret_value, EDCC_SUCCESS, EDCC_LOAD_CONFIG_FAIL);
    } else {
        ifstream features_in;
        features_in.open(features_output_file_name);
        CHECK_FALSE_RETURN(features_in.is_open(), EDCC_LOAD_FEATURES_FAIL);
        ret_value = train_io.LoadPalmprintFeatureData(features_in, &original_features);
        if(ret_value != EDCC_SUCCESS
           || !Check::CheckFeatureData(original_features, train_io.config())) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    }
    if(!Check::CheckConfig(train_io.config())) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    
    ifstream trainingset_in;
    trainingset_in.open(trainingset_palmprint_group_file_name);
    CHECK_FALSE_RETURN(trainingset_in.is_open(), EDCC_LOAD_TAINING_SET_FAIL);
    ret_value = train_io.LoadPalmprintTrainingSet(trainingset_in, &all_features);
    if(ret_value != EDCC_SUCCESS 
       || !Check::CheckTrainingSet(all_features)) {
        return EDCC_LOAD_TAINING_SET_FAIL;
    }
    
    EncodeAllPalmprint(train_io.config(), &all_features);
    if(is_incremental) {
        BuildUpAllFeaturesWhenIncremental(original_features, all_features, &all_features);
    }

    ofstream features_out;
    features_out.open(features_output_file_name);
    CHECK_FALSE_RETURN(features_out.is_open(), EDCC_SAVE_FEATURES_FAIL);
    ret_value = train_io.SavePalmprintFeatureData(features_out, all_features);
    CHECK_NE_RETURN(ret_value, EDCC_SUCCESS, EDCC_SAVE_FEATURES_FAIL);

    return EDCC_SUCCESS;
}

int GetTopKMatchScore(const char *palmprint_image_path,
                      const char *trainingset_features_or_palmprint_group_file_name,
                      const char *config_file_name,
                      bool is_features,
                      size_t k,
                      std::map<size_t, MatchResult> *top_k_result)
{
    CHECK_POINTER_NULL_RETURN(palmprint_image_path, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(trainingset_features_or_palmprint_group_file_name, EDCC_NULL_POINTER_ERROR);
    CHECK_POINTER_NULL_RETURN(config_file_name, EDCC_NULL_POINTER_ERROR);

    Status api_ret = 0;
    IO match_io;
    int ret_value = 0;
    ifstream features_or_group_in;
    vector<PalmprintCode> all_features;
    
    features_or_group_in.open(trainingset_features_or_palmprint_group_file_name);
    CHECK_FALSE_RETURN(features_or_group_in.is_open(), EDCC_LOAD_FEATURES_FAIL);
    if(is_features) {
        ret_value = match_io.LoadPalmprintFeatureData(features_or_group_in, &all_features);
        if(ret_value != EDCC_SUCCESS
           || !Check::CheckFeatureData(all_features, match_io.config())) {
            return EDCC_LOAD_FEATURES_FAIL;
        }
    } else {
        ret_value = match_io.LoadPalmprintTrainingSet(features_or_group_in, &all_features);
        if(ret_value != EDCC_SUCCESS
           || !Check::CheckTrainingSet(all_features)) {
            return EDCC_LOAD_TAINING_SET_FAIL;
        }

        ifstream configIn;
        configIn.open(config_file_name);
        CHECK_FALSE_RETURN(configIn.is_open(), EDCC_LOAD_CONFIG_FAIL);
        ret_value = match_io.LoadConfig(configIn);
    }
    if(ret_value != EDCC_SUCCESS
       || !Check::CheckConfig(match_io.config())) {
        return EDCC_LOAD_CONFIG_FAIL;
    }
    if(!is_features) {
        EncodeAllPalmprint(match_io.config(), &all_features);
    }
    PalmprintCode onePalmprint("identity", palmprint_image_path);
    api_ret = onePalmprint.Encode(match_io.config());
    CHECK_NE_RETURN(api_ret, EDCC_SUCCESS, api_ret);

    SortTopK(onePalmprint, all_features, k, top_k_result);

    return EDCC_SUCCESS;
}

size_t EncodeAllPalmprint(const EDCC_CFG_T &config,
                          vector<PalmprintCode> *all_palmprint_code)
{
    assert(all_palmprint_code);
    vector<PalmprintCode>::iterator pc_iter;
    for(pc_iter = all_palmprint_code->begin();
        pc_iter != all_palmprint_code->end();) {
        Status s;
        s = pc_iter->Encode(config);
        if(s != EDCC_SUCCESS) {
            vector<PalmprintCode>::iterator pc_iter_tmp = pc_iter;
            pc_iter = all_palmprint_code->erase(pc_iter_tmp);
            continue;
        }
        ++pc_iter;
    }

    return all_palmprint_code->size();
}

size_t BuildUpAllFeaturesWhenIncremental(const vector<PalmprintCode> &original_features,
                                         const vector<PalmprintCode> &incremental_features,
                                         vector<PalmprintCode> *all_features)
{
    vector<PalmprintCode>::const_iterator pc_iter_outter, pc_iter_inner;
    *all_features = incremental_features;

    for(pc_iter_outter = original_features.begin();
        pc_iter_outter != original_features.end();
        ++pc_iter_outter) {
        bool exists = false;
        for(pc_iter_inner = incremental_features.begin();
            pc_iter_inner != incremental_features.end();
            ++pc_iter_inner) {
            if((*pc_iter_outter->palmprint()) == (*pc_iter_inner->palmprint())) {
                EDCC_Log("----Cover\t%s: %s", 
                         pc_iter_outter->palmprint()->identity().c_str(), 
                         pc_iter_outter->palmprint()->image_path().c_str());
                exists = true;
                break;
            }
        }
        if(!exists) {
            all_features->push_back(*pc_iter_outter);
        }
    }

    return all_features->size();
}

bool cmp(const MatchResult &lhs , const MatchResult &rhs)
{
    return lhs.score > rhs.score;
}

bool SortTopK(const PalmprintCode &instance,
              const vector<PalmprintCode> &all_features,
              size_t k,
              map<size_t, MatchResult> *top_k_result)
{
    vector<MatchResult> results;

    for(size_t i = 0; i < all_features.size(); ++i) {
        MatchResult oneResult;
        oneResult.identity = all_features.at(i).palmprint()->identity();
        oneResult.imagePath = all_features.at(i).palmprint()->image_path();
        oneResult.score = all_features.at(i).MatchWith(instance);
        results.push_back(oneResult);
    }
    sort(results.begin(), results.end(), cmp);

    for(size_t i = 0; i < k && i < all_features.size(); ++i) {
        results.at(i).rank = i;
        top_k_result->insert(map<size_t, MatchResult>::value_type(i, results.at(i)));
    }

    return true;
}
