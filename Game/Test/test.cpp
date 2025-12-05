//
// Created by kyle on 2025/12/01.
//

#include <unordered_map>
#include <catch2/catch_test_macros.hpp>

import Game.Drug;
import Game.DrugCollection;
import Game.Wallet;
import Game.DrugMarket;

auto make_test_drug_collection() -> DrugCollection {
    DrugCollection dc{};

    // Add 8 test_drug_1
    for (int i = 0; i < 8; i++) {
        dc.add_drug(std::make_unique<Drug>("test_drug_1"));
    }

    // Add 200 test_drug_2
    for (int i = 0; i < 200; i++) {
        dc.add_drug(std::make_unique<Drug>("test_drug_2"));
    }
    return std::move(dc);
}

TEST_CASE("Test make_test_drug_collection()", "[Drug][DrugCollection]") {
    auto dc = make_test_drug_collection();
    REQUIRE(dc.check_stock("test_drug_1") == 8);
    REQUIRE(dc.check_stock("test_drug_2") == 200);

    dc.add_drug(std::make_unique<Drug>("test_drug_1"));
    dc.add_drug(std::make_unique<Drug>("test_drug_2"));
    REQUIRE(dc.check_stock("test_drug_1") == 9);
    REQUIRE(dc.check_stock("test_drug_2") == 201);

    dc.remove_drug("test_drug_1");
    dc.remove_drug("test_drug_2");
    REQUIRE(dc.check_stock("test_drug_1") == 8);
    REQUIRE(dc.check_stock("test_drug_2") == 200);
}


TEST_CASE("Wallet constructors", "[Wallet]") {
    auto wallet = std::make_shared<Wallet>();
    REQUIRE(wallet->get_balance() == 0);

    wallet = std::make_shared<Wallet>(75);
    REQUIRE(wallet->get_balance() == 75);
}

TEST_CASE("Wallet add and remove funds", "[Wallet]") {
    auto wallet = std::make_shared<Wallet>(1000);

    wallet->add_funds(10);
    REQUIRE(wallet->get_balance() == 1010);

    bool remove_result = wallet->remove_funds(20);
    REQUIRE(wallet->get_balance() == 990);
    REQUIRE(remove_result == true);

    remove_result = wallet->remove_funds(1000);
    REQUIRE(wallet->get_balance() == 990);
    REQUIRE(remove_result == false);
}


TEST_CASE("Wallet and DrugMarket", "[Wallet][DrugMarket]") {
    auto wallet = std::make_shared<Wallet>();
    REQUIRE(wallet->get_balance() == 0);

    wallet->add_funds(1000);
    REQUIRE(wallet->get_balance() == 1000);

    DrugMarket drug_market{};
    REQUIRE(drug_market.get_collection().check_stock("test_drug") == 0);
    // Add test data to drug_market
    const_cast<DrugCollection&>(drug_market.get_collection()) = make_test_drug_collection();
    const_cast<std::unordered_map<std::string, int>&>(drug_market.get_prices())["test_drug_1"] = 10;
    const_cast<std::unordered_map<std::string, int>&>(drug_market.get_prices())["test_drug_2"] = 12;

    REQUIRE(drug_market.get_prices().at("test_drug_1") == 10);
    REQUIRE(drug_market.get_prices().at("test_drug_2") == 12);

    auto purchase = drug_market.buy_drug("test_drug_1", 1, wallet);
    REQUIRE(purchase.check_success() == true);
    auto dc = purchase.extract_drug_collection();
    REQUIRE(purchase.check_extracted() == true);

    REQUIRE(drug_market.get_collection().check_stock("test_drug_1") == 7);
    REQUIRE(dc.check_stock("test_drug_1") == 1);
    REQUIRE(wallet->get_balance() == 990);
}
