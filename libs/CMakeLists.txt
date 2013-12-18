################# X11 #################
add_definitions( -DHAVE_X11=${X11_FOUND} )

################# liboxygenstyle #################
set(oxygenstyle_LIB_SRCS
    oxygenanimation.cpp
    oxygenhelper.cpp
    oxygenitemmodel.cpp
    oxygenshadowcache.cpp
    oxygentileset.cpp
    oxygenutil.cpp
)

kconfig_add_kcfg_files(oxygenstyle_LIB_SRCS
  oxygenactiveshadowconfiguration.kcfgc
  oxygeninactiveshadowconfiguration.kcfgc
)

add_library(oxygentransparentstyle SHARED ${oxygenstyle_LIB_SRCS})

target_link_libraries(oxygentransparentstyle Qt5::Core Qt5::Gui Qt5::Widgets )
target_link_libraries(oxygentransparentstyle KF5::ConfigCore KF5::ConfigWidgets KF5::GuiAddons)

if( X11_FOUND )
  target_link_libraries(oxygentransparentstyle ${XCB_LIBRARIES})
  target_link_libraries(oxygentransparentstyle Qt5::X11Extras)
endif()

set_target_properties(oxygentransparentstyle PROPERTIES VERSION 5.0.0 SOVERSION 5 )
install(TARGETS oxygentransparentstyle ${INSTALL_TARGETS_DEFAULT_ARGS} )

################# liboxygenstyleconfig #################
set(oxygenstyleconfig_LIB_SRCS
    oxygenanimationconfigitem.cpp
    oxygenbaseanimationconfigwidget.cpp
    oxygengenericanimationconfigitem.cpp
    oxygenshadowconfigwidget.cpp
)

set(oxygenstyleconfig_LIB_FORMS
    ui/oxygenanimationconfigitem.ui
    ui/oxygenanimationconfigwidget.ui
    ui/oxygengenericanimationconfigbox.ui
    ui/oxygenshadowconfigurationui.ui
)

qt5_wrap_ui(oxygenstyleconfig_LIB_FORMS_HEADERS ${oxygenstyleconfig_LIB_FORMS})

kconfig_add_kcfg_files(oxygenstyleconfig_LIB_SRCS
    oxygenactiveshadowconfiguration.kcfgc
    oxygeninactiveshadowconfiguration.kcfgc
)

add_library(oxygentransparentstyleconfig SHARED
    ${oxygenstyleconfig_LIB_SRCS}
    ${oxygenstyleconfig_LIB_FORMS_HEADERS} )

target_link_libraries(oxygentransparentstyleconfig Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(oxygentransparentstyleconfig KF5::I18n )
target_link_libraries(oxygentransparentstyleconfig oxygentransparentstyle)

set_target_properties(oxygentransparentstyleconfig PROPERTIES VERSION 5.0.0 SOVERSION 5 DEFINE_SYMBOL MAKE_OXYGENSTYLE_LIB )
install(TARGETS oxygentransparentstyleconfig ${INSTALL_TARGETS_DEFAULT_ARGS} )

# Note: no headers installed
