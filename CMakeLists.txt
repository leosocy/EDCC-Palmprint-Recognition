cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME palmprint_recognition)
project( ${PROJECT_NAME} )

find_package( OpenCV REQUIRED )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include )
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB_RECURSE jsoncpp_include_files 
	${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include/*.h
)
source_group("jsoncpp_include" FILES ${jsoncpp_include_files}) 

file(GLOB_RECURSE jsoncpp_src_files
	${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/src/*.cpp
)
source_group("jsoncpp_src" FILES ${jsoncpp_src_files}) 

file(GLOB_RECURSE include_files 
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)
source_group("include" FILES ${include_files}) 

file(GLOB_RECURSE src_files
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)
source_group("src" FILES ${src_files}) 

add_executable( ${PROJECT_NAME}
	${jsoncpp_include_files}
	${jsoncpp_src_files}

	${include_files}
	${src_files}
	)

target_link_libraries( ${PROJECT_NAME}  ${OpenCV_LIBS} )
