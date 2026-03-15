cmake_minimum_required(VERSION 3.28)
project(Drug_Lord_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Gui Widgets WaylandClient)
find_package(Qt6WaylandClient CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)

qt_standard_project_setup()

# Shared Game library
qt_add_library(game_lib)
add_subdirectory(src/GameLib)

# No-Gui
qt_add_executable(main_console)
add_subdirectory(src/MainConsole)

# Gui
qt_add_executable(main_gui)
add_subdirectory(src/MainGui)

# Tests
qt_add_executable(tests)
add_subdirectory(test)
include(CTest)
include(Catch)
catch_discover_tests(tests)