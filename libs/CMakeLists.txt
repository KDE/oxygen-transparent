set(oxygenstyle_LIB_SRCS
    oxygenanimation.cpp
    oxygenhelper.cpp
    oxygenitemmodel.cpp
    oxygenshadowcache.cpp
    oxygentileset.cpp
    oxygenutil.cpp
)

kde4_add_kcfg_files(oxygenstyle_LIB_SRCS
  oxygenactiveshadowconfiguration.kcfgc
  oxygeninactiveshadowconfiguration.kcfgc
)

kde4_add_library(oxygentransparentstyle SHARED ${oxygenstyle_LIB_SRCS})

target_link_libraries(oxygentransparentstyle ${KDE4_KDEUI_LIBS})

if ( X11_FOUND )
   target_link_libraries(oxygentransparentstyle ${X11_LIBRARIES})
endif( X11_FOUND )

set_target_properties(oxygentransparentstyle PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS oxygentransparentstyle ${INSTALL_TARGETS_DEFAULT_ARGS} )

# next target
set(oxygenstyleconfig_LIB_SRCS
    oxygenanimationconfigitem.cpp
    oxygenbaseanimationconfigwidget.cpp
    oxygengenericanimationconfigitem.cpp
    oxygenshadowconfigwidget.cpp
)

kde4_add_ui_files(oxygenstyleconfig_LIB_SRCS
    ui/oxygenanimationconfigitem.ui
    ui/oxygenanimationconfigwidget.ui
    ui/oxygengenericanimationconfigbox.ui
    ui/oxygenshadowconfigurationui.ui
)

kde4_add_kcfg_files(oxygenstyleconfig_LIB_SRCS
  oxygenactiveshadowconfiguration.kcfgc
  oxygeninactiveshadowconfiguration.kcfgc
)

kde4_add_library(oxygentransparentstyleconfig SHARED ${oxygenstyleconfig_LIB_SRCS})

target_link_libraries(oxygentransparentstyleconfig oxygentransparentstyle)
target_link_libraries(oxygentransparentstyleconfig ${KDE4_KDEUI_LIBS})

if ( X11_FOUND )
   target_link_libraries(oxygentransparentstyleconfig ${X11_LIBRARIES})
endif( X11_FOUND )

set_target_properties(oxygentransparentstyleconfig PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS oxygentransparentstyleconfig ${INSTALL_TARGETS_DEFAULT_ARGS} )

# Note: no headers installed
