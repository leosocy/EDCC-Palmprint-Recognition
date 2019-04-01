// Copyright (c) 2019 leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license
// that can be found in the LICENSE file.

#include <vector>
#include "codec/gabor_filter.h"
#include "config/config.h"
#include "config/reader.h"
#include "test_base.h"

namespace {

using edcc::ConfigReader;
using edcc::GaborFilter;
using edcc::kDefaultCoreEncoderConfig;
using edcc::SimpleConfigReader;

class CodecGaborFilterTestFixture : public EdccTestFixtureBase {
 public:
  virtual void SetUp() {
    EdccTestFixtureBase::SetUp();
    config_reader_ = std::unique_ptr<ConfigReader>(new edcc::SimpleConfigReader(kDefaultCoreEncoderConfig));
    config_reader_->LoadAndValidate();
    gabor_filter_ = std::unique_ptr<GaborFilter>(new GaborFilter(config_reader_->GetCoreEncoderConfig()));
  }

 protected:
  std::unique_ptr<GaborFilter> gabor_filter_;
  std::unique_ptr<ConfigReader> config_reader_;
};

TEST_F(CodecGaborFilterTestFixture, handle_palmprint_image) {
  std::vector<cv::Mat> result;
  auto cfg = config_reader_->GetCoreEncoderConfig();
  gabor_filter_->Handle(a_01_, &result);
  EXPECT_EQ(result.size(), cfg.gabor_directions);
  auto d1 = result[0];
  EXPECT_EQ(d1.cols, cfg.image_size);  // has been resized
  EXPECT_EQ(d1.channels(), 1);         // has been converted to gray image.
  for (int row = 0; row < d1.rows; ++row) {
    const double *col_ptr = d1.ptr<double>(row);
    for (int col = 0; col < d1.cols; ++col) {
      // expect has been nomalized.
      EXPECT_TRUE(*col_ptr <= 1.0);
      ++col_ptr;
    }
  }
}
}  // namespace