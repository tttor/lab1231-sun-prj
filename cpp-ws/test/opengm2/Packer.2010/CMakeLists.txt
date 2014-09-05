cmake_minimum_required(VERSION 2.8)
project( experiment )

find_package( OpenCV REQUIRED )

include_directories (/home/tor/sun/ws/mrf/opengm-master-2.3.1/include)
include_directories (/home/tor/robotics/prj/011/ws/mrf/opengm-master-2.3.1/src/external/MaxFlow-v3.02.src-patched)

# 
add_executable( main main.cc )
target_link_libraries( main ${OpenCV_LIBS} /home/tor/sun/ws/mrf/opengm-master-2.3.1/b2/src/external/libexternal-library-maxflow.a)