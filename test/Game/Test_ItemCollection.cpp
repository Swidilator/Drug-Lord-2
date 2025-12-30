//
// Created by kyle on 2025/12/08.
//

#include "test_setup.h"

import Game.Item;
import Game.ItemCollection;

TEST_CASE("ItemCollection - A new ItemCollection is empty", "[ItemCollection]") {
    const ItemCollection ic{};
    CHECK(ic.get_stock_list().empty() == true);
}

TEST_CASE("ItemCollection - An Item can be moved into an ItemCollection", "[ItemCollection]") {
    ItemCollection ic{};
    Item i{"test_item", ItemType::Drug};
    ic.add_item(std::move(i));

    // Check inside ItemContainer
    CHECK(ic.check_stock("test_item") == 1);
    const auto [item_type, item_count] = ic.get_stock_list()["test_item"];
    CHECK(item_type == ItemType::Drug);
    CHECK(item_count == 1);
}

TEST_CASE("ItemCollection - An Item in an ItemCollection can be retrieved", "[ItemCollection]") {
    ItemCollection ic{};
    Item i{"test_item", ItemType::Drug};
    i.set_price_last_bought_at(30);
    ic.add_item(std::move(i));

    const auto io = ic.retrieve_item("test_item");
    CHECK(ic.get_stock_list().empty());
    CHECK(ic.check_stock("test_item") == 0);

    CHECK(io.get_price_last_bought_at() == 30);
}

TEST_CASE("ItemCollection - An item not in an ItemCollection cannot be retrieved", "[ItemCollection]") {
    ItemCollection ic{};
    CHECK_THROWS_AS(ic.retrieve_item("test_item"), std::out_of_range);
}

TEST_CASE("ItemCollection - Removing the last of an item_name will remove that category", "ItemCollection") {
    ItemCollection ic{};
    ic.add_item(Item{"test_item", ItemType::Drug});
    // Item category exists, does not throw
    CHECK(ic.get_stock_list().at("test_item").first == ItemType::Drug);

    auto i{ic.retrieve_item("test_item")};
    // Item category has been removed, throws
    CHECK_THROWS_AS(ic.get_stock_list().at("test_item"), std::out_of_range);
}

TEST_CASE("ItemCollection - A stock list can be retrieved from an ItemCollection", "[ItemCollection]") {
    ItemCollection ic{};
    const auto stock_list_empty{ic.get_stock_list()};
    CHECK(stock_list_empty.empty() == true);

    ic.add_item(Item{"test_item_1", ItemType::Drug});

    ic.add_item(Item{"test_item_2", ItemType::Ammo});
    ic.add_item(Item{"test_item_2", ItemType::Ammo});

    const auto stock_list_full{ic.get_stock_list()};

    CHECK(stock_list_full.empty() == false);
    CHECK(stock_list_full.at("test_item_1").first == ItemType::Drug);
    CHECK(stock_list_full.at("test_item_1").second == 1);

    CHECK(stock_list_full.at("test_item_2").first == ItemType::Ammo);
    CHECK(stock_list_full.at("test_item_2").second == 2);
}