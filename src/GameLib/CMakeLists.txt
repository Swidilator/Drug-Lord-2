target_link_libraries(game_lib PUBLIC Qt6::Core)

target_sources(game_lib PUBLIC FILE_SET CXX_MODULES FILES
        Game.ixx
        World.ixx
        Location.ixx
        Item.ixx
        ItemCollection.ixx
        ItemMarket.ixx
        Wallet.ixx
        Inventory.ixx
        Character.ixx
        CharacterRank.ixx
        ItemMarketUpdater.ixx
        PubSub/PubSub.ixx
        PubSub/Publisher.ixx
        PubSub/Message.ixx
        PubSub/Subscriber.ixx
)