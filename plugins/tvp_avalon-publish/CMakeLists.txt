set( tvp_avalon_publishplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( avalon_publishplugin_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tvp_avalon_publish.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_client.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_server.c
    ${CMAKE_SOURCE_DIR}/lib/tokenizer.cpp
    ${TVP_SDK_LIB}/dllx.c
)

add_library( avalon_publishplugin SHARED ${avalon_publishplugin_SOURCES})
# prevent "lib"-prefix:
set_property(TARGET avalon_publishplugin PROPERTY PREFIX "")
set_property(TARGET avalon_publishplugin PROPERTY POSITION_INDEPENDENT_CODE ON)

