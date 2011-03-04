set(oxygentransparentstyle_LIB_SRCS oxygenanimation.cpp
                         oxygenhelper.cpp
                         oxygenitemmodel.cpp
                         oxygenshadowcache.cpp
                         oxygenshadowconfiguration.cpp
                         oxygentileset.cpp)

kde4_add_library(oxygentransparentstyle SHARED ${oxygentransparentstyle_LIB_SRCS})

target_link_libraries(oxygentransparentstyle ${KDE4_KDEUI_LIBS})

if ( X11_FOUND )
   target_link_libraries(oxygentransparentstyle ${X11_LIBRARIES})
endif( X11_FOUND )

set_target_properties(oxygentransparentstyle PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS oxygentransparentstyle ${INSTALL_TARGETS_DEFAULT_ARGS} )

# Note: no headers installed
