include_directories(../)
add_library(struct_classify svm_struct_classify.cpp)
add_library(struct_learn svm_struct_learn.cpp)
add_library(struct_common svm_struct_common.cpp)
add_executable(do_learn svm_struct_main.cpp)
add_executable(do_classify svm_struct_classify.cpp)

target_link_libraries(struct_common svm_common struct_api)
target_link_libraries(struct_classify struct_common ${TBB_LIBRARIES})
target_link_libraries(struct_learn struct_common svm_learn ${TBB_LIBRARIES})


target_link_libraries(do_learn struct_learn struct_learn_custom struct_api svm_hideo svm_learn svm_common struct_common struct_learn_custom)

target_link_libraries(do_classify struct_learn struct_api struct_classify svm_common struct_common struct_learn_custom)