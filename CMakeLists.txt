cmake_minimum_required(VERSION 4.0)
project(Drug_Lord_2)

set(CMAKE_CXX_STANDARD 26)

add_library(game)

target_sources(game
        PUBLIC
        FILE_SET CXX_MODULES FILES
        Game/Character.ixx
        Game/Game.ixx
        Game/World.ixx
        Game/Location.ixx
        Game/Drug.ixx
)

add_executable(Drug_Lord_2 main.cxx)

target_link_libraries(Drug_Lord_2 game)

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests Game/Test/test.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain game)