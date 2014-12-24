cmake_minimum_required(VERSION 2.8)
project(ssvm-ss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

include_directories(src/svm-struct-core)
include_directories(src/svm-struct-ss)

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

# make ssvm-ss executables
add_executable(svm_struct_ss_learn 
  src/svm-struct-ss/svm_struct_ss_learn.cc
  src/svm-struct-ss/svm_struct_ss_api.cc
)

target_link_libraries(svm_struct_ss_learn 
  svm_light
  svm_struct
)

add_executable(svm_struct_ss_predict 
  src/svm-struct-ss/svm_struct_ss_predict.cc
  src/svm-struct-ss/svm_struct_ss_api.cc
)

target_link_libraries(svm_struct_ss_predict 
  svm_light
  svm_struct
)