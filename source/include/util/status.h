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
    Status() : state_(NULL) {}
    ~Status() { DeleteState(); }
    Status(const Status &s);
    void operator=(const Status &s);

    static Status Ok() { return Status(kOk); }
    static Status NullPtrError() { return Status(kNullPtrError); }
private:
    const char* CopyState(const char *state);
    void DeleteState()
    {
        if (state_ != NULL)
        {
            delete[] state_;
            state_ = NULL;
        }
    }
    enum Code
    {
        kOk = 0,
        kNullPtrError = 1
    };
    Status(Code code);
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

} // namespace edcc

#endif // !__STATUS_H__
