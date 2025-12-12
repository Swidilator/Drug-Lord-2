//
// Created by kyle on 2025/12/08.
//

#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"

import Game.Item;
import Game.ItemCollection;

TEST_CASE("ItemCollection::add_item", "[ItemCollection]") {
    constexpr int num_drugs_1 = 8;
    constexpr int num_drugs_2 = 200;

    auto dc = make_test_drug_collection(num_drugs_1, num_drugs_2);

    dc.add_item(Item("test_drug_1", ItemType::Drug));
    dc.add_item(Item("test_drug_2", ItemType::Drug));
    REQUIRE(dc.check_stock("test_drug_1") == num_drugs_1 + 1);
    REQUIRE(dc.check_stock("test_drug_2") == num_drugs_2 + 1);
}

TEST_CASE("ItemCollection::remove_item", "[ItemCollection]") {
    constexpr int num_drugs_1 = 8;
    constexpr int num_drugs_2 = 200;

    auto dc = make_test_drug_collection(num_drugs_1, num_drugs_2);

    dc.remove_item("test_drug_1");
    dc.remove_item("test_drug_2");
    REQUIRE(dc.check_stock("test_drug_1") == num_drugs_1 - 1);
    REQUIRE(dc.check_stock("test_drug_2") == num_drugs_2 - 1);
}

