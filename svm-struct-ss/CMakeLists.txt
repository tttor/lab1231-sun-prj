cmake_minimum_required(VERSION 2.8)
project(svm-struct-ss)

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(EigenIncludeDir /usr/local/include/eigen3)
set(OpenGMDir ../external/opengm-2.3.3)

include_directories(src/svm-struct-core)
include_directories(src/svm-struct-ss)
include_directories(${EigenIncludeDir})
include_directories(../shotton-2009/include)
include_directories(../external)
include_directories(../util/include)
include_directories(${OpenGMDir}/include)
include_directories (${OpenGMDir}/src/external/MaxFlow-v3.02.src-patched)
include_directories (${OpenGMDir}/src/external/QPBO-v1.3.src-patched)
include_directories (../philipp-unary-mrf/include)

# make svm light lib
add_library(svm_light
  src/svm-struct-core/svm_light/svm_common.c
  src/svm-struct-core/svm_light/svm_learn_main.c
  src/svm-struct-core/svm_light/svm_learn.c
  src/svm-struct-core/svm_light/svm_classify.c
  src/svm-struct-core/svm_light/svm_hideo.c
)

target_link_libraries(svm_light
  m
)

# make svm struct lib
add_library(svm_struct
  src/svm-struct-core/svm_struct/svm_struct_common.c
  src/svm-struct-core/svm_struct/svm_struct_learn.c
)

target_link_libraries(svm_struct
  svm_light
)

#
add_library(svm_struct_ss_api
  src/svm-struct-ss/api.cc
)

target_link_libraries(svm_struct_ss_api
  svm_light
  svm_struct
  /home/tor/sun/ws/lab1231-sun-prj/external/prob-image/build/libprobimage.a
  /home/tor/sun/ws/lab1231-sun-prj/shotton-2009/build/libshotton_mrf.a
  /home/tor/sun/ws/lab1231-sun-prj/external/opengm-2.3.3/build/src/external/libexternal-library-maxflow.a
  /home/tor/sun/ws/lab1231-sun-prj/external/opengm-2.3.3/build/src/external/libexternal-library-qpbo.a
  ${OpenCV_LIBS}
)

# make ssvm-ss executables
add_executable(svm_struct_ss_learn 
  src/svm-struct-ss/learn.cc
)

target_link_libraries(svm_struct_ss_learn 
  svm_struct_ss_api
)

#add_executable(svm_struct_ss_predict 
#  src/svm-struct-ss/predict.cc
#)

#target_link_libraries(svm_struct_ss_predict 
#svm_struct_ss_api
#)
