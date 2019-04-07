# EDCC: An efficient and accurate algorithm for palmprint-recognition

[![Build Status](https://travis-ci.org/Leosocy/EDCC-Palmprint-Recognition.svg?branch=master)](https://travis-ci.org/Leosocy/EDCC-Palmprint-Recognition)
[![codecov](https://codecov.io/gh/Leosocy/EDCC-Palmprint-Recognition/branch/master/graph/badge.svg)](https://codecov.io/gh/Leosocy/EDCC-Palmprint-Recognition)
[![MIT licensed](https://img.shields.io/badge/license-MIT-green.svg)](https://raw.githubusercontent.com/Leosocy/EDCC-Palmprint-Recognition/master/LICENSE)

`EDCC(Enhanced and Discriminative Competitive Code)`, which is used for palmprint-recognition.

Use the `EDCC` algorithm to validate on several published palmprint databases(`multispectral`, `tongji`), the first `N(N = 2, 4, 6, 8)` palmprint images of each palm are employed as training samples and the remaining palmprint images form the test sample set. Each sample in the test sample set is compared with all samples of each class in the training set to calculate the matching score. The class that produces the highest matching score is treated as the class of the test sample.

|     Database      |   N=2    |   N=4    |   N=6    |   N=8    |
| :---------------: | :------: | :------: | :------: | :------: |
| `Multispectral_B` | 98.6800% | 99.8750% | 99.9667% | 99.9800% |
| `Multispectral_G` | 98.8400% | 99.8500% | 99.9333% | 99.9500% |
| `Multispectral_I` | 98.9200% | 99.9000% | 99.9000% | 99.9000% |
| `Multispectral_R` | 98.8400% | 99.7500% | 99.8667% | 99.9000% |
|     `Tongji`      | 98.8056% | 99.6979% | 99.9881% | 99.9861% |

Advantages of `EDCC` algorithm:

1. **Less training samples.**
1. **Faster recognition speed.**
1. **Higher recognition accuracy.**

[**More details of `EDCC`**](https://blog.leosocy.top/EDCC%20Algorithm/)

## Installation

### Install library

There are ***some requirements*** if you want to install `EDCC` library:

- OS *nix like.
- [`OpenCV`](https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html) installed.

*Steps:*

1. `git clone https://github.com/Leosocy/EDCC-Palmprint-Recognition.git`
2. `cd EDCC-Palmprint-Recognition && mkdir -p build && cd build`
3. `cmake .. && sudo make install`

### Install Python Package

Please make sure that the core library has been successfully installed by following the steps above.

**Python3.x** required.

*Steps:*

1. `cd pypackage`
1. `python setup.py install`

## Usage

### C/C++

In your CMakeLists.txt, add these lines:

```cmake
find_package(edcc REQUIRED)
include_directories(${EDCC_INCLUDE_DIR})
...
add_dependencies(${YOUR_PROJECT} ${EDCC_LIBRARIES})
target_link_libraries(${YOUR_PROJECT} ${EDCC_LIBRARIES})
```

Then you can use it in your source code like this:

```c++
#include <edcc/facade.h>
#include <edcc/status.h>

#define ASSERT_STATUS_OK(s) \
  do {                      \
    if (!s.IsOk()) {        \
      perror(s.msg());      \
      return -1;            \
    }                       \
  } while (0)

using edcc::EdccFacade;
using edcc::Status;

int main() {
  Status s;
  // create a new encoder.
  auto inst = EdccFacade::Instance();
  auto encoder_id = inst->NewEncoderWithConfig(29, 5, 5, 10, &s);
  ASSERT_STATUS_OK(s);
  // encode palmprints to code buffer.
  size_t buffer_size = inst->GetSizeOfCodeBufferRequired(encoder_id);
  char* code_buffer_one = new char[buffer_size];
  char* code_buffer_another = new char[buffer_size];
  inst->EncodePalmprint(encoder_id, one_image_file_path, code_buffer_one, buffer_size, &s);
  inst->EncodePalmprint(encoder_id, another_image_file_path, code_buffer_another, buffer_size, &s);
  ASSERT_STATUS_OK(s);
  // calculate the similarity score of two codes.
  double score = inst->CalcCodeSimilarity(code_buffer_one, code_buffer_another, &s);
  ASSERT_STATUS_OK(s);
  return 0;
}
```

You can find the detail under `examples/cpp_example` directory.

### Python

```Python
import edcc

config  = edcc.EncoderConfig(29, 5, 5 ,10)
encoder = edcc.create_encoder(config)
one_palmprint_code = encoder.encode_using_filename("./palmprint_one.bmp")
another_palmprint_code = encoder.encode_using_filename("./palmprint_another.bmp")
# TODO: encode_using_image_bytes from requests.
similarity_score = one_palmprint_code.compare_to(another_palmprint_code)
```

## Examples

Make sure you have installed [library](#Install\ library) and [python package](#Install\ Python\ Package) before running examples.

Before you run examples, please prepare some date follow this:

1. `cd examples/palmprint_data`
1. `sh download.sh`

### C

### C++

### Python

## Contributing

Please see [CONTRIBUTING.md](https://github.com/Leosocy/EDCC-Palmprint-Recognition/blob/master/CONTRIBUTING.md)
