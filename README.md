# EDCC: An efficient and accurate algorithm for palmprint-recognition

`EDCC(Enhanced and Discriminative Competitive Code)`, which is used for palmprint-recognition.

Use the `EDCC` algorithm to validate on several published palmprint databases(`multispectral`, `tongji`), the first `N(N = 1, 2, 3, 4)` palmprint images of each palm are employed as training samples and the remaining palmprint images form the test sample set. Each sample in the test sample set is compared with all samples of each class in the training set to calculate the matching score. The class that produces the highest matching score is treated as the class of the test sample.

|       Database      |   N=5   |   N=6   |   N=7   |   N=8   |
|      :-------:      | :-----: | :-----: | :-----: | :-----: |
|  `Multispectral_B`  |97.9429% |98.0333% |99.6400% |99.9800% |
|  `Multispectral_G`  |98.1714% |98.1667% |99.6400% |99.9500% |
|  `Multispectral_I`  |98.7429% |98.7667% |99.8400% |99.9500% |
|  `Multispectral_R`  |98.4857% |98.5000% |99.6000% |99.8500% |
|       `Tongji`      |99.8111% |99.9405% |99.9744% |99.9722% |

Advantages of `EDCC` algorithm:

1. **Less training samples.**
1. **Faster recognition speed.**
1. **Higher recognition accuracy.**

[**More details of `EDCC`**](https://leosocy.github.io/2017/10/18/EDCC-Algorithm/)

## How to use `EDCC` in your project

### Use existing library

1. `git clone https://github.com/Leosocy/EDCC.git`
1. `cd EDCC/lib`

There is existing library in [`lib`](https://github.com/Leosocy/EDCC/tree/master/lib) directory, include Windows/Linux. EDCC api header is in [`include`](https://github.com/Leosocy/EDCC/tree/master/include) directory.

You can use the following methods to use EDCC in your project:

- Add *.a *.so to system lib directory, such as /usr/lib; add EDCC.h to system include directory, such as /usr/include.
- Add EDCC library directory path to your system environment varibles.
- Edit your CMakeLists.txt
    ```cmake
    find_package(OpenCV REQUIRED)
    include_directories(EDCC.h)
    ······
    link_directories(${EDCC_LIB_PATH})
    target_link_libraries(${PROJECT} EDCC ${OpenCV_LIBS})
    ```
- Put EDCC library and EDCC.h in the same folder as your project.

### Generate library and use it

1. `git clone https://github.com/Leosocy/EDCC.git`
1. `cd EDCC/lib`
1. `mkdir build`
1. `cmake ..`
1. `make -j`

`libEDCC_STATIC.a` and `libEDCC.so` will be generated in `edcc_lib` directory.

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
- `APIInputExample`: Some json files [examples](https://github.com/Leosocy/EDCC/tree/master/APIInputExample) which are used in EDCC api.