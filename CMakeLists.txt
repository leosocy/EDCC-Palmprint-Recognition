cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME palmprint_recognition)
project( ${PROJECT_NAME} )

find_package( OpenCV REQUIRED )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include )
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB_RECURSE headers *.h)

file(GLOB_RECURSE jsoncpp_files
	${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/src/*.cpp
)

file(GLOB_RECURSE src_files
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable( ${PROJECT_NAME}
	${jsoncpp_files}
	${src_files}
	${headers}
	)

target_link_libraries( ${PROJECT_NAME}  ${OpenCV_LIBS} )
