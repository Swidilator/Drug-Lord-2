//
// Created by kyle on 2025/12/16.
//

#include <unordered_map>

#include "test_setup.h"

import Game.Item;
import Game.ItemMarket;
import Game.ItemCollection;
import Game.Wallet;

template<ItemType T>
auto setup_item_market() -> ItemMarket<T> {
    std::unordered_map<std::string, int> item_prices{
        {"test_item_1", 5},
        {"test_item_2", 4}
    };

    ItemCollection<T> ic{};

    // Add test items
    ic.add_item({"test_item_1"});
    ic.add_item({"test_item_2"});

    return {std::move(ic), item_prices};
}

TEST_CASE("ItemMarket: can be bought from", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>();

    // Buy - Does work
    auto purchase_ic = market.buy_items(
        {{"test_item_1", 1}},
        wallet
    );
    CHECK(wallet->get_balance() == 195);
    CHECK(purchase_ic.total_items() == 1);
    CHECK(purchase_ic.get_stock_count()["test_item_1"] == 1);
    CHECK(market.total_items() == 1);
    CHECK(market.get_stock_count()["test_item_1"] == 0);
};

TEST_CASE("ItemMarket: can be sold to", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
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
    CHECK(wallet->get_balance() == 205);
    CHECK(market.get_stock_count().at("test_item_1") == 2);
};

TEST_CASE("ItemMarket: only allows buying items that have a price", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>();

    // Buy - Does not work
    CHECK_THROWS_AS(market.buy_items(
                        {{"test_item_3", 1}},
                        wallet
                    ), std::domain_error);
    CHECK(wallet->get_balance() == 200);
};

TEST_CASE("ItemMarket: allows only selling items that have a price", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
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
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>();

    auto purchase_ic = market.buy_items(
        {{"test_item_1", 1}, {"test_item_2", 1}},
        wallet
    );

    CHECK(purchase_ic.total_items() == 2);
    CHECK(purchase_ic.get_stock_count()["test_item_1"] == 1);
    CHECK(purchase_ic.get_stock_count()["test_item_2"] == 1);

    CHECK(wallet->get_balance() == 191);

    CHECK(market.get_stock_count()["test_item_1"] == 0);
    CHECK(market.get_stock_count()["test_item_2"] == 0);
};

TEST_CASE("ItemMarket: can accept multiple items of different names when selling", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>(); {
        ItemCollection<ItemType::Drug> ic{};
        ic.add_item({"test_item_1"});
        ic.add_item({"test_item_1"});
        ic.add_item({"test_item_2"});

        market.sell_items(std::move(ic), wallet);
    }

    CHECK(market.total_items() == 5);
    auto sc{market.get_stock_count()};
    CHECK(sc["test_item_1"] == 3);
    CHECK(sc["test_item_2"] == 2);

    CHECK(wallet->get_balance() == 214);
};

TEST_CASE("ItemMarket: total_items() shows the number of items in the underlying ItemCollection", "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.total_items() == 2);
};

TEST_CASE("ItemMarket: get_stock_count() has entries for each price even when stock is empty", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.total_items() == 2);

    auto out = market.buy_items({{"test_item_1", 1}}, wallet);

    CHECK(market.get_stock_count().contains("test_item_1") == true);
    CHECK(market.get_stock_count().at("test_item_1") == 0);
};

TEST_CASE("ItemMarket: offers const references of underlying resources", "[ItemMarket]") {
    auto market = setup_item_market<ItemType::Drug>();

    CHECK(market.get_prices().at("test_item_1") == 5);
    CHECK(market.get_collection().total_items() == 2);
}

TEST_CASE("ItemMarket: throws when wallet pointer is invalid when buying and selling", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    wallet.reset();
    auto market = setup_item_market<ItemType::Drug>();

    CHECK_THROWS_AS(market.buy_items({{"test_item_1", 1}}, wallet), std::logic_error);

    ItemCollection<ItemType::Drug> ic{};
    ic.add_item({"test_item_1"});
    CHECK_THROWS_AS(market.sell_items(std::move(ic), wallet), std::logic_error);
}

TEST_CASE("ItemMarket: throws when wallet has insufficient funds when buying", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(4);
    auto market = setup_item_market<ItemType::Drug>();

    CHECK_THROWS_AS(market.buy_items({{"test_item_1", 1}}, wallet), std::range_error);
}

TEST_CASE("ItemMarket: throws when insufficient items when buying", "[ItemMarket]") {
    auto wallet = std::make_shared<Wallet>(200);
    auto market = setup_item_market<ItemType::Drug>();

    CHECK_THROWS_AS(market.buy_items({{"test_item_1", 2}}, wallet), std::range_error);
}

TEST_CASE("ItemMarket: throws when non-priced items are present on creation", "[ItemMarket]") {
    std::unordered_map<std::string, int> item_prices{
        {"test_item_1", 5}
    };

    ItemCollection<ItemType::Drug> ic{};

    // Add test items
    ic.add_item({"test_item_2"});

    try {
        ItemMarket a{std::move(ic), item_prices};
    } catch (const std::domain_error&) {
        CHECK(true);
    } catch (...) {
        CHECK(false);
    }
}
