//
// Created by kyle on 2025/12/13.
//
#include "test_setup.h"

import Game.Item;
import Game.ItemCollection;
import Game.ItemTransaction;


// TEST_CASE("ItemTransaction - A new default ItemTransaction is ready to use", "[ItemTransaction]") {
//     ItemTransaction<ItemType::Drug> it{};
//
//     CHECK(it.check_success() == false);
//     CHECK(it.check_extracted() == false);
//     // This needs to happen after the extracted check, as this extracts it
//     CHECK(it.extract_item_collection().get_stock_count().get_item_stock().empty() == true);
// }
//
// TEST_CASE("ItemTransaction - An ItemTransaction moves an existing ItemCollection", "[ItemTransaction]") {
//     ItemCollection<ItemType::Drug> ic{};
//
//     ic.add_item(Item<ItemType::Drug>{"test_item"});
//     auto sc = ic.get_stock_count();
//     CHECK(sc.get_item_stock().empty() == false);
//     CHECK(sc.get_item_stock().at("test_item") == 1);
//
//     ItemTransaction it{true, std::move(ic)};
//
//     sc = ic.get_stock_count();
//     CHECK(sc.total_items() == 0);
//     CHECK(sc.get_item_stock().at("test_item") == 0);
// }
//
// TEST_CASE("ItemTransaction - Extracting an ItemTransaction retrieves its contents", "[ItemTransaction]") {
//     Item<ItemType::Drug> i{"test_item"};
//     i.set_price_last_bought_at(25);
//     i.set_price_last_sold_at(30);
//
//     ItemCollection<ItemType::Drug> ic{};
//     ic.add_item(std::move(i));
//
//     ItemTransaction<ItemType::Drug> it{true, std::move(ic)};
//
//     auto ic2 = it.extract_item_collection();
//     CHECK(it.check_extracted() == true);
//     auto sc = ic2.get_stock_count();
//     CHECK(sc.total_items() == 1);
//     CHECK(sc.get_item_stock().at("test_item") == 1);
//
//     auto i2 = ic2.retrieve_item("test_item");
//     CHECK(i2.get_item_type() == ItemType::Drug);
//     CHECK(i2.get_price_last_bought_at() == 25);
//     CHECK(i2.get_price_last_sold_at() == 30);
// }
//
// TEST_CASE("ItemTransaction - An extracted ItemTransaction cannot be re-extracted", "[ItemTransaction]") {
//     ItemCollection<ItemType::Drug> ic{};
//     ItemTransaction it{true, std::move(ic)};
//     auto ic2 = it.extract_item_collection();
//     CHECK_THROWS_AS(it.extract_item_collection(), std::logic_error);
// }
