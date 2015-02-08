#set(CMAKE_CXX_COMPILER g++)
#set_source_files_properties(filename.cpp PROPERTIES LANGUAGE CXX)
#set( CMAK_C_COMPILER_INIT g++)

#SET(CMAKE_C_COMPILER mpicc)
#SET(CMAKE_CXX_COMPILER mpicxx)

add_library(struct_api svm_struct_api.cpp)
add_library(struct_learn_custom svm_struct_learn_custom.cpp)



target_link_libraries(struct_api shotton1 probimage util ${OpenCV_LIBS} 
  ${OpenGMDir}/build/src/external/libexternal-library-maxflow.a 
  ${OpenGMDir}/build/src/external/libexternal-library-qpbo.a
  ${QT_QTGUI_LIBRARY} 
  ${QT_QTCORE_LIBRARY})

target_link_libraries(struct_learn_custom struct_api)



add_subdirectory(svm_light)
add_subdirectory(svm_struct)

