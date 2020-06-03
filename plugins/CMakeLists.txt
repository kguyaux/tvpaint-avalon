cmake_minimum_required (VERSION 2.8)
project (tvp_avalonplugins C CXX)

# Where the TVPaint_SDK lib-source-files are to be found:
set(TVP_SDK_LIB ${PROJECT_SOURCE_DIR}/../../TVPaint\ SDK/lib)
# Where the TVPaint_SDK header-files are to be found:
set(TVP_SDK_INCLUDE ${PROJECT_SOURCE_DIR}/../../TVPaint\ SDK/include)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/plugins/external/jWrite
    ${TVP_SDK_INCLUDE}
)

add_subdirectory(${PROJECT_SOURCE_DIR}/tvp_avalon-loader)
add_subdirectory(${PROJECT_SOURCE_DIR}/tvp_avalon-workfiles)
add_subdirectory(${PROJECT_SOURCE_DIR}/tvp_avalon-publish)
add_subdirectory(${PROJECT_SOURCE_DIR}/tvp_avalon-create)
add_subdirectory(${PROJECT_SOURCE_DIR}/tvp_avalon-startstop)


# Check for OS
if( UNIX )
    set(tvp_avalonplugins_DEFINITIONS
        "${tvp_avalonplugins_DEFINITIONS} -Wall -Wextra -Werror -Wno-deprecated-declarations -Wno-unused-parameter -Wno-comment" )
endif()

install(TARGETS avalon_workfilesplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )
install(TARGETS avalon_loaderplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )
install(TARGETS avalon_publishplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )
install(TARGETS avalon_createplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )
install(TARGETS avalon_startstopplugin DESTINATION /usr/share/tvpaint-developpement/tvp-animation-11-pro/plugins/ )

