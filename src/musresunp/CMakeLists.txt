cmake_minimum_required (VERSION 3.0)

set (PROJECT musresunp) 
project (${PROJECT})

include_directories (
    AFTER ../.
    )

link_directories ( 
    )

set (HEADERS
    musresunp.h
) 
set (SOURCES
    musresunp.c
) 

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES}) 

set (LIBS
  )

if (WIN32)
    set(LIBS ${LIBS} kernel32)
endif ()


# main executable
add_executable (${PROJECT} ${HEADERS} ${SOURCES}) 
target_link_libraries (${PROJECT} ${LIBS})
