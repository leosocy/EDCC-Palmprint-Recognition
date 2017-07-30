cmake_minimum_required( VERSION 2.8 )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include/ )

project( palmprint_recognition )
find_package( OpenCV REQUIRED )

add_executable( palmprint_recognition
	${CMAKE_CURRENT_SOURCE_DIR}/src/exe/palmprint_recognition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/Core.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/IO.cc
	)

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ )
FIND_LIBRARY(JSONCPP_LIB jsoncpp ${CMAKE_CURRENT_SOURCE_DIR}/third_party/  NO_DEFAULT_PATH)

target_link_libraries( palmprint_recognition ${JSONCPP_LIB} ${OpenCV_LIBS} )
