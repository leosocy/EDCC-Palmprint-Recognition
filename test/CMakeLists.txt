cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME ft_edcc)
project(${PROJECT_NAME})

set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/../thirdparty/gtest)
set(EDCC_ROOT ${CMAKE_SOURCE_DIR}/../source)
set(EDCC_PUB_HEADER ${CMAKE_SOURCE_DIR}/../include)

message(STATUS "——————————————————————————————————————")
message(STATUS "PROJECT_NAME : " ${PROJECT_NAME})
message(STATUS "DIR : " ${CMAKE_BINARY_DIR})
message(STATUS "——————————————————————————————————————")

add_subdirectory(${EDCC_ROOT} ${CMAKE_BINARY_DIR}/EDCC)

include_directories(${EDCC_PUB_HEADER})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE test_include_files 
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${EDCC_PUB_HEADER}/*.h
)
file(GLOB_RECURSE test_src_files
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)
if(MSVC)
	source_group(include FILES ${test_include_files})
	source_group(src FILES ${test_src_files}) 
endif()

add_executable(${PROJECT_NAME} ${test_include_files} ${test_src_files})

#FIND_LIBRARY(LIB_GTEST gtest.a ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)
link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} EDCC)
