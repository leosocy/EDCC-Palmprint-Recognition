// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __STATUS_H__
#define __STATUS_H__

#include <stdio.h>

namespace edcc
{

class Status
{
public:
    enum Code
    {
        kOk = 0,
        kNullPtrError = 1,
        kLoadConfigError = 2,
        kLoadTrainingsetError = 3,
        kLoadFeaturesError = 4,
        kSaveFeaturesError = 5,
        kLoadPalmprintError = 6,
        kCodingBufferLenNotEnough = 7,
        kCodingInvalid = 8,
        kCodingsConfigDiff = 9
    };
    Status() : state_(NULL) {}
    ~Status() { DeleteState(); }
    Status(const Status &s);
    void operator=(const Status &s);

    static Status Ok() { return Status(kOk); }
    static Status NullPtrError() { return Status(kNullPtrError); }
    static Status LoadConfigError() { return Status(kLoadConfigError); }
    static Status LoadTrainingsetError() { return Status(kLoadTrainingsetError); }
    static Status LoadFeaturesError() { return Status(kLoadFeaturesError); }
    static Status SaveFeaturesError() { return Status(kSaveFeaturesError); }
    static Status LoadPalmprintError() { return Status(kLoadPalmprintError); }
    static Status CodingBufferLenNotEnough() { return Status(kCodingBufferLenNotEnough); }
    static Status CodingInvalid() { return Status(kCodingInvalid); }
    static Status CodingsConfigDiff() { return Status(kCodingsConfigDiff); }
    bool IsOk() { return code() == kOk; }

    int ToExtCode();
private:
    Status(Code code);
    const char* CopyState(const char *state);
    void DeleteState();
    Code code() { return static_cast<Code>(state_[0]); }
    // state[0] == code
    const char *state_;
};

inline Status::Status(const Status &s)
{
    state_ = (s.state_ == NULL ? NULL : CopyState(s.state_));
}

inline void Status::operator=(const Status &s)
{
    if (this != &s)
    {
        DeleteState();
        state_ = (s.state_ == NULL ? NULL : CopyState(s.state_));
    }
}

inline void Status::DeleteState()
{
    if (state_ != NULL)
    {
        delete[] state_;
        state_ = NULL;
    }
}

} // namespace edcc

#endif // !__STATUS_H__
