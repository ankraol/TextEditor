cmake_minimum_required(VERSION 3.18.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(app_project LANGUAGES CXX)
#that is what tutorial on windows recommends to do, might not work
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(${PROJECT_NAME} main.cpp)

link_directories(src)
include_directories(src)
target_link_libraries(${PROJECT_NAME} src)

target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES})

