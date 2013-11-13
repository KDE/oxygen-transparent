
set(kwin_oxygen_config_PART_SRCS
    oxygenanimationconfigwidget.cpp
    oxygenconfig.cpp
    oxygenconfigwidget.cpp
    oxygendetectwidget.cpp
    oxygenexceptiondialog.cpp
    oxygenexceptionlistwidget.cpp
    oxygenexceptionmodel.cpp
    ../oxygenexceptionlist.cpp
  )

kconfig_add_kcfg_files(kwin_oxygen_config_PART_SRCS ../oxygenconfiguration.kcfgc )

set( kwin_oxygen_config_PART_FORMS
   ui/oxygenconfigurationui.ui
   ui/oxygendetectwidget.ui
   ui/oxygenexceptiondialog.ui
   ui/oxygenexceptionlistwidget.ui
)

qt5_wrap_ui(kwin_oxygen_config_PART_FORMS_HEADERS ${kwin_oxygen_config_PART_FORMS})
add_library(kwin_oxygen_transparent_config MODULE
    ${kwin_oxygen_config_PART_SRCS}
    ${kwin_oxygen_config_PART_FORMS_HEADERS})

target_link_libraries(kwin_oxygen_transparent_config Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
target_link_libraries(kwin_oxygen_transparent_config KF5::KWindowSystem KF5::KCompletion)
target_link_libraries(kwin_oxygen_transparent_config kdecorations oxygentransparentstyle  oxygentransparentstyleconfig)

if(X11_FOUND)
  target_link_libraries(kwin_oxygen_transparent_config ${XCB_LIBRARIES})
  target_link_libraries(kwin_oxygen_transparent_config Qt5::X11Extras)
endif()

install(TARGETS kwin_oxygen_transparent_config DESTINATION ${QT_PLUGIN_INSTALL_DIR}/kf5)
