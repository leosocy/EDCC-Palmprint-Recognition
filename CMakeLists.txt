cmake_minimum_required(VERSION 2.8)
project( main )
find_package( OpenCV REQUIRED )

<<<<<<< HEAD
add_executable( main main.cpp ./functions/pretreatment/binarization.cpp ./functions/pretreatment/adjust_palm.cpp ./global/general_function.cpp ./functions/extract_roi/peak_valley_detect.cpp ./functions/extract_roi/extract_roi.cpp ./functions/process_parameter/process_parameter.cpp)
=======
add_executable( main main.cpp ./functions/pretreatment/binarization.cpp ./functions/pretreatment/adjust_palm.cpp ./global/general_function.cpp ./functions/extract_roi/peak_valley_detect.cpp ./functions/extract_roi/extract_roi.cpp ./functions/process_parameter/process_parameter.cpp ./functions/extract_features/image_transform/wavelet.cpp )
>>>>>>> feature_statistic

target_link_libraries( main ${OpenCV_LIBS} )
