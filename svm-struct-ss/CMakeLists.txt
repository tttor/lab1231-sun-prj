cmake_minimum_required(VERSION 2.8)
project(svm-struct-ss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(EigenIncludeDir /usr/local/include/eigen3)

include_directories(src/svm-struct-core)
include_directories(src/svm-struct-ss)
include_directories(${EigenIncludeDir})

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
  /home/tor/sun/ws/tmp/lab1231-sun-prj.20141226.2053/external/prob-image/build/libprobimage.a
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
