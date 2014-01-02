cmake_minimum_required(VERSION 2.8.12)
project(oxygen-transparent)

include(FeatureSummary)

################# set KDE specific information #################
find_package(ECM 0.0.9 REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
include(ECMMarkNonGuiExecutable)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

################# Qt5 #################
find_package( Qt5 REQUIRED COMPONENTS Gui Widgets DBus )
find_package( Qt5 REQUIRED COMPONENTS PrintSupport Xml Network Concurrent Svg )

################# KF5 #################
# TODO remove it in future
find_package(LibAttica NO_MODULE)

find_package( KF5 CONFIG REQUIRED
    Config
    GuiAddons
    ConfigWidgets
    I18n
    IconThemes
    Completion
    Service
    WindowSystem
    Style
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

################# X11 #################
find_package(X11 MODULE)
if(X11_FOUND)

    find_package(Qt5X11Extras REQUIRED NO_MODULE)
    find_package(XCB REQUIRED COMPONENTS XCB)
    add_definitions( -DHAVE_X11=${X11_FOUND} )

endif()

################# Enable C++11 features for clang and gcc #################
if(UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
endif()

add_subdirectory(libs)
add_subdirectory(style)
add_subdirectory(decoration)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
