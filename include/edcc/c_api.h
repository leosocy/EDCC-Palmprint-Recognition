// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_INCLUDE_EDCC_C_API_H_
#define EDCC_INCLUDE_EDCC_C_API_H_

#define EDCC_API

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a edcc encoder.
 * @param status_ptr: a pointer of status, status[0] stands for code, status[1:] stands for msg.
 * @return: id of encoder. return -1 if config is invalid.
 */
EDCC_API int new_encoder_with_config(uint8_t image_size, uint8_t gabor_kernel_size, uint8_t laplace_kernel_size,
                                     uint8_t gabor_directions, char* status_ptr);

/**
 * Get the length used to initialize code buffer.
 * @param eid: encoder id returned from `new_encoder_with_config`.
 * @return: size of code buffer which calculate by encoder config.
 */
EDCC_API unsigned long get_size_of_code_buffer_required(int eid);

/**
 * Encode a palmprint bytes to code buffer.
 * @param eid: encoder id returned from `new_encoder_with_config`.
 * @param palmprint_bytes: a bytes of palmprint image.
 * @param palmprint_bytes_size: the size of palmprint_bytes.
 * @param code_bytes: a bytes of palmprint code buffer.
 * @param code_bytes_size: the size of code_bytes buffer.
 */
EDCC_API void encode_palmprint_bytes(int eid, const char* palmprint_bytes, unsigned long palmprint_bytes_size,
                                     char* code_bytes, unsigned long code_bytes_size, char* status_ptr);

/**
 * Calculate similarity of two codes.
 * @param lhs_code_bytes: one code_bytes encoded by `encode_palmprint_bytes`.
 * @param rhs_code_bytes: another code_bytes encoded by `encode_palmprint_bytes`.
 * @return: the similarity score between two codes.
 */
EDCC_API double calculate_codes_similarity(char* lhs_code_bytes, char* rhs_code_bytes, char* status_ptr);

#ifdef __cplusplus
}
#endif

#endif  // EDCC_INCLUDE_EDCC_C_API_H_