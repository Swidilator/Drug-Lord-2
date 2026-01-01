cmake_minimum_required(VERSION 4.0)
project(Drug_Lord_2)

set(CMAKE_CXX_STANDARD 26)

add_library(game)

target_sources(game
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/Game/Character.ixx
        src/Game/Game.ixx
        src/Game/World.ixx
        src/Game/Location.ixx
        src/Game/Item.ixx
        src/Game/ItemCollection.ixx
        #src/Game/ItemCollectionStockCount.ixx
        src/Game/ItemMarket.ixx
        #src/Game/ItemTransaction.ixx
        src/Game/Wallet.ixx
)

add_executable(Drug_Lord_2 src/main.cxx)

target_link_libraries(Drug_Lord_2 game)

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests test/Game/test.cpp
        test/Game/Test_Wallet.cpp
        test/Game/Test_ItemCollection.cpp
        test/Game/Test_Item.cpp
        #test/Game/Test_ItemTransaction.cpp
        #test/Game/Test_ItemCollectionStockCount.cpp
)

target_include_directories(tests PRIVATE test)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain game)