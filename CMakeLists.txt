########################################################################
# CMake build script for edcc.

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." ON)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(PROJECT_NAME edcc)
PROJECT(${PROJECT_NAME})

SET(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
SET(JSONCPP_ROOT ${PROJECT_SOURCE_DIR}/thirdparty/jsoncpp)
SET(EDCC_ROOT ${PROJECT_SOURCE_DIR}/source)
SET(EDCC_PUB_HEADER ${PROJECT_SOURCE_DIR}/include)

FIND_PACKAGE(OpenCV REQUIRED)

MESSAGE(STATUS "——————————————————————————————————————")
MESSAGE(STATUS "PROJECT_NAME : " ${PROJECT_NAME})
MESSAGE(STATUS "DIR : " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "OpenCV_LIBS : " ${OpenCV_LIBS})
MESSAGE(STATUS "OpenCV_Include : " ${OpenCV_INCLUDE_DIRS})
MESSAGE(STATUS "——————————————————————————————————————")

INCLUDE_DIRECTORIES(${EDCC_ROOT}/include/)
INCLUDE_DIRECTORIES(${EDCC_PUB_HEADER})
INCLUDE_DIRECTORIES(${JSONCPP_ROOT}/include)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

FILE(GLOB_RECURSE jsoncpp_include_files 
	${JSONCPP_ROOT}/include/*.h
	${JSONCPP_ROOT}/src/*.h
)
FILE(GLOB_RECURSE jsoncpp_src_files
	${JSONCPP_ROOT}/src/*.cpp
	${JSONCPP_ROOT}/src/*.c
	${JSONCPP_ROOT}/src/*.ccc
)

FILE(GLOB_RECURSE edcc_include_files 
	${EDCC_ROOT}/include/*.h
	${EDCC_PUB_HEADER}/*.h
)

FILE(GLOB_RECURSE edcc_src_files
	${EDCC_ROOT}/src/*.cc
	${EDCC_ROOT}/src/*.cpp
	${EDCC_ROOT}/src/*.c
)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/edcc_lib)
SET(srcs ${jsoncpp_include_files} ${jsoncpp_src_files} ${edcc_include_files} ${edcc_src_files})

ADD_LIBRARY(${PROJECT_NAME} ${srcs})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION 1.1 SOVERSION 1)

INSTALL(TARGETS edcc
        LIBRARY DESTINATION lib)
INSTALL(DIRECTORY ${EDCC_PUB_HEADER}/ DESTINATION include/edcc
        FILES_MATCHING PATTERN "*.h"
        PATTERN "*.md"
        PATTERN "apiInputsExample" EXCLUDE
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ)
INSTALL(FILES ${PROJECT_ROOT}/LICENSE DESTINATION include/edcc)
EXECUTE_PROCESS(COMMAND touch /etc/ld.so.conf.d/edcc.conf
		COMMAND echo "${CMAKE_INSTALL_PREFIX}/lib"
		OUTPUT_FILE /etc/ld.so.conf.d/edcc.conf)
EXECUTE_PROCESS(COMMAND ldconfig)
