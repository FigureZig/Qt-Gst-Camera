# Set the minimum version of Cmake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.16)

# Set the project name and languages
project(Qt-Gst-Camera LANGUAGES CXX)

# CXX options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt optimization parameters
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt Core module include
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
if (NOT (QT_FOUND))
    message(FATAL_ERROR "Please Install Qt: Cmake will Exit")
else()
    message(STATUS "Qt installed")
endif()

# Qt additional include 
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
)

# PkgConfing include
find_package(PkgConfig REQUIRED)
if (NOT (PKGCONFIG_FOUND))
    message(FATAL_ERROR "Please Install PPkgConfig: CMake will Exit")
else()
    message(STATUS "PkgConfing installed")
endif()

# Gstreamer include
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0>=1.4
                               gstreamer-sdp-1.0>=1.4
                               gstreamer-video-1.0>=1.4
                               gstreamer-app-1.0>=1.4)
if (NOT (GST_FOUND))
    message(FATAL_ERROR "Please Install Gstreamer Dev: CMake will Exit")
else()
    message(STATUS "GStreamer installed")
endif()

# Cmake binding for Qt
include(qt.cmake)

# Defaut qt project setup
qt_standard_project_setup()

# Source files
set(PROJECT_SOURCES
    qdarkstyle/style.qrc
    qdarkstyle/style.qss    
    src/main.cpp
    src/mainwindow.ui
    src/mainwindow.h
    src/mainwindow.cpp
    src/styleloader.cpp 
    src/styleloader.h
    src/videoplayer.cpp
    src/videoplayer.h
    src/gstreamerhandler.cpp
    src/gstreamerhandler.h
    src/imageprocessing.h
    src/imageprocessing.cpp
)

#Resources
QT6_ADD_RESOURCES(RESOURCES_FILES qdarkstyle/style.qrc)

# Including Gstreamer in a project
include_directories(${PROJECT_NAME} PRIVATE ${GST_INCLUDE_DIRS})
link_directories(${PROJECT_NAME} PRIVATE ${GST_LIBRARY_DIRS})

# Qt executables
qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${RESOURCES_FILES} )

if(WIN32)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)
endif()

# Linkage
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        ${GST_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
   RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/run")
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
    VS_STARTUP_PROJECT ${PROJECT_NAME})