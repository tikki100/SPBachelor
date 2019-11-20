SET(program Main)

include_directories( ${CIMG_H_PATH} )

add_executable(
	${program} 
	main.cpp
	classMaze.cpp
	classHPAMaze.cpp
)

#target_link_libraries(${program} ${SYSTEM_LIBS} )

target_link_libraries(${program} ${X11_LIBRARIES} ${PNG_LIBRARIES} ${JPEG_LIBRARIES})
