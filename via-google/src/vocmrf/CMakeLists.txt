
add_executable ( 
  main 
  main.cc 
)

add_library( 
  shotton 
  shotton.cc
  edge_potential.cc
)

target_link_libraries ( 
  main 
  shotton
  probimage
  color_map
  util
  ${OpenCV_LIBS} 
  ${OpenGMDir}/build/src/external/libexternal-library-maxflow.a 
  ${OpenGMDir}/build/src/external/libexternal-library-qpbo.a
)

