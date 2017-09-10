cmake_minimum_required( VERSION 2.8 )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include )

project( palmprint_recognition )
find_package( OpenCV REQUIRED )

file(GLOB jsoncpp_files
${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/src/*.cpp
)

add_executable( palmprint_recognition
	${jsoncpp_files}
	${CMAKE_CURRENT_SOURCE_DIR}/src/exe/palmprint_recognition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/Core.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/IO.cc
	)

target_link_libraries( palmprint_recognition  ${OpenCV_LIBS} )
