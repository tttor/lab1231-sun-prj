
add_library( textonutil colorconvertion.cpp util.cpp labelimage.cpp image.cpp colorimage.cpp segmentationimage.cpp )
target_link_libraries( textonutil ${QT_QTGUI_LIBRARY} )
