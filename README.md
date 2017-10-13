# EDCC: 高效、准确的掌纹识别算法

`EDCC( Enhanced and Discriminative Competitive Code )`算法，用于掌纹的识别。

## 目录结构

- `include`:包含算法API接口的头文件。
- `source`:EDCC算法的源代码，以及一个算法说明.md。
- `test`:程序的FT用例。
- `thirdparty`:第三方库，包括jsoncpp和gtest。
- `lib`:一个CMakeList用于生成EDCC静态和动态链接库。
- `configExample`:EDCC API使用的一些json文件示例

## How To Use EDCC In Your Project

### 生成EDCC静态库&&动态库

1. `git clone https://github.com/Leosocy/EDCC.git`
1. `cd EDCC/lib`
1. `mkdir build`
1. `cmake ..`
1. `make -j`
1. `cd edcc_lib`
1. 将生成的\*.a和\*.so以及EDCC/include/EDCC.h拷贝到自己的项目目录

### 使用EDCC库

#### CMakeLists.txt

```cmake
find_package(OpenCV REQUIRED)
include_directories(EDCC.h)
······
target_link_libraries(${PROJECT} EDCC ${OpenCV_LIBS})
```
