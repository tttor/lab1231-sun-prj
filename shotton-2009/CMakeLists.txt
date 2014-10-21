cmake_minimum_required(VERSION 2.8)
project( Shotton.2009 )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")# Activate C++11

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

#
set(EigenIncludeDir /usr/local/include/eigen3)
set(OpenGMDir /home/tor/sun4/ws/mrf/opengm-master-2.3.1)
set(ProbImageDir ../external/ProbImage)

#
include_directories (${OpenGMDir}/include)
include_directories (${OpenGMDir}/src/external/MaxFlow-v3.02.src-patched)
include_directories (${OpenGMDir}/src/external/QPBO-v1.3.src-patched)
include_directories (../external/ProbImage)
include_directories (../util)

include_directories (${EigenIncludeDir})
include_directories (include)

#
add_library( 
  probimage 
  ../external/ProbImage/probimage.cpp 
)

add_library( 
  shotton 
  src/shotton/shotton.cc
  src/shotton/edge_potential.cc
)

add_library (
  util
  ../util/util/util.cc
)

# 
add_executable ( 
  main 
  src/main/main.cc 
)

#
target_link_libraries ( 
  main 
  shotton
  probimage
  util
  ${OpenCV_LIBS} 
  ${OpenGMDir}/b2/src/external/libexternal-library-maxflow.a
  ${OpenGMDir}/b2/src/external/libexternal-library-qpbo.a
)