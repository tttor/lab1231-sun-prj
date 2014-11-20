cmake_minimum_required (VERSION 2.8)
project (PhilippUnaryMRF)

#
find_package(OpenCV REQUIRED)

#
set(OPENGM_DIR ../external/opengm-2.3.3)
set(PROBIMAGE_DIR ../external/prob-image)
set(EIGEN_DIR /usr/local/include/eigen3)

#
include_directories(../external)
include_directories(../util/include)
include_directories(${OPENGM_DIR}/include)
include_directories(${OPENGM_DIR}/src/external/MaxFlow-v3.02.src-patched)
include_directories(${EIGEN_DIR})

# TODO why this does not work? and raise a warning!
#link_directories(${OPENGM_DIR}/build/src/external)

#
add_library(util
  ../util/src/util.cc
  ../util/src/color_map.cc
)

# 
add_definitions(-DDEBUG_OFF)
add_executable(main 
  main.cc
  ${PROBIMAGE_DIR}/probimage.cpp
)

target_link_libraries(main 
  ${OpenCV_LIBS}
  util
  /home/tor/sun/ws/lab1231-sun-prj/external/opengm-2.3.3/build/src/external/libexternal-library-maxflow.a
)