//
// Created by kyle on 2025/12/12.
//

#include "test_setup.hpp"

import Game.Item;

// Test

TEST_CASE("Item: should have the name and ItemType from arguments", "[Item]") {
    const Item<ItemType::Drug> i{"test_item"};

    CHECK(i.name() == "test_item");
    CHECK(i.item_type() == ItemType::Drug);

    CHECK(i.price_last_bought_at() == 0);
    CHECK(i.price_last_sold_at() == 0);
}

TEST_CASE("Item: can have it's bought and sold values changed", "[Item]") {
    Item<ItemType::Drug> i{"test_item"};

    CHECK(i.price_last_bought_at() == 0);
    CHECK(i.price_last_sold_at() == 0);

    i.set_price_last_bought_at(25);
    i.set_price_last_sold_at(44);

    CHECK(i.price_last_bought_at() == 25);
    CHECK(i.price_last_sold_at() == 44);
}

TEST_CASE("Item: is moved correctly", "[Item]") {
    Item<ItemType::Drug> i{"test_item"};
    i.set_price_last_bought_at(25);
    i.set_price_last_sold_at(44);

    Item i2 = std::move(i);

    CHECK(i.name().empty() == true);
    CHECK(i.item_type() == ItemType::None);
    CHECK(i.price_last_bought_at() == 0);
    CHECK(i.price_last_sold_at() == 0);

    CHECK(i2.name() == "test_item");
    CHECK(i2.item_type() == ItemType::Drug);
    CHECK(i2.price_last_bought_at() == 25);
    CHECK(i2.price_last_sold_at() == 44);
}

TEST_CASE("Item: cannot have negative bought and sold values", "[Item]") {
    Item<ItemType::Drug> item{"test_item"};

    CHECK_THROWS_AS(item.set_price_last_bought_at(-1), std::out_of_range);
    CHECK_THROWS_AS(item.set_price_last_sold_at(-1), std::out_of_range);
}
