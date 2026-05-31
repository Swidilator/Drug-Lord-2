target_sources(tests PRIVATE
        Game/Test_Wallet.cpp
        Game/Test_ItemCollection.cpp
        Game/Test_Item.cpp
        Game/Test_Inventory.cpp
        Game/Test_ItemMarket.cpp
        Game/Test_Character.cpp
        Game/Test_CharacterRank.cpp
        Game/Test_Location.cpp

        Game/PubSub/Test_PubSub.cpp
)
target_sources(tests PRIVATE FILE_SET HEADERS FILES
        test_setup.hpp
)
target_link_libraries(tests PRIVATE game_lib Qt6::Core Catch2::Catch2WithMain)