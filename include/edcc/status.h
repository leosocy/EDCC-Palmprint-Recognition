// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_INCLUDE_EDCC_STATUS_H_
#define EDCC_INCLUDE_EDCC_STATUS_H_

#include <assert.h>
#include <stdio.h>
#include <string.h>

namespace edcc {

class Status {
 public:
  enum Code {
    kOk = 0,
    kLoadImageError,
    kImageLowQualityError,
    kLoadConfigYamlError,
    kCApiOutBufferInsufficient,
  };
  static Status Ok(const char* msg = NULL) { return Status(kOk, msg); }
  static Status LoadImageError(const char* msg = NULL) {
    return Status(kLoadImageError, msg);
  }
  static Status ImageLowQualityError(const char* msg = NULL) {
    return Status(kImageLowQualityError, msg);
  }
  static Status LoadConfigYamlError(const char* msg = NULL) {
    return Status(kLoadConfigYamlError, msg);
  }
  static Status CApiOutBufferInsufficient(const char* msg = NULL) {
    return Status(kCApiOutBufferInsufficient, msg);
  }

  Status() : state_(NULL) {}
  Status(const Status& s) {
    state_ = (s.state_ == NULL ? NULL : CopyState(s.state_));
  }
  void operator=(const Status& s) {
    if (this != &s) {
      DeleteState();
      state_ = (s.state_ == NULL ? NULL : CopyState(s.state_));
    }
  }
  ~Status() { DeleteState(); }
  Code code() {
    assert(state_ != NULL);
    return static_cast<Code>(state_[0]);
  }
  const char* msg() {
    assert(state_ != NULL);
    return state_ + 1;
  }
  bool IsOk() { return code() == kOk; }

 private:
  Status(Code code, const char* msg = NULL);
  const char* CopyState(const char* state);
  void DeleteState();
  const char* state_;
};

inline Status::Status(Code code, const char* msg) {
  size_t msg_len = (msg == NULL) ? 0 : strlen(msg);
  char* result = new char[msg_len + 2];
  result[0] = static_cast<char>(code);
  strncpy(result + 1, msg, msg_len);
  result[msg_len + 1] = '\0';
  state_ = result;
}

inline const char* Status::CopyState(const char* state) {
  assert(state != NULL);
  size_t state_len = 1 + strlen(state + 1) + 1;  // code_len + msg_len + '\0'
  char* result = new char[state_len];
  memcpy(result, state, state_len);
  return result;
}

inline void Status::DeleteState() {
  if (state_ != NULL) {
    delete[] state_;
    state_ = NULL;
  }
}

}  // namespace edcc

#endif  // EDCC_INCLUDE_EDCC_STATUS_H_