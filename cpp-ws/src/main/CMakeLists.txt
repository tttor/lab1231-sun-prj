add_executable (extract_gist extract_gist.cc) 

set (LIBS lab1231_sun_prj_gist boost_filesystem boost_program_options boost_system)
target_link_libraries (extract_gist ${LIBS}) 

# add the install targets, invoked by: $ make install
install (TARGETS extract_gist DESTINATION bin)
