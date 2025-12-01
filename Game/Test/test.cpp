//
// Created by kyle on 2025/12/01.
//

#include <unordered_map>
#include <catch2/catch_test_macros.hpp>

import Game.Drug;
import Game.DrugCollection;
import Game.Wallet;
import Game.DrugMarket;


TEST_CASE("Wallet and DrugMarket", "[Wallet][DrugMarket]") {
    auto wallet = std::make_shared<Wallet>();
    REQUIRE(wallet->get_balance() == 0);

    wallet->add_funds(1000);
    REQUIRE(wallet->get_balance() == 1000);

    DrugMarket drug_market{};
    REQUIRE(drug_market.get_collection().check_stock("test_drug") == 0);
    // Add test data to drug_market
    const_cast<DrugCollection&>(drug_market.get_collection()).add_drug(std::make_unique<Drug>("test_drug"));
    const_cast<std::unordered_map<std::string, int>&>(drug_market.get_prices())["test_drug"] = 10;

    REQUIRE(drug_market.get_collection().check_stock("test_drug") == 1);
    REQUIRE(drug_market.get_prices().at("test_drug") == 10);

    auto purchase = drug_market.buy_drug("test_drug", 1, wallet);
    REQUIRE(drug_market.get_collection().check_stock("test_drug") == 0);
    REQUIRE(purchase.check_stock("test_drug") == 1);
    REQUIRE(wallet->get_balance() == 990);
}
