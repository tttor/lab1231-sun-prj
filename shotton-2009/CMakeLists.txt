cmake_minimum_required(VERSION 2.8)
project( Shotton.2009 )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")# Activate C++11

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

#
set(EigenIncludeDir /usr/local/include/eigen3)
set(OpenGMDir ../external/opengm-2.3.3)

#
include_directories (${OpenGMDir}/include)
include_directories (${OpenGMDir}/src/external/MaxFlow-v3.02.src-patched)
include_directories (${OpenGMDir}/src/external/QPBO-v1.3.src-patched)
include_directories (../util/include)
include_directories (../external)
include_directories (../philipp-unary-mrf/include)

include_directories (${EigenIncludeDir})
include_directories (include)

#
add_library( shotton_mrf 
  src/shotton.cc
  src/edge_potential.cc
  ../philipp-unary-mrf/src/philipp_unary_mrf.cc
  ../util/src/util.cc
  ../util/src/color_map.cc
)

target_link_libraries( shotton_mrf
  /home/tor/sun/ws/lab1231-sun-prj/external/prob-image/build/libprobimage.a
)

# 
add_executable( main 
  src/main.cc 
)

target_link_libraries( main 
  shotton_mrf
  /home/tor/sun/ws/lab1231-sun-prj/external/opengm-2.3.3/build/src/external/libexternal-library-maxflow.a
  /home/tor/sun/ws/lab1231-sun-prj/external/opengm-2.3.3/build/src/external/libexternal-library-qpbo.a
  ${OpenCV_LIBS}
)