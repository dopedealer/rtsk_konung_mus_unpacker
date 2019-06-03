cmake_minimum_required (VERSION 3.0)

set (PROJECT rtsk_musresunp)
project (${PROJECT})

#set (CMAKE_BUILD_TYPE Release) 

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/")
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/")


# build executables
add_subdirectory ("src/musresunp/" "${CMAKE_BINARY_DIR}/i/musresunp/")
