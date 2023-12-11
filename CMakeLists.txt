cmake_minimum_required( VERSION 3.0 )
project( project )

set( SOURCES
	src/main.cpp
)

add_executable( project ${SOURCES} )

set_target_properties( project PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add compile options
target_compile_options( project PUBLIC -Wall -Wextra )

if( CMAKE_EXPORT_COMPILE_COMMANDS )
  set( CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES} )
endif()
