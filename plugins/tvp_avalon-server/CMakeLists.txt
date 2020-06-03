cmake_minimum_required (VERSION 2.8)
project (tvp_avalon_serverplugin CXX)


set( tvp_avalon_serverplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
#set( tvp_avalon_serverplugin_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# Include Directories
# In GCC, this will invoke the "-I" command
#include_directories(
#    ${CMAKE_SOURCE_DIR}/../include
#    )

set( tvp_avalon_serverplugin_SOURCES
    tvp_avalon-server_plugin.c
    ${TVP_SDK_LIB}/dllx.c
    ${PROJECT_SOURCE_DIR}/external/jWrite/jWrite.c
    ../lib/tokenizer.cpp
)

set_source_files_properties(../lib/tokenizer.cpp PROPERTIES LANGUAGE CXX )

# instead of add_target, create a library
add_library( avalon_serverplugin SHARED ${tvp_avalon_serverplugin_SOURCE_DIR})

# prevent "lib"-prefix:
set_property(TARGET avalon_serverplugin PROPERTY PREFIX "")
set_property(TARGET avalon_serverplugin PROPERTY POSITION_INDEPENDENT_CODE ON)

## Check for OS
#if( UNIX )
#    set( tvp_avalon_serverplugin_DEFINITIONS
#        "${tvp_avalon_serverplugin_DEFINITIONS} -Wall -Wextra -Werror -Wno-deprecated-declarations -Wno-unused-parameter -Wno-comment" )
#endif()
#
#
#install(TARGETS avalon-serverplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )
#
