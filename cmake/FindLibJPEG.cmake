find_package(JPEG REQUIRED)

message(STATUS "JPEG_FOUND = ${JPEG_FOUND}")
message(STATUS "JPEG_INCLUDE_DIR = ${JPEG_INCLUDE_DIR}")
message(STATUS "JPEG_LIBRARIES = ${JPEG_LIBRARIES}")

include_directories(${JPEG_INCLUDE_DIR})

link_directories(${JPEG_LIBRARIES})