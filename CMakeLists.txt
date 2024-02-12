# Regrafusion - draws graphics recursively
# Distributed under GPL-3.0
# Copyright (C) 2023  Vesko Milev

cmake_minimum_required(VERSION 3.5)

project(Regrafusion VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/viewer.cpp
        inc/viewer.h
        viewer.ui
)

set(CMAKE_AUTOUIC_SEARCH_PATHS .)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Regrafusion
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        inc/displaywidget.h src/displaywidget.cpp
        inc/gfx/tree.h src/gfx/tree.cpp
        inc/gfx/branch.h src/gfx/branch.cpp
        inc/gfx/leaf.h src/gfx/leaf.cpp
        inc/gfx/leaves/spawnpoint.h src/gfx/leaves/spawnpoint.cpp
        inc/gfx/leaves/circle.h src/gfx/leaves/circle.cpp
        inc/gfx/leaves/line.h src/gfx/leaves/line.cpp
        inc/gfx/leaves/rectangle.h src/gfx/leaves/rectangle.cpp
        inc/gfx/leaves/path.h src/gfx/leaves/path.cpp
        inc/leaf_identifier.h src/leaf_identifier.cpp
        inc/rgf_ctx.h src/rgf_ctx.cpp
        inc/uipainter.h src/uipainter.cpp
        inc/shape_widget_event_filter.h src/shape_widget_event_filter.cpp
        inc/editors/editor.h src/editors/editor.cpp
        inc/editors/transform_editor.h src/editors/transform_editor.cpp
        inc/view.h
        inc/common.h
        inc/math_utils.h
        subdirs.pro
        icons.qrc
        icons/circle.png
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Regrafusion APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Regrafusion SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Regrafusion
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(Regrafusion PRIVATE ./inc)

target_link_libraries(Regrafusion PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Regrafusion PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Regrafusion)
endif()
set_target_properties(Regrafusion PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Regrafusion
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Regrafusion)
endif()
