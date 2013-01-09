########### add version number into compilation defines
add_definitions ( -DAPP_VERSION=\\\"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}\\\")
add_subdirectory( config )

########### next target ###############
set(kwin_oxygen_transparent_SRCS
     oxygenbutton.cpp
     oxygenclient.cpp
     oxygenclientgroupitemdata.cpp
     oxygendecohelper.cpp
     oxygenexceptionlist.cpp
     oxygenfactory.cpp
     oxygensizegrip.cpp
     oxygentitleanimationdata.cpp
  )

kde4_add_kcfg_files(kwin_oxygen_transparent_SRCS oxygenconfiguration.kcfgc )

kde4_add_plugin(kwin3_oxygen_transparent ${kwin_oxygen_transparent_SRCS})
target_link_libraries(
    kwin3_oxygen_transparent
    ${KDE4_KDEUI_LIBS}
    ${X11_X11_LIB}
    ${X11_Xrender_LIB}
    ${KDE4WORKSPACE_KDECORATIONS_LIBS}
    oxygentransparentstyle
  )

install(TARGETS kwin3_oxygen_transparent  DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############
install( FILES oxygentransparentclient.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kwin/ )

