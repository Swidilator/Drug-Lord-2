//
// Created by kyle on 2025/12/08.
//

#include "test_setup.h"

import Game.Item;
import Game.ItemCollection;

TEST_CASE("ItemCollection - A new ItemCollection is empty", "[ItemCollection]") {
    const ItemCollection<ItemType::Drug> ic{};
    CHECK(ic.total_items() == 0);
}

TEST_CASE("ItemCollection - An Item can be moved into an ItemCollection", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    Item<ItemType::Drug> i{"test_item"};
    ic.add_item(std::move(i));

    // Check inside ItemContainer
    auto stock_count = ic.get_stock_count();
    CHECK(stock_count["test_item"] == 1);
}

TEST_CASE("ItemCollection - An Item in an ItemCollection can be retrieved", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    {
        Item<ItemType::Drug> i{"test_item",};
        i.set_price_last_bought_at(30);
        ic.add_item(std::move(i));
    }

    auto io = ic.retrieve_item("test_item");
    CHECK(io.get_price_last_bought_at() == 30);
    CHECK(io.get_item_type() == ItemType::Drug);

    auto stock_count = ic.get_stock_count();
    CHECK(ic.total_items() == 0);
    CHECK(stock_count["test_item"] == 0);
}

TEST_CASE("ItemCollection - An item not in an ItemCollection cannot be retrieved", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    CHECK_THROWS_AS(ic.retrieve_item("test_item"), std::out_of_range);
}

TEST_CASE("ItemCollection - A retrieved stock count contains all items", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    auto stock_count_empty{ic.get_stock_count()};
    CHECK(ic.total_items() == 0);

    ic.add_item(Item<ItemType::Drug>{"test_item_1"});

    ic.add_item(Item<ItemType::Drug>{"test_item_2"});
    ic.add_item(Item<ItemType::Drug>{"test_item_2"});

    auto stock_count_full{ic.get_stock_count()};

    CHECK(ic.total_items() == 3);
    CHECK(stock_count_full["test_item_1"] == 1);
    CHECK(stock_count_full["test_item_2"] == 2);
}
