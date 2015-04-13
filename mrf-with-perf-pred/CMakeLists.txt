cmake_minimum_required (VERSION 2.8)
project (mrf_with_perf_pred)

#
find_package(OpenCV REQUIRED)

#
set (PROBIMAGE_DIR ../external/prob-image)
set (EIGEN_INC_DIR /usr/include/eigen3)
set (PYTHON_INC_DIR /usr/include/python2.7)

#
include_directories (../external)
include_directories (../util/include)
include_directories (../shotton-2009/include)
include_directories (${EIGEN_INC_DIR})
include_directories (${PYTHON_INC_DIR})
include_directories (include)

#
add_library(util
  ../util/src/util.cc
  ../util/src/color_map.cc
)

add_library(mrf_with_perf_pred
  ../shotton-2009/src/edge_potential.cc
  ${PROBIMAGE_DIR}/probimage.cpp
  src/mrf_with_perf_pred.cc
  src/perf_predictor.cc
)

# 
add_definitions(-DDEBUG_LEVEL_1)
add_executable(main src/main.cc )

target_link_libraries( main 
  ${OpenCV_LIBS}
  util
  mrf_with_perf_pred
  python2.7 
  boost_python
)
