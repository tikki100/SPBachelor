find_package(PNG REQUIRED)

message(STATUS "PNG_FOUND = ${PNG_FOUND}")
message(STATUS "PNG_INCLUDE_DIR = ${PNG_INCLUDE_DIR}")
message(STATUS "PNG_LIBRARIES = ${PNG_LIBRARIES}")

include_directories(${PNG_INCLUDE_DIR})

link_directories(${PNG_LIBRARIES})