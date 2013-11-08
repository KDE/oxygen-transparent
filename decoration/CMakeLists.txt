########### add version number into compilation defines
# FIXME: CPACK_PACKAGE_VERSION_* are empty
add_definitions (-DAPP_VERSION=\\\"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}\\\")
add_definitions(-DHAVE_X11=${X11_FOUND})
add_subdirectory(config)

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

kconfig_add_kcfg_files(kwin_oxygen_transparent_SRCS oxygenconfiguration.kcfgc )

kde4_add_plugin(kwin3_oxygen_transparent ${kwin_oxygen_transparent_SRCS})
target_link_libraries(kwin3_oxygen_transparent Qt5::Widgets )
target_link_libraries(kwin3_oxygen_transparent KF5::KConfigWidgets KF5::KGuiAddons KF5::KI18n KF5::KWindowSystem KF5::KStyle)
target_link_libraries(kwin3_oxygen_transparent kdecorations oxygentransparentstyle)

if(X11_FOUND)
  target_link_libraries(kwin3_oxygen_transparent ${XCB_XCB_LIBRARIES})
endif()

install(TARGETS kwin3_oxygen_transparent  DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############
install( FILES oxygentransparentclient.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kwin/ )
