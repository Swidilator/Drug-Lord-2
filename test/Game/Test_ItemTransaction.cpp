//
// Created by kyle on 2025/12/13.
//
#include <catch2/catch_test_macros.hpp>

import Game.ItemTransaction;
import Game.ItemCollection;
import Game.Item;

TEST_CASE("ItemTransaction - A new default ItemTransaction is ready to use", "[ItemTransaction]") {
    ItemTransaction it{};

    CHECK(it.check_success() == false);
    CHECK(it.check_extracted() == false);
    // This needs to happen after the extracted check, as this extracts it
    CHECK(it.extract_item_collection().get_stock_list().empty() == true);
}

TEST_CASE("ItemTransaction - An ItemTransaction moves an existing ItemCollection", "[ItemTransaction]") {
    ItemCollection ic{};

    ic.add_item(Item{"test_item", ItemType::Drug});
    CHECK(ic.get_stock_list().empty() == false);
    CHECK(ic.check_stock("test_item") == 1);

    ItemTransaction it{true, std::move(ic)};

    CHECK(ic.get_stock_list().empty() == true);
    CHECK(ic.check_stock("test_item") == 0);
}

TEST_CASE("ItemTransaction - Extracting an ItemTransaction retrieves its contents", "[ItemTransaction]") {
    Item i{"test_item", ItemType::Drug};
    i.set_price_last_bought_at(25);
    i.set_price_last_sold_at(30);

    ItemCollection ic{};
    ic.add_item(std::move(i));

    ItemTransaction it{true, std::move(ic)};

    auto ic2 = it.extract_item_collection();
    CHECK(it.check_extracted() == true);
    CHECK(ic2.get_stock_list().size() == 1);
    CHECK(ic2.check_stock("test_item") == 1);

    auto i2 = ic2.retrieve_item("test_item");
    CHECK(i2.get_item_type() == ItemType::Drug);
    CHECK(i2.get_price_last_bought_at() == 25);
    CHECK(i2.get_price_last_sold_at() == 30);
}

TEST_CASE("ItemTransaction - An extracted ItemTransaction cannot be re-extracted", "[ItemTransaction]") {
    ItemCollection ic{};
    ItemTransaction it{true, std::move(ic)};
    auto ic2 = it.extract_item_collection();
    CHECK_THROWS_AS(it.extract_item_collection(), std::logic_error);
}