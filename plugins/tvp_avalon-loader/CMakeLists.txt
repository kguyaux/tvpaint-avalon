set( tvp_avalon_loaderplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( avalon_loaderplugin_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tvp_avalon_loader.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_client.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_server.c
    ${CMAKE_SOURCE_DIR}/lib/tokenizer.cpp
    ${TVP_SDK_LIB}/dllx.c
)
add_library( avalon_loaderplugin SHARED ${avalon_loaderplugin_SOURCES})
set_property(TARGET avalon_loaderplugin PROPERTY PREFIX "")
set_property(TARGET avalon_loaderplugin PROPERTY POSITION_INDEPENDENT_CODE ON)

