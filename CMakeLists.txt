cmake_minimum_required( VERSION 2.8 )

INCLUDE_DIRECTORIES( "include/" )

project( palmprint_recognition )
find_package( OpenCV REQUIRED )

add_executable( palmprint_recognition
	src/exe/palmprint_recognition.cc
	src/lib/IO.cc
	)

link_directories( "third_party/" )
#link_libraries( libjsoncpp.a )
FIND_LIBRARY(JSONCPP_LIB jsoncpp third_party/  NO_DEFAULT_PATH)

target_link_libraries( palmprint_recognition ${JSONCPP_LIB} ${OpenCV_LIBS} )
