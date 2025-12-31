//
// Created by kyle on 2025/12/30.
//
#include "test_setup.h"

import Game.Item;
import Game.ItemCollection;
import Game.ItemCollectionStockCount;

TEST_CASE("ItemCollectionStockCount: An ItemCollectionStockCount is empty when passed an empty map",
          "[ItemCollectionStockCount]") {
    const ItemCollectionStockCount<ItemType::Drug> sc{{}};
    CHECK(sc.empty() == true);
    CHECK(sc.size() == 0);
}


TEST_CASE("ItemCollectionStockCount: get_item_type() should return the same type as when created",
          "[ItemCollectionStockCount]") {
    CHECK(ItemCollectionStockCount<ItemType::Drug>{{}}.get_item_type() == ItemType::Drug);
}

TEST_CASE("ItemCollectionStockCount: empty() should return true when all counts are zero",
          "[ItemCollectionStockCount]") {
    ItemCollectionStockCount<ItemType::Drug> sc{{}};
    CHECK(sc.empty() == true);

    sc = ItemCollectionStockCount<ItemType::Drug>{
        {
            {"test_item_1", 0},
            {"test_item_2", 0}
        }
    };
    CHECK(sc.empty() == true);

    sc = ItemCollectionStockCount<ItemType::Drug>{
        {
            {"test_item_1", 0},
            {"test_item_2", 1}
        }
    };
    CHECK(sc.empty() == false);
}

TEST_CASE("ItemCollectionStockCount: size() should return the count of non-zero stock items",
          "[ItemCollectionStockCount]") {
    ItemCollectionStockCount<ItemType::Drug> sc{{}};
    CHECK(sc.size() == 0);

    sc = ItemCollectionStockCount<ItemType::Drug>{
        {
            {"test_item_1", 0},
            {"test_item_2", 0}
        }
    };
    CHECK(sc.size() == 0);

    sc = ItemCollectionStockCount<ItemType::Drug>{
        {
            {"test_item_1", 0},
            {"test_item_2", 1}
        }
    };
    CHECK(sc.size() == 1);
}
