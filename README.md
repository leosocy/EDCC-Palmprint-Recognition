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

### Install Python module

Please make sure the C++ library is installed successfully.

**Python3.x** required.

*Steps:*

1. `cd pypackage`
1. `python setup.py install`

## Usage

### C

### C++

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
```

You can find the detail under `samples/cpp_sample` directory.

### Python

## Run samples

### C

### C++

### Python

### Contributing

Please see [CONTRIBUTING.md](https://github.com/Leosocy/EDCC-Palmprint-Recognition/blob/master/CONTRIBUTING.md)
