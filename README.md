# EDCC: An efficient and accurate algorithm for palmprint-recognition

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

## How to use `EDCC` in your project

### Use existing library

1. `git clone https://github.com/Leosocy/EDCC-Palmprint-Recognition.git`
1. `cd EDCC-Palmprint-Recognition/lib`

There is existing library in [`lib`](https://github.com/Leosocy/EDCC/tree/master/lib) directory, include Windows/Linux. EDCC api header is in [`include`](https://github.com/Leosocy/EDCC/tree/master/include) directory.

You can use the following methods to use EDCC in your project:

- Add *.a *.so to system lib directory, such as /usr/lib; add edcc.h to system include directory, such as /usr/include.
- Add edcc library directory path to your system environment varibles.
- Edit your CMakeLists.txt
    ```cmake
    find_package(OpenCV REQUIRED)
    include_directories(edcc.h)
    ······
    link_directories(${EDCC_LIB_PATH})
    target_link_libraries(${PROJECT} edcc ${OpenCV_LIBS})
    ```
- Put EDCC library and EDCC.h in the same folder as your project.

### Generate library and use it

1. `git clone https://github.com/Leosocy/EDCC-Palmprint-Recognition.git`
1. `cd EDCC/lib`
1. `mkdir build`
1. `cmake ..`
1. `make -j`

`libedcc_static.a` and `libedcc.so` will be generated in `edcc_lib` directory.

Compile library process frequently asked questions [FAQ](https://github.com/Leosocy/EDCC/tree/master/lib)

## API Instructions

[Instructions](https://github.com/Leosocy/EDCC/tree/master/include)

## Directory Structure

- `include`: Contain the EDCC api header and api instructions.
- `source`: EDCC algorithm source code and code architecture.
- `samples`: Some samples about how to use EDCC api, include `python`, `C++`.
- `test`: functions test.
- `thirdparty`: Third party library, include `gtest` and `jsoncpp`.
- `lib`: A CMakeList used to generate EDCC static and dynamic link libraries. And two directories include windows and linux libraries.