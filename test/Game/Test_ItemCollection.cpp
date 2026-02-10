//
// Created by kyle on 2025/12/08.
//

#include "test_setup.hpp"

import Game.Item;
import Game.ItemCollection;

TEST_CASE("ItemCollection: is empty when new", "[ItemCollection]") {
    const ItemCollection<ItemType::Drug> ic{};
    CHECK(ic.total_items() == 0);
}

TEST_CASE("ItemCollection: can be moved into an ItemCollection", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    Item<ItemType::Drug> i{"test_item"};
    ic.add_item(std::move(i));

    // Check inside ItemContainer
    auto stock_count = ic.stock_count();
    CHECK(stock_count["test_item"] == 1);
}

TEST_CASE("ItemCollection: can have an Item retrieved", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    {
        Item<ItemType::Drug> i{"test_item",};
        i.set_price_last_bought_at(30);
        ic.add_item(std::move(i));
    }

    auto io = ic.retrieve_item("test_item");
    CHECK(io.price_last_bought_at() == 30);
    CHECK(io.item_type() == ItemType::Drug);

    auto stock_count = ic.stock_count();
    CHECK(ic.total_items() == 0);
    CHECK(stock_count["test_item"] == 0);
}

TEST_CASE("ItemCollection: cannot retrieve items that don't exist", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    CHECK_THROWS_AS(ic.retrieve_item("test_item"), std::out_of_range);
}

TEST_CASE("ItemCollection: retrieved stock count contains all items", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    auto stock_count_empty{ic.stock_count()};
    CHECK(ic.total_items() == 0);

    ic.add_item(Item<ItemType::Drug>{"test_item_1"});

    ic.add_item(Item<ItemType::Drug>{"test_item_2"});
    ic.add_item(Item<ItemType::Drug>{"test_item_2"});

    auto stock_count_full{ic.stock_count()};

    CHECK(ic.total_items() == 3);
    CHECK(stock_count_full["test_item_1"] == 1);
    CHECK(stock_count_full["test_item_2"] == 2);
}

TEST_CASE("ItemCollection: can take all items out of another ItemCollection", "[ItemCollection]") {
    ItemCollection<ItemType::Drug> ic{};
    ItemCollection<ItemType::Drug> ic_other{};

    ic_other.add_item({"test_item_1"});
    ic_other.add_item({"test_item_2"});

    ic.take_all(ic_other);

    CHECK(ic.stock_count().size() == 2);
    CHECK(ic.stock_count()["test_item_1"] == 1);
    CHECK(ic.stock_count()["test_item_2"] == 1);

    CHECK(ic_other.stock_count().size() == 0);
    CHECK(ic_other.stock_count()["test_item_1"] == 0);
    CHECK(ic_other.stock_count()["test_item_2"] == 0);
}
