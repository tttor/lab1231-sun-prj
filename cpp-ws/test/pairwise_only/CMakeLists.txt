cmake_minimum_required(VERSION 2.8)
project( experiment )

find_package( OpenCV REQUIRED )

include_directories (/home/jogie/ws/opengm-master-2.3.1/include)
include_directories (/home/jogie/ws/opengm-master-2.3.1/src/external/MaxFlow-v3.02.src-patched)
include_directories (/home/jogie/ws/pairwise_only/lib_vlfeat/)
include_directories (/home/jogie/ws/ProbImage)

#
add_library(probimage /home/jogie/ws/ProbImage/probimage.cpp)

add_library(vlfeat_slic lib_vlfeat/vl/host.c
lib_vlfeat/vl/random.c
lib_vlfeat/vl/generic.c
lib_vlfeat/vl/slic.c)

# 
add_executable( main main.cc )
target_link_libraries( main ${OpenCV_LIBS} /home/jogie/ws/opengm-master-2.3.1/b2/src/external/libexternal-library-maxflow.a /home/jogie/ws/pairwise_only/libvlfeat_slic.a probimage)


