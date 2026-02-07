//
// Created by kyle on 2025/12/16.
//

#include <unordered_map>

#include "test_setup.hpp"

import Game.Item;
import Game.ItemMarket;
import Game.ItemCollection;
import Game.Wallet;

template <ItemType T>
auto setup_item_market() -> ItemMarket<T> {
    ItemPrices item_prices{
        {"test_item_1", 5},
        {"test_item_2", 4}
    };

    ItemCollection<T> ic{};

    // Add test items
    ic.add_item({"test_item_1"});
    ic.add_item({"test_item_2"});
    ic.add_item({"test_item_3"});

    return {std::move(ic), item_prices};
}

TEST_CASE("ItemMarket: can be bought from", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    // Buy - Does work
    auto purchase_ic = market.buy_items(
        {{"test_item_1", 1}},
        wallet
    );
    CHECK(wallet.balance() == 195);
    CHECK(purchase_ic.total_items() == 1);
    CHECK(purchase_ic.stock_count()["test_item_1"] == 1);
    CHECK(market.total_items() == 1);
    CHECK(market.stock_count()["test_item_1"] == 0);
};

TEST_CASE("ItemMarket: can be sold to", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    // Sell - Does work
    {
        ItemCollection<ItemType::Drug> sell_ic{};
        sell_ic.add_item({"test_item_1"});
        market.sell_items(
            std::move(sell_ic),
            wallet
        );
    }
    CHECK(wallet.balance() == 205);
    CHECK(market.stock_count().at("test_item_1") == 2);
};

TEST_CASE("ItemMarket: only allows buying items that have a price", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    // Buy - Does not work
    CHECK_THROWS_AS(market.buy_items(
                        {{"test_item_3", 1}},
                        wallet
                    ), std::domain_error);
    CHECK(wallet.balance() == 200);
};

TEST_CASE("ItemMarket: allows only selling items that have a price", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    // Sell - Does not work
    ItemCollection<ItemType::Drug> sell_ic_fails{};
    sell_ic_fails.add_item({"test_item_3"});
    CHECK_THROWS_AS(
        market.sell_items(
            std::move(sell_ic_fails),
            wallet
        ),
        std::domain_error
    );
};

TEST_CASE("ItemMarket: can accept multiple items of different names when buying", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    auto purchase_ic = market.buy_items(
        {{"test_item_1", 1}, {"test_item_2", 1}},
        wallet
    );

    CHECK(purchase_ic.total_items() == 2);
    CHECK(purchase_ic.stock_count()["test_item_1"] == 1);
    CHECK(purchase_ic.stock_count()["test_item_2"] == 1);

    CHECK(wallet.balance() == 191);

    CHECK(market.stock_count()["test_item_1"] == 0);
    CHECK(market.stock_count()["test_item_2"] == 0);
};

TEST_CASE("ItemMarket: can accept multiple items of different names when selling", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();
    {
        ItemCollection<ItemType::Drug> ic{};
        ic.add_item({"test_item_1"});
        ic.add_item({"test_item_1"});
        ic.add_item({"test_item_2"});

        market.sell_items(std::move(ic), wallet);
    }

    CHECK(market.total_items() == 5);
    auto sc{market.stock_count()};
    CHECK(sc["test_item_1"] == 3);
    CHECK(sc["test_item_2"] == 2);

    CHECK(wallet.balance() == 214);
};

TEST_CASE("ItemMarket: total_items() shows the number of items in the underlying ItemCollection that have a price",
          "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.total_items() == 2);
};

TEST_CASE("ItemMarket: get_stock_count() has entries for all valid priced entries even when stock is empty",
          "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.total_items() == 2);

    auto out = market.buy_items({{"test_item_1", 1}}, wallet);

    CHECK(market.stock_count().contains("test_item_1") == true);
    CHECK(market.stock_count().at("test_item_1") == 0);
};

TEST_CASE("ItemMarket: offers const references of underlying resources", "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.prices().at("test_item_1") == 5);
    CHECK(market.item_collection().total_items() == 3);
}

TEST_CASE("ItemMarket: throws when wallet has insufficient funds when buying", "[ItemMarket]") {
    auto wallet = Wallet{4};
    auto market = setup_item_market<ItemType::Drug>();

    CHECK_THROWS_AS(market.buy_items({{"test_item_1", 1}}, wallet), std::range_error);
}

TEST_CASE("ItemMarket: throws when insufficient items when buying", "[ItemMarket]") {
    auto wallet = Wallet{200};
    auto market = setup_item_market<ItemType::Drug>();

    CHECK_THROWS_AS(market.buy_items({{"test_item_1", 2}}, wallet), std::range_error);
}

TEST_CASE("ItemMarket: allows items to be directly added to and removed from underlying item collection",
          "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.item_collection().stock_count().at("test_item_1") == 1);
    market.add_item({"test_item_1"});
    CHECK(market.item_collection().stock_count().at("test_item_1") == 2);

    auto i{market.retrieve_item("test_item_1")};
    CHECK(market.item_collection().stock_count().at("test_item_1") == 1);
}

TEST_CASE("ItemMarket: allows replacing the ItemPrices member", "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();
    CHECK(market.prices().at("test_item_1") == 5);
    CHECK(market.prices().size() == 2);

    market.set_prices({
        {"test_item_1", 6},
        {"test_item_2", 4},
        {"test_item_3", 8}
    });

    CHECK(market.prices().at("test_item_1") == 6);
    CHECK(market.prices().at("test_item_3") == 8);
    CHECK(market.prices().size() == 3);
}

TEST_CASE("update_item_market: correctly changes ItemMarket to match parameters", "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    ItemPrices proposed_item_prices{
        {
            {"test_item_1", 6},
            {"test_item_2", 4},
            {"test_item_3", 8}
        }
    };

    ItemStockCount proposed_item_stock_count{
        {
            {"test_item_1", 7},
            {"test_item_2", 4},
            {"test_item_3", 0}
        }
    };

    update_item_market(market, proposed_item_prices, proposed_item_stock_count);

    ItemPrices updated_item_prices{market.prices()};
    ItemStockCount updated_item_stock_count{market.stock_count()};

    CHECK(updated_item_prices.size() == 3);
    CHECK(updated_item_prices.at("test_item_1") == 6);
    CHECK(updated_item_prices.at("test_item_2") == 4);
    CHECK(updated_item_prices.at("test_item_3") == 8);

    CHECK(updated_item_stock_count.size() == 3);
    CHECK(updated_item_stock_count.at("test_item_1") == 7);
    CHECK(updated_item_stock_count.at("test_item_2") == 4);
    CHECK(updated_item_stock_count.at("test_item_3") == 0);
}
