
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

kde4_add_ui_files(kwin_oxygen_config_PART_SRCS
   ui/oxygenconfigurationui.ui
   ui/oxygendetectwidget.ui
   ui/oxygenexceptiondialog.ui
   ui/oxygenexceptionlistwidget.ui
)

kde4_add_kcfg_files(kwin_oxygen_config_PART_SRCS ../oxygenconfiguration.kcfgc )

kde4_add_plugin(kwin_oxygen_transparent_config ${kwin_oxygen_config_PART_SRCS})

target_link_libraries(
    kwin_oxygen_transparent_config
    ${KDE4_KDEUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${X11_X11_LIB}
    ${KDE4WORKSPACE_KDECORATIONS_LIBS}
    oxygentransparentstyle
    oxygentransparentstyleconfig
  )

install(TARGETS kwin_oxygen_transparent_config  DESTINATION ${PLUGIN_INSTALL_DIR} )
