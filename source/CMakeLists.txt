cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME EDCC)
project(${PROJECT_NAME})
set(JSONCPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/jsoncpp)
set(EDCC_PUB_HEADER ${CMAKE_SOURCE_DIR}/../include)
set(AES_ROOT ${CMAKE_SOURCE_DIR}/../thirdparty/aes)

message(STATUS "——————————————————————————————————————")
message(STATUS "PROJECT_NAME : " ${PROJECT_NAME})
message(STATUS "DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "——————————————————————————————————————")

add_subdirectory(${JSONCPP_ROOT} ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp)

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${EDCC_PUB_HEADER})
include_directories(${JSONCPP_ROOT}/include)
include_directories(${AES_ROOT}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE edcc_include_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

file(GLOB_RECURSE edcc_src_files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)
if(MSVC)
    source_group(include FILES ${edcc_include_files})
    source_group(src FILES ${edcc_src_files}) 
endif()

if(MSVC)
    add_compile_options("/MT")
elseif (UNIX)
endif()

add_library(${PROJECT_NAME} STATIC 
            ${edcc_include_files}
            ${edcc_src_files})
#FIND_LIBRARY(LIB_JSONCPP NAMES jsoncpp PATHS ${CMAKE_BINARY_DIR}/thirdparty/jsoncpp/Debug NO_DEFAULT_PATH)
#message(${LIB_JSONCPP})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} jsoncpp)
