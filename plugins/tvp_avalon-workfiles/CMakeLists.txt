set( tvp_avalon_workfilesplugin_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( tvp_avalon_workfilesplugin_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tvp_avalon_workfiles.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_client.c
    ${CMAKE_SOURCE_DIR}/lib/tvp_avalon_server.c
    ${CMAKE_SOURCE_DIR}/lib/tokenizer.cpp
    ${TVP_SDK_LIB}/dllx.c
)
set_source_files_properties(lib/tokenizer.cpp PROPERTIES LANGUAGE CXX )

add_library( avalon_workfilesplugin SHARED ${tvp_avalon_workfilesplugin_SOURCES})
# prevent "lib"-prefix:
set_property(TARGET avalon_workfilesplugin PROPERTY PREFIX "")
set_property(TARGET avalon_workfilesplugin PROPERTY POSITION_INDEPENDENT_CODE ON)
