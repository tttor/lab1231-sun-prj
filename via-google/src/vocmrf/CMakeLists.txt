
add_executable ( 
  vocmain 
  ./main.cc 
)

add_library( 
  shotton1 
  shotton.cc
  edge_potential.cc
)

target_link_libraries ( 
  vocmain 
  shotton1
  probimage
  color_map
  util
  ${OpenCV_LIBS} 
  ${OpenGMDir}/build/src/external/libexternal-library-maxflow.a 
  ${OpenGMDir}/build/src/external/libexternal-library-qpbo.a
)

