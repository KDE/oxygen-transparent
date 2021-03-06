include_directories( ${KDE4_KDEUI_INCLUDES} )

########### next target ###############

set(kstyle_oxygen_config_PART_SRCS
    oxygenanimationconfigwidget.cpp
    oxygenblacklistmodel.cpp
    oxygenblacklistdialog.cpp
    oxygenfollowmouseanimationconfigitem.cpp
    oxygenstyleconfig.cpp
)

kde4_add_kcfg_files(kstyle_oxygen_config_PART_SRCS ../oxygenstyleconfigdata.kcfgc)

kde4_add_ui_files(kstyle_oxygen_config_PART_SRCS
    ui/oxygenblacklistdialog.ui
    ui/oxygenfollowmouseanimationconfigbox.ui
    ui/oxygenstyleconfig.ui
)

kde4_add_plugin(kstyle_oxygen_transparent_config ${kstyle_oxygen_config_PART_SRCS})

target_link_libraries(
    kstyle_oxygen_transparent_config
    ${KDE4_KDEUI_LIBS}
    oxygentransparentstyle
    oxygentransparentstyleconfig )

install(TARGETS kstyle_oxygen_transparent_config  DESTINATION ${PLUGIN_INSTALL_DIR} )

########### next target ###############
set( oxygen_settings_SOURCES
    oxygenconfigdialog.cpp
    main.cpp)

kde4_add_executable( oxygen-transparent-settings ${oxygen_settings_SOURCES} )

target_link_libraries( oxygen-transparent-settings
    ${KDE4_KDEUI_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY})

########### install files ###############
install (TARGETS oxygen-transparent-settings ${INSTALL_TARGETS_DEFAULT_ARGS} )
