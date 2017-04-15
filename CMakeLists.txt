cmake_minimum_required(VERSION 2.8)
project( main )
find_package( OpenCV REQUIRED )

add_executable( main main.cpp 
./functions/pretreatment/binarization.cpp 
./functions/pretreatment/adjust_palm.cpp 
./global/general_function.cpp 
./global/general_functions.cpp
./functions/extract_roi/peak_valley_detect.cpp 
./functions/extract_roi/extract_roi.cpp 
./functions/process_parameter/process_parameter.cpp 
./functions/extract_features/image_transform/wavelet.cpp 
./functions/extract_features/image_transform/dft.cpp 
./functions/extract_features/image_transform/gabor.cpp
./functions/extract_features/features_statistic/extract_features_with_statisic.cpp 
./functions/extract_features/features_subspace/extract_features_with_subspace.cpp 
./functions/extract_features/image_reduce_dimension/pca.cpp
./functions/extract_features/features_coding/extract_features_with_coding.cpp
./functions/extract_features/features_base.cpp
./functions/extract_features/features_coding/DRCC.cpp
./functions/extract_features/features_coding/BDRCC.cpp
./functions/extract_features/image_enhance/image_enhance.cpp)
target_link_libraries( main ${OpenCV_LIBS} )
