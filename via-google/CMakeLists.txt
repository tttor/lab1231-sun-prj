cmake_minimum_required(VERSION 2.8)
project( Shotton.2009 )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")# Activate C++11

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

#
set(EigenIncludeDir /usr/local/include/eigen3)
set(OpenGMDir /home/rizkivmaster/EXPERIMENT/opengm)
set(ProbImageDir ../external/prob-image/ProbImage)

#
include_directories (${OpenGMDir}/include)
include_directories (${OpenGMDir}/src/external/MaxFlow-v3.02.src-patched)
include_directories (${OpenGMDir}/src/external/QPBO-v1.3.src-patched)
include_directories (${ProbImageDir})
include_directories (../util/include)

include_directories (${EigenIncludeDir})
include_directories (include)



add_library( 
  probimage 
  ${ProbImageDir}/probimage.cpp 
)

add_library (
  util
  ../util/src/util.cc
)

add_library (
  color_map
  ../util/src/color_map.cc
)

target_link_libraries ( util color_map  )

add_subdirectory( src )
