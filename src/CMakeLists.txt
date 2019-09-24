SET(program Main)

include_directories( ${CIMG_H_PATH} )

add_executable(
	${program} 
	main.cpp
)

target_link_libraries(${program} ${SYSTEM_LIBS} )