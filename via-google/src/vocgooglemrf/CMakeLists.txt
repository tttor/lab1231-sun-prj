
add_executable ( 
  vocgooglemain
  main.cc
)

add_library( 
  shotton2 
  shotton.cc
  edge_potential.cc
)

target_link_libraries ( 
  vocgooglemain
  shotton2
  probimage
  color_map
  util
  ${OpenCV_LIBS} 
  ${OpenGMDir}/build/src/external/libexternal-library-maxflow.a 
  ${OpenGMDir}/build/src/external/libexternal-library-qpbo.a
  ${saliency_libs}
)

