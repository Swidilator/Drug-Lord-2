//
// Created by kyle on 2025/12/08.
//

#include "test_setup.h"

import Game.Item;
import Game.ItemCollection;
import Game.ItemCollectionStockCount;

TEST_CASE("ItemCollection - A new ItemCollection is empty", "[ItemCollection]") {
    const ItemCollection<ItemType::Drug> ic{};
    CHECK(ic.get_stock_count().empty() == true);
}

TEST_CASE("ItemCollection - An Item can be moved into an ItemCollection", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    Item<ItemType::Drug> i{"test_item"};
    ic.add_item(std::move(i));

    // Check inside ItemContainer
    const auto stock_count = ic.get_stock_count();
    CHECK(stock_count.get_item_type() == ItemType::Drug);
    CHECK(stock_count["test_item"] == 1);
}

TEST_CASE("ItemCollection - An Item in an ItemCollection can be retrieved", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    Item<ItemType::Drug> i{"test_item",};
    i.set_price_last_bought_at(30);
    ic.add_item(std::move(i));

    const auto io = ic.retrieve_item("test_item");
    const auto stock_count = ic.get_stock_count();
    CHECK(stock_count.empty() == true);
    CHECK(stock_count["test_item"] == 0);

    CHECK(io.get_price_last_bought_at() == 30);
}

TEST_CASE("ItemCollection - An item not in an ItemCollection cannot be retrieved", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    CHECK_THROWS_AS(ic.retrieve_item("test_item"), std::out_of_range);
}

TEST_CASE("ItemCollection - A retrieved stock count contains all items", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    const auto stock_count_empty{ic.get_stock_count()};
    CHECK(stock_count_empty.empty() == true);

    ic.add_item(Item<ItemType::Drug>{"test_item_1"});

    ic.add_item(Item<ItemType::Drug>{"test_item_2"});
    ic.add_item(Item<ItemType::Drug>{"test_item_2"});

    const auto stock_count_full{ic.get_stock_count()};

    CHECK(stock_count_full.empty() == false);
    CHECK(stock_count_full["test_item_1"] == 1);

    CHECK(stock_count_full["test_item_2"] == 2);
}

TEST_CASE("ItemCollection - A retrieved stock count is the same ItemType", "[ItemCollection]") {
    const ItemCollection<ItemType::Drug> ic1{};
    CHECK(ic1.get_stock_count().get_item_type() == ItemType::Drug);

    const ItemCollection<ItemType::Ammo> ic2{};
    CHECK(ic2.get_stock_count().get_item_type() == ItemType::Ammo);
}
