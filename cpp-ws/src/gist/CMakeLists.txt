add_definitions(-DUSE_GIST -DSTANDALONE_GIST)
add_library (lab1231_sun_prj_gist lear_gist_wrapper.cc lear_gist-1.2/standalone_image.c lear_gist-1.2/gist.c)
target_link_libraries (lab1231_sun_prj_gist m fftw3f lab1231_sun_prj_util ${OpenCV_LIBS})
