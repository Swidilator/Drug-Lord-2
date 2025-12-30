//
// Created by kyle on 2025/12/12.
//

#include "test_setup.h"

import Game.Item;

// Test

TEST_CASE("Item - A new Item should have the name and ItemType from arguments", "[Item]") {
    const Item i{"test_item", ItemType::Drug};

    CHECK(i.get_name() == "test_item");
    CHECK(i.get_item_type() == ItemType::Drug);

    CHECK(i.get_price_last_bought_at() == 0);
    CHECK(i.get_price_last_sold_at() == 0);
}

TEST_CASE("Item - An Item can have it's bought and sold values changed", "[Item]") {
    Item i{"test_item", ItemType::Drug};

    CHECK(i.get_price_last_bought_at() == 0);
    CHECK(i.get_price_last_sold_at() == 0);

    i.set_price_last_bought_at(25);
    i.set_price_last_sold_at(44);

    CHECK(i.get_price_last_bought_at() == 25);
    CHECK(i.get_price_last_sold_at() == 44);
}

TEST_CASE("Item - An Item is moved correctly", "[Item]") {
    Item i{"test_item", ItemType::Drug};
    i.set_price_last_bought_at(25);
    i.set_price_last_sold_at(44);

    Item i2 = std::move(i);

    CHECK(i.get_name().empty() == true);
    CHECK(i.get_item_type() == ItemType::None);
    CHECK(i.get_price_last_bought_at() == 0);
    CHECK(i.get_price_last_sold_at() == 0);

    CHECK(i2.get_name() == "test_item");
    CHECK(i2.get_item_type() == ItemType::Drug);
    CHECK(i2.get_price_last_bought_at() == 25);
    CHECK(i2.get_price_last_sold_at() == 44);
}
