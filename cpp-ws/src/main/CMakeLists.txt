##
add_executable (create_ground_truth_image_of_stanford_background_dataset create_ground_truth_image_of_stanford_background_dataset.cc) 
target_link_libraries( create_ground_truth_image_of_stanford_background_dataset ${OpenCV_LIBS} )

##
add_executable (extract_gist extract_gist.cc) 

set (LIBS lab1231_sun_prj_gist boost_filesystem boost_program_options boost_system)
target_link_libraries (extract_gist ${LIBS}) 

##
add_executable (segment segment.cc)
target_link_libraries (segment lab1231_sun_prj_segment)

# add the install targets, invoked by: $ make install
set (EXE extract_gist segment)
install (TARGETS ${EXE} DESTINATION bin)
