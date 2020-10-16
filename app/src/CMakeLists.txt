cmake_minimum_required(VERSION 3.18.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CPPS
    mainwindow.cpp
    optionswindow.cpp
    searchwindow.cpp
    LineNumberArea.cpp
    CodeEditor.cpp
    ClickableLabel.cpp
    tabWidget.cpp
    ProjectsView.cpp
    Highlighter.cpp
)

add_compile_options(
    -std=c++17
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

## QT INCLUDE
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_library(src ${CPPS})

target_link_libraries(src ${Qt5Widgets_LIBRARIES})
target_link_libraries(src ${Qt5PrintSupport_LIBRARIES})
