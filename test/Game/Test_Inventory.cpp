//
// Created by kyle on 2025/12/16.
//

#include "test_setup.h"


import Game.Inventory;
import Game.Item;
import Game.ItemCollection;

TEST_CASE("Inventory: offers valid weak_ptr's to underlying ItemCollections", "[Inventory]") {
    Inventory i{};

    auto drugs_ptr{i.get_drugs_ptr().lock()};
    auto armours_ptr{i.get_armours_ptr().lock()};
    auto weapons_ptr{i.get_weapons_ptr().lock()};
    auto ammo_ptr{i.get_ammo_ptr().lock()};

    CHECK(drugs_ptr != nullptr);
    CHECK(armours_ptr != nullptr);
    CHECK(weapons_ptr != nullptr);
    CHECK(ammo_ptr != nullptr);
};

TEST_CASE("Inventory: underlying ItemContainers are empty on creation", "[Inventory]") {
    Inventory i{};

    CHECK(i.get_drugs_ptr().lock()->total_items() == 0);
    CHECK(i.get_armours_ptr().lock()->total_items() == 0);
    CHECK(i.get_weapons_ptr().lock()->total_items() == 0);
    CHECK(i.get_ammo_ptr().lock()->total_items() == 0);
};
