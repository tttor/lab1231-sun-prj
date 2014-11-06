
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
  util
  ${OpenCV_LIBS} 
  ${OpenGMDir}/b2/src/external/libexternal-library-maxflow.a
  ${OpenGMDir}/b2/src/external/libexternal-library-qpbo.a
)

