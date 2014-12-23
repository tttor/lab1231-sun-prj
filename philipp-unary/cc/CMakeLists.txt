project(philipp-unary)
cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(${QT_INCLUDES} external/include)

add_subdirectory( src )
add_subdirectory( external )