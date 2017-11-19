cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME EDCC)
project(${PROJECT_NAME})
set(JSONCPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/jsoncpp)
set(EDCC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../source)
set(EDCC_PUB_HEADER ${CMAKE_SOURCE_DIR}/../include)

message(STATUS "——————————————————————————————————————")
message(STATUS "PROJECT_NAME : " ${PROJECT_NAME})
message(STATUS "DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "——————————————————————————————————————")


find_package(OpenCV REQUIRED)

include_directories(${EDCC_ROOT}/include/)
include_directories(${EDCC_PUB_HEADER})
include_directories(${JSONCPP_ROOT}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE jsoncpp_include_files 
	${JSONCPP_ROOT}/include/*.h
	${JSONCPP_ROOT}/src/*.h
)

file(GLOB_RECURSE jsoncpp_src_files
	${JSONCPP_ROOT}/src/*.cpp
	${JSONCPP_ROOT}/src/*.c
	${JSONCPP_ROOT}/src/*.ccc
)

file(GLOB_RECURSE edcc_include_files 
	${EDCC_ROOT}/include/*.h
	${EDCC_PUB_HEADER}/*.h
)

file(GLOB_RECURSE edcc_src_files
	${EDCC_ROOT}/src/*.cc
	${EDCC_ROOT}/src/*.cpp
	${EDCC_ROOT}/src/*.c
)

if(MSVC)
	add_compile_options("/MT")
elseif (UNIX)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/edcc_lib)
set(srcs ${jsoncpp_include_files} ${jsoncpp_src_files} ${edcc_include_files} ${edcc_src_files})

# 生成动态库目标
add_library(${PROJECT_NAME} SHARED ${srcs})
# 生成静态库目标
add_library(${PROJECT_NAME}_STATIC STATIC ${srcs})

# 指定静态库的输出名称
set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES OUTPUT_NAME ${PROJECT_NAME}_STATIC)
# 使动态库和静态库同时存在
set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)
