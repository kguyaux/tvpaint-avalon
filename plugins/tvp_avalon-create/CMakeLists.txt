set( tvp_avalon_createplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( avalon_createplugin_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tvp_avalon_create.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_client.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_server.c
    ${CMAKE_SOURCE_DIR}/lib/tokenizer.cpp
    ${TVP_SDK_LIB}/dllx.c
)

add_library( avalon_createplugin SHARED ${avalon_createplugin_SOURCES})
# prevent "lib"-prefix:
set_property(TARGET avalon_createplugin PROPERTY PREFIX "")
set_property(TARGET avalon_createplugin PROPERTY POSITION_INDEPENDENT_CODE ON)

