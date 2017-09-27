cmake_minimum_required( VERSION 2.8 )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include )

project( palmprint_recognition )
find_package( OpenCV REQUIRED )

file(GLOB jsoncpp_files
${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/src/*.cpp
)

file(GLOB_RECURSE src_files
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable( palmprint_recognition
	${jsoncpp_files}
	${src_files}
	)

target_link_libraries( palmprint_recognition  ${OpenCV_LIBS} )
