cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME jsoncpp)
project(${PROJECT_NAME})

message(STATUS "——————————————————————————————————————")
message(STATUS "PROJECT_NAME : " ${PROJECT_NAME})
message(STATUS "DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "——————————————————————————————————————")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(GLOB_RECURSE jsoncpp_include_files 
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE jsoncpp_src_files
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.ccc
)
if(MSVC)
	source_group(include FILES ${jsoncpp_include_files})
	source_group(src FILES ${jsoncpp_src_files}) 
endif()

if(MSVC)
	add_compile_options("/MTd")
elseif (UNIX)
endif()

add_library(${PROJECT_NAME} STATIC 
						${jsoncpp_include_files}
						${jsoncpp_src_files})
