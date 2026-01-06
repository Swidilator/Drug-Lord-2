//
// Created by kyle on 2025/12/16.
//

#include "test_setup.h"


import Game.Inventory;
import Game.Item;
import Game.ItemCollection;

TEST_CASE("Inventory: automatically creates and offers valid weak_ptrs of any ItemType", "[Inventory]") {
    Inventory i{};

    auto drugs_ptr{i.get_collection_ptr<ItemType::Drug>()};

    CHECK(typeid(drugs_ptr) == typeid(std::weak_ptr<ItemCollection<ItemType::Drug>>));
    CHECK_NOTHROW(drugs_ptr.lock()->add_item({"test_item"}));
}

TEST_CASE("Inventory: underlying ItemContainers are empty on creation", "[Inventory]") {
    Inventory i{};

    CHECK(i.get_collection_ptr<ItemType::Drug>().lock()->total_items() == 0);
};
