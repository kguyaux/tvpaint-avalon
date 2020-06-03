set( tvp_avalon_startstopplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( avalon_startstopplugin_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tvp_avalon-startstop_plugin.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_client.c
    ${TVP_SDK_LIB}/dllx.c
)

add_library( avalon_startstopplugin SHARED ${avalon_startstopplugin_SOURCES})
# prevent "lib"-prefix:
set_property(TARGET avalon_startstopplugin PROPERTY PREFIX "")
set_property(TARGET avalon_startstopplugin PROPERTY POSITION_INDEPENDENT_CODE ON)

