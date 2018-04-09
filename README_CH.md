# EDCC: 高效、准确的掌纹识别算法

*EDCC优点:*

1. **更少的训练样本**
1. **更快的识别速度**
1. **更高的准确率**

`EDCC(Enhanced and Discriminative Competitive Code, 增强的、有辨识度的竞争编码)`算法，用于掌纹的识别。



了解EDCC算法: [EDCC算法详解](https://leosocy.github.io/2017/10/18/EDCC%E7%AE%97%E6%B3%95%E8%AF%A6%E8%A7%A3/)

## 目录结构

- `include`:包含算法API的头文件以及API说明。
- `source`:EDCC算法的源代码以及代码架构。
- `test`:程序的FT用例。
- `thirdparty`:第三方库，包括jsoncpp和gtest。
- `lib`:一个CMakeList用于生成EDCC静态和动态链接库。
- `APIInputExample`:EDCC API使用的一些json[文件示例](https://github.com/Leosocy/EDCC/tree/master/APIInputExample)。

## How to use EDCC in your project

### 生成EDCC静态库&&动态库

1. `git clone https://github.com/Leosocy/EDCC.git`
1. `cd EDCC/lib`
1. `mkdir build`
1. `cmake ..`
1. `make -j`

在`edcc_lib`目录下会生成`libEDCC_STATIC.a`以及`libEDCC.so`

编译库过程常见问题[FAQ](https://github.com/Leosocy/EDCC/tree/master/lib)

### 链接EDCC库

1. 将生成的\*.a和\*.so以及EDCC/include/edcc.h拷贝到自己的项目目录
1. 编辑您的CMakeLists.txt
    ```cmake
    find_package(OpenCV REQUIRED)
    include_directories(edcc.h)
    ······
    target_link_libraries(${PROJECT} EDCC ${OpenCV_LIBS})
    ```

### 使用EDCC API

[API使用说明](https://github.com/Leosocy/EDCC/tree/master/include)
