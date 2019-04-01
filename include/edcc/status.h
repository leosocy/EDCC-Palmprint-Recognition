// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#ifndef EDCC_INCLUDE_EDCC_STATUS_H_
#define EDCC_INCLUDE_EDCC_STATUS_H_

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace edcc {

#define DECL_STATUS_CODE_FUNC(name, code)         \
  static Status name(const char* fmt = "", ...) { \
    va_list args;                                 \
    va_start(args, fmt);                          \
    Status status = Status(code, fmt, args);      \
    va_end(args);                                 \
    return status;                                \
  }

class Status {
 public:
  enum Code {
    kOk = 0,
    kInvalidArgument = 1,
    kLackingCodeBuffer = 2,
  };
  DECL_STATUS_CODE_FUNC(Ok, kOk);
  DECL_STATUS_CODE_FUNC(InvalidArgument, kInvalidArgument);
  DECL_STATUS_CODE_FUNC(LackingCodeBuffer, kLackingCodeBuffer);

  Status() : state_(nullptr) {}
  Status(const Status& s) = delete;
  Status& operator=(const Status& s) = delete;
  Status(Status&& s) : state_(s.state_) { s.state_ = nullptr; }
  Status& operator=(Status&& s) noexcept {
    if (this == &s) {
      return *this;
    }
    DeleteState();
    state_ = s.state_;
    s.state_ = nullptr;
    return *this;
  }
  ~Status() { DeleteState(); }
  Code code() {
    assert(state_ != nullptr);
    return static_cast<Code>(state_[0]);
  }
  const char* msg() {
    assert(state_ != nullptr);
    return state_ + 1;
  }
  bool IsOk() { return code() == kOk; }

 private:
  Status(Code code, const char* fmt, va_list args);
  void DeleteState();
  const char* state_;
};

inline Status::Status(Code code, const char* fmt, va_list args) {
  assert(fmt != nullptr);
  size_t msg_len = strlen(fmt);
  char* result = new char[msg_len + 2];
  result[0] = static_cast<char>(code);
  vsnprintf(result + 1, msg_len, fmt, args);
  result[msg_len + 1] = '\0';
  state_ = result;
}

inline void Status::DeleteState() {
  if (state_ != nullptr) {
    delete[] state_;
    state_ = nullptr;
  }
}

}  // namespace edcc

#endif  // EDCC_INCLUDE_EDCC_STATUS_H_