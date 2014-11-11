cmake_minimum_required (VERSION 2.6)
project(test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

find_package( OpenCV REQUIRED )


if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

add_definitions( -std=c++0x )

##########################################################
#Executable source file
set(SOURCE_DIR /home/rizkivmaster/ws/googleimages/executables/smrf)

add_executable( smrf ${SOURCE_DIR}/SMRF.cpp)

###########################################################
#SaliencyFilters library
set(SALIENCY_DIR /home/rizkivmaster/ws/saliencyfilters)
include_directories(${SALIENCY_DIR}/src)

set(saliency_libs 
${SALIENCY_DIR}/build/src/saliency/libsaliency.a
${SALIENCY_DIR}/build/src/filter/libfilter.a
${SALIENCY_DIR}/build/src/superpixel/libsuperpixel.a)



################################################################\
#add OPENGM
set(OPENGM_DIRS /home/rizkivmaster/ws/opengm)
add_library(maxflow ${OPENGM_DIRS}/src/external/MaxFlow-v3.02.src-patched/maxflow.cpp)
add_library(graph ${OPENGM_DIRS}/src/external/MaxFlow-v3.02.src-patched/graph.cpp)
include_directories (${OPENGM_DIRS}/include)
include_directories(${OPENGM_DIRS}/src/external/MaxFlow-v3.02.src-patched)

target_link_libraries(smrf ${OpenCV_LIBS} ${saliency_libs} maxflow graph)
