cmake_minimum_required(VERSION 3.28)
project(Drug_Lord_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core)
find_package(Catch2 3 REQUIRED)

qt_standard_project_setup()

# Shared Game library
qt_add_library(game_lib)
target_sources(game_lib
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/Game/Game.ixx
        src/Game/World.ixx
        src/Game/Location.ixx
        src/Game/Item.ixx
        src/Game/ItemCollection.ixx
        src/Game/ItemMarket.ixx
        src/Game/Wallet.ixx
        src/Game/Inventory.ixx
        src/Game/Character.ixx
        src/Game/CharacterRank.ixx
        src/Game/ItemMarketUpdater.ixx
)
target_link_libraries(game_lib PRIVATE Qt6::Core)

qt_add_executable(main src/main.cxx)
target_link_libraries(main PRIVATE game_lib Qt6::Core)


# These tests can use the Catch2-provided main
qt_add_executable(tests
        test/Game/Test_Wallet.cpp
        test/Game/Test_ItemCollection.cpp
        test/Game/Test_Item.cpp
        test/Game/Test_Inventory.cpp
        test/Game/Test_ItemMarket.cpp
        test/Game/Test_Character.cpp
        test/Game/Test_CharacterRank.cpp
        test/Game/Test_Location.cpp
)
target_include_directories(tests PRIVATE test)
target_link_libraries(tests PRIVATE game_lib Qt6::Core Catch2::Catch2WithMain)


include(CTest)
include(Catch)
catch_discover_tests(tests)