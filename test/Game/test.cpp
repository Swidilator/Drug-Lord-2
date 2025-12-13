//
// Created by kyle on 2025/12/01.
//

#include <unordered_map>
#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"

import Game.Item;
import Game.ItemCollection;
import Game.ItemMarket;
import Game.Wallet;

TEST_CASE("Use TDD,Behaviour-Driven Design, and Black Box Testing") {
    std::string youtube_video = "https://youtu.be/MwoAM3sznS0?si=PMcZnm5_vrE7lQ-e&t=2622";
    CHECK(false);
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
