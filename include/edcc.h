// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef __EDCC_H__
#define __EDCC_H__

#include <map>
#include <string>

#ifdef _WINDOWS

#ifdef EDCC_USER
#define EDCC_API __declspec(dllimport)
#else
#define EDCC_API __declspec(dllexport)
#endif

#else
#define EDCC_API

#endif

#define EDCC_SUCCESS 0
#define EDCC_NULL_POINTER_ERROR 1

#define EDCC_LOAD_CONFIG_FAIL 100

#define EDCC_LOAD_TAINING_SET_FAIL 200

#define EDCC_LOAD_FEATURES_FAIL 300
#define EDCC_SAVE_FEATURES_FAIL 301

#define EDCC_LOAD_PALMPRINT_IMAGE_FAIL 400

#define EDCC_CODING_BUFF_LEN_NOT_ENOUGH 500
#define EDCC_CODING_INVALID 501
#define EDCC_CODINGS_DIFF_CONFIG 502

EDCC_API typedef struct {
  std::string identity;
  std::string imagePath;
  double score;
  size_t rank;
} MatchResult;

#ifdef __cplusplus
extern "C" {
#endif
/* Some primitive operation */
EDCC_API int GetEDCCCoding(const char *palmprint_image_path,
                           const char *config_file_name, size_t buffer_max_len,
                           unsigned char *coding_buffer, size_t *buffer_len);

EDCC_API int GetTwoPalmprintCodingMatchScore(
    const unsigned char *lhs_coding_buffer,
    const unsigned char *rhs_coding_buffer, double *score);

EDCC_API int GetTwoPalmprintMatchScore(const char *lhs_image_path,
                                       const char *rhs_image_path,
                                       const char *config_file_name,
                                       double *score);
/* primitive operation end */

/* Some encapsulated APIs which can be used in small-scale data. */
EDCC_API int GetTrainingSetFeatures(
    const char *trainingset_palmprint_group_file_name,
    const char *config_file_name, const char *features_output_file_name,
    bool is_incremental = false);

EDCC_API int GetTopKMatchScore(
    const char *palmprint_image_path,
    const char *trainingset_features_or_palmprint_group_file_name,
    const char *config_file_name, bool is_features, size_t k,
    std::map<size_t, MatchResult> *top_k_result);
/* encapsulated APIs end */

#ifdef __cplusplus
}
#endif

#endif
