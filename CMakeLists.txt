cmake_minimum_required( VERSION 3.0 )

project( KINGDOMS C )

set( sources
    main.c
)

add_executable( KINGDOMS ${sources} )

set_target_properties( KINGDOMS
    PROPERTIES
        OUTPUT_NAME KINGDOMS.PRG
        COMPILE_FLAGS "-t c64 -O"
        LINK_FLAGS "-t c64 -O"
)

#set(CMAKE_VERBOSE_MAKEFILE ON)