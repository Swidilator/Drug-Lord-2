//
// Created by kyle on 2025/12/01.
//

#include <unordered_map>
#include <catch2/catch_test_macros.hpp>

import Game.Item;
import Game.ItemCollection;
import Game.ItemMarket;
import Game.Wallet;

auto make_test_drug_collection(const int& num_drugs_1, const int& num_drugs_2) -> ItemCollection {
    ItemCollection dc{};

    // Add 8 test_drug_1
    for (int i = 0; i < num_drugs_1; i++) {
        dc.add_item(Item("test_drug_1", ItemType::Drug));
    }

    // Add 200 test_drug_2
    for (int i = 0; i < num_drugs_2; i++) {
        dc.add_item(Item("test_drug_2", ItemType::Drug));
    }
    return std::move(dc);
}

TEST_CASE("Test make_test_drug_collection()", "[Item][ItemCollection]") {
    constexpr int num_drugs_1 = 8;
    constexpr int num_drugs_2 = 200;

    auto dc = make_test_drug_collection(num_drugs_1, num_drugs_2);
    REQUIRE(dc.check_stock("test_drug_1") == num_drugs_1);
    REQUIRE(dc.check_stock("test_drug_2") == num_drugs_2);

    dc.add_item(Item("test_drug_1", ItemType::Drug));
    dc.add_item(Item("test_drug_2", ItemType::Drug));
    REQUIRE(dc.check_stock("test_drug_1") == num_drugs_1 + 1);
    REQUIRE(dc.check_stock("test_drug_2") == num_drugs_2 + 1);

    dc.remove_item("test_drug_1");
    dc.remove_item("test_drug_2");
    REQUIRE(dc.check_stock("test_drug_1") == num_drugs_1);
    REQUIRE(dc.check_stock("test_drug_2") == num_drugs_2);
}


TEST_CASE("Wallet constructors", "[Wallet]") {
    auto wallet = std::make_shared<Wallet>();
    REQUIRE(wallet->get_balance() == 0);

    wallet = std::make_shared<Wallet>(75);
    REQUIRE(wallet->get_balance() == 75);
}

TEST_CASE("Wallet add and remove funds", "[Wallet]") {
    const auto wallet = std::make_shared<Wallet>(1000);

    wallet->add_funds(10);
    REQUIRE(wallet->get_balance() == 1010);

    bool remove_result = wallet->remove_funds(20);
    REQUIRE(wallet->get_balance() == 990);
    REQUIRE(remove_result == true);

    remove_result = wallet->remove_funds(1000);
    REQUIRE(wallet->get_balance() == 990);
    REQUIRE(remove_result == false);
}

TEST_CASE("ItemMarket constructors", "[ItemMarket]") {
    constexpr int num_drugs_1 = 8;
    constexpr int num_drugs_2 = 200;

    const ItemMarket im1{};
    REQUIRE(im1.get_collection().check_stock("test_drug_1") == 0);
    ItemMarket im2{make_test_drug_collection(num_drugs_1, num_drugs_2)};
    REQUIRE(im2.get_collection().check_stock("test_drug_1") == num_drugs_1);
}


TEST_CASE("Wallet and DrugMarket", "[Wallet][ItemMarket]") {
    constexpr int num_drugs_1 = 8;
    constexpr int num_drugs_2 = 200;

    auto wallet = std::make_shared<Wallet>();
    REQUIRE(wallet->get_balance() == 0);

    wallet->add_funds(1000);
    REQUIRE(wallet->get_balance() == 1000);

    ItemMarket drug_market{};
    REQUIRE(drug_market.get_collection().check_stock("test_drug") == 0);
    // Add test data to drug_market
    drug_market = ItemMarket{make_test_drug_collection(num_drugs_1, num_drugs_2)};
    const_cast<std::unordered_map<std::string, int>&>(drug_market.get_prices())["test_drug_1"] = 10;
    const_cast<std::unordered_map<std::string, int>&>(drug_market.get_prices())["test_drug_2"] = 12;

    REQUIRE(drug_market.get_prices().at("test_drug_1") == 10);
    REQUIRE(drug_market.get_prices().at("test_drug_2") == 12);

    auto purchase = drug_market.buy_drug("test_drug_1", 1, wallet);
    REQUIRE(purchase.check_success() == true);
    auto dc = purchase.extract_item_collection();
    REQUIRE(purchase.check_extracted() == true);

    REQUIRE(drug_market.get_collection().check_stock("test_drug_1") == 7);
    REQUIRE(dc.check_stock("test_drug_1") == 1);
    REQUIRE(wallet->get_balance() == 990);
}
