add_executable (main main.cc) 

set (LIBS lab1231_sun_prj_gist boost_filesystem boost_program_options boost_system)
target_link_libraries (main ${LIBS}) 

# add the install targets, invoked by: $ make install
install (TARGETS main DESTINATION bin)
