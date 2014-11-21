cmake_minimum_required (VERSION 2.8)
project (ladicky2009)

#
find_package(OpenCV REQUIRED)

#
set (VLFEAT_DIR ../external/vlfeat-0.9.19)
set (PROBIMAGE_DIR ../external/prob-image)
set (EIGEN_DIR /usr/include/eigen3)
#
include_directories (../external)
include_directories (../util/include)
include_directories (../segmenter/include)
include_directories (../ladicky-2009/include)
include_directories (../shotton-2009/include)
include_directories (${EIGEN_DIR})

#
add_library(util
  ../util/src/util.cc
  ../util/src/color_map.cc
)

add_library(ladicky
  ../ladicky-2009/src/ladicky.cc
  ../ladicky-2009/src/robustpn.cc
  ../shotton-2009/src/edge_potential.cc
  ${PROBIMAGE_DIR}/probimage.cpp
)

add_library(segmenter
  ../segmenter/src/segment_wrapper.cc
  ${VLFEAT_DIR}/vl/host.c
  ${VLFEAT_DIR}/vl/random.c
  ${VLFEAT_DIR}/vl/generic.c
  ${VLFEAT_DIR}/vl/slic.c
)

# 
add_definitions(-DDEBUG_OFF)
add_executable(main src/main.cc )

target_link_libraries( main 
  ${OpenCV_LIBS}
  util
  ladicky
  segmenter
)


