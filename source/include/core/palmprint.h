// Copyright (c) 2017 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.

#ifndef __PALMPRINT_H__
#define __PALMPRINT_H__

#include <string>
#include <opencv2/opencv.hpp>

namespace edcc
{

using std::string;
using cv::Mat;
using cv::Size;

class Palmprint
{
public:
    Palmprint(const char *identity, const char *image_path);
    explicit Palmprint(const Palmprint &rhs);
    Palmprint& operator =(const Palmprint &rhs);
    virtual ~Palmprint();
    virtual bool operator==(const Palmprint &rhs) const;

    string identity() const { return identity_; }
    string image_path() const { return image_path_; }

    Mat* GenOrigImg();
    Mat* GenSpecImg(const Size &img_size, bool is_gray = true);
private:
    string identity_;
    string image_path_;
    Mat image_;
};


} // namespace edcc

#endif // __PALMPRINT_H__