cmake_minimum_required( VERSION 2.8 )

INCLUDE_DIRECTORIES( ./include/EDCC )

project( palmprint_recognition )
find_package( OpenCV REQUIRED )

add_executable( palmprint_recognition
	./src/exe/palmprint_recognition.cc
	#	./src/lib/IO.cc
	)

link_libraries( libjsoncpp.a )

target_link_libraries( palmprint_recognition ${OpenCV_LIBS} )
