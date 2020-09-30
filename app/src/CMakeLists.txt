#cmake_minimum_required(VERSION 3.18.2)
#
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_STANDARD_REQUIRED YES)
#
#project(src LANGUAGES CXX)
##that is what tutorial on windows recommends to do, might not work
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
##set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
##set(SOURCE_EXE main.cpp)
#
##now you can add subdirectories
##add_subdirectory(src)
#
##that is for libraries:
##include_directories(src)
#
#add_compile_options(
#        -std=c++17
#        -Werror
#        -Wall
#        -Wextra
#        -Wpedantic
#        -Wno-unused-parameter
#)
#
##find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
###
##set(project_ui
##        mainWindow.ui)
##
##set(project_headers
##        mainWindow.h)
##
##set(project_sources
##        mainWindow.cpp)
##
##qt5_wrap_ui(project_headers_wrapped ${project_ui})
##qt5_wrap_cpp(projects_sources_moc ${projects_headers})
##
##add_library(src ${project_headers} ${project_sources}
##        ${projects_sources_moc} ${projects_headers_moc})
##
##target_link_libraries(${PROJECT_NAME}
##        PUBLIC
##        Qt5::Core
##        Qt5::Gui
##        Qt5::Widgets)
#
## Widgets finds its own dependencies (QtGui and QtCore).
#find_package(Qt5Widgets REQUIRED)
#
## The Qt5Widgets_INCLUDES also includes the include directories for
## dependencies QtCore and QtGui
#include_directories(${Qt5Widgets_INCLUDES})
#
## We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
#add_definitions(${Qt5Widgets_DEFINITIONS})
#
## Executables fail to build with Qt 5 in the default configuration
## without -fPIE. We add that here.
#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#
#add_library(src mainwindow.cpp)
#
## The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
#target_link_libraries(src ${Qt5Widgets_LIBRARIES})

cmake_minimum_required(VERSION 3.18.2)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_library(src mainwindow.cpp)

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(src ${Qt5Widgets_LIBRARIES})

