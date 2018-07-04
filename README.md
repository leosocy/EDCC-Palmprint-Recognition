# EDCC: An efficient and accurate algorithm for palmprint-recognition

[![pipeline status](https://gitlab.com/leosocy/EDCC-Palmprint-Recognition/badges/master/pipeline.svg)](https://gitlab.com/leosocy/EDCC-Palmprint-Recognition/commits/master)
[![codecov](https://codecov.io/gh/Leosocy/EDCC-Palmprint-Recognition/branch/master/graph/badge.svg)](https://codecov.io/gh/Leosocy/EDCC-Palmprint-Recognition)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/Leosocy/EDCC-Palmprint-Recognition/blob/master/LICENSE)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/16141/badge.svg)](https://scan.coverity.com/projects/leosocy-edcc-palmprint-recognition)

`EDCC(Enhanced and Discriminative Competitive Code)`, which is used for palmprint-recognition.

Use the `EDCC` algorithm to validate on several published palmprint databases(`multispectral`, `tongji`), the first `N(N = 2, 4, 6, 8)` palmprint images of each palm are employed as training samples and the remaining palmprint images form the test sample set. Each sample in the test sample set is compared with all samples of each class in the training set to calculate the matching score. The class that produces the highest matching score is treated as the class of the test sample.

|       Database      |   N=2   |   N=4   |   N=6   |   N=8   |
|      :-------:      | :-----: | :-----: | :-----: | :-----: |
|  `Multispectral_B`  |98.6800% |99.8750% |99.9667% |99.9800% |
|  `Multispectral_G`  |98.8400% |99.8500% |99.9333% |99.9500% |
|  `Multispectral_I`  |98.9200% |99.9000% |99.9000% |99.9000% |
|  `Multispectral_R`  |98.8400% |99.7500% |99.8667% |99.9000% |
|       `Tongji`      |98.8056% |99.6979% |99.9881% |99.9861% |

Advantages of `EDCC` algorithm:

1. **Less training samples.**
1. **Faster recognition speed.**
1. **Higher recognition accuracy.**

[**More details of `EDCC`**](https://leosocy.github.io/2017/10/18/EDCC-Algorithm/)

## Installation

There are ***some requirements*** if you want to install `EDCC`:

1. Linux/Unix OS
1. OpenCV Installed

Steps:

1. `git clone https://github.com/Leosocy/EDCC-Palmprint-Recognition.git`
1. `cd EDCC-Palmprint-Recognition && mkdir -p build && cd build`
1. `cmake ..`
1. `sudo make -j install`

## How to use `EDCC` in your project

There are some [samples](https://github.com/Leosocy/EDCC-Palmprint-Recognition/tree/master/samples) about how to use `edcc api`.

[Usage of api.](https://github.com/Leosocy/EDCC/tree/master/include)

- C++

    In your CMakeLists.txt, add these lines:

    ```cmake
    FIND_PACKAGE(EDCC REQUIRED)
    INCLUDE_DIRECTORIES(${EDCC_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(YOUR_TARGET_NAME ${EDCC_LIBS})
    ```

    Then you can use those apis in `edcc.h` like this:

    ```C++
    #include <edcc.h>
    int main(int argc, char** argv)
    {
        const char* palmprint_image_path_1 = "IMAGE_PATH_1";
        const char* palmprint_image_path_2 = "IMAGE_PATH_1";
        const char* config_file_path = "config.json";
        #define CODE_BUFFER_MAX_LEN 1024
        unsigned char code_buffer_1[CODE_BUFFER_MAX_LEN] = {0};
        unsigned char code_buffer_2[CODE_BUFFER_MAX_LEN] = {0};
        size_t buffer_len_1 = 0;
        size_t buffer_len_2 = 0;
        int ret = GetEDCCCoding(palmprint_image_path_1,
                                config_file_path,
                                CODE_BUFFER_MAX_LEN,
                                code_buffer_1,
                                &buffer_len_1);
        ret = GetEDCCCoding(palmprint_image_path_2,
                            config_file_path,
                            CODE_BUFFER_MAX_LEN,
                            code_buffer_2,
                            &buffer_len_2);
        double coding_matching_score = 0.0;
        double image_matching_score = 0.0;
        GetTwoPalmprintCodingMatchScore(code_buffer_1,
                                        code_buffer_2,
                                        &coding_matching_score);
        GetTwoPalmprintMatchScore(palmprint_image_path_1,
                                  palmprint_image_path_2,
                                  config_file_path,
                                  &image_matching_score);
    }
    ```

- Python

    Python need 3.x.

    In your py, add these lines:

    ```python
    from edcc_adapter import *

    api = EdccApi()
    coding_1, coding_len_1 = api.GetEDCCCoding("IMAGE_PATH_1", "CONFIG_PATH")
    coding_2, coding_len_2 = api.GetEDCCCoding("IMAGE_PATH_2", "CONFIG_PATH")

    coding_matching_score = api.GetTwoPalmprintCodingMatchScore(coding_1, coding_2)
    image_matching_score = api.GetTwoPalmprintMatchScore("IMAGE_PATH_1", "IMAGE_PATH_2", "CONFIG_PATH")
    ```

## Run samples

Before running the example, make sure `make install` succeed.

- Run Python sample

    1. `cd EDCC-Palmprint-Recognition && mkdir -p build && cd build`
    1. `cmake ..`
    1. `sudo make -j run_py_sample`

## How to contribute code

