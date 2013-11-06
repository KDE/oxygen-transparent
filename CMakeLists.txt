cmake_minimum_required(VERSION 2.8.10.1)
project(oxygen-transparent)

include(FeatureSummary)

################# set KDE specific information #################
find_package(ECM 0.0.8 REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

################# Qt5 #################
find_package( Qt5 REQUIRED COMPONENTS Gui Widgets DBus )
find_package( Qt5 REQUIRED COMPONENTS PrintSupport Xml Network Concurrent Svg )

################# KF5 #################
# TODO remove it in future
find_package(LibAttica NO_MODULE)

find_package( KF5 MODULE REQUIRED COMPONENTS
    CMake
    Compiler
    InstallDirs
    KIdleTime
    ItemModels
    KWidgetsAddons
    KWindowSystem
    KCodecs
    KArchive
    KCoreAddons
    Solid
    ThreadWeaver
    KConfig
    KAuth
    KJS
    KWallet
    KDBusAddons
    Sonnet
    KI18n
    KGuiAddons
    KService
    KConfigWidgets
    ItemViews
    KNotifications
    KIconThemes
    KStyle
    KCompletion
    KJobWidgets
    KTextWidgets
    XmlGui
    KCrash
    KBookmarks
    Kross
    KIO
)

# needed for kde4_add_kcfg_file
find_package(KDELibs4 REQUIRED NO_MODULE)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    ${KF5_INCLUDE_DIRS}
    ${KF5_INCLUDE_DIRS}/KDE
    ${CMAKE_INSTALL_PREFIX}/include/KDE
)

################# X11 #################
find_package(X11 MODULE)
if(X11_FOUND)

    find_package(Qt5X11Extras REQUIRED NO_MODULE)
    include_directories( ${X11_X11_INCLUDE_PATH} )

    find_package(XCB REQUIRED COMPONENTS XCB)
    #find_package(XCB REQUIRED)
    #find_package(X11_XCB)

    add_definitions( -DHAVE_X11=${X11_FOUND} )

endif()

################# Enable C++11 features for clang and gcc #################
if(UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
endif()

add_subdirectory(libs)
add_subdirectory(style)
add_subdirectory(decoration)
