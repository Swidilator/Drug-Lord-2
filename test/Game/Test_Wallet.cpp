//
// Created by kyle on 2025/12/12.
//

#include "test_setup.h"

import Game.Wallet;

TEST_CASE("Wallet - A new wallet is empty with the default constructor", "[Wallet]") {
    constexpr Wallet w{};
    CHECK(w.get_balance() == 0);
}

TEST_CASE("Wallet - A wallet cannot be created with a negative balance", "[Wallet]") {
    CHECK_THROWS_AS(Wallet{-2}, std::domain_error);
}

TEST_CASE("Wallet - A new wallet has funds matching the constructor argument value", "[Wallet]") {
    const Wallet w{25};
    CHECK(w.get_balance() == 25);
}

TEST_CASE("Wallet - A wallet can have funds added", "[Wallet]") {
    Wallet w{25};
    const auto new_balance = w.add_funds(26);
    CHECK(new_balance == 51);
}

TEST_CASE("Wallet - A wallet can have funds removed if it contains enough funds", "[Wallet]") {
    Wallet w{25};
    const auto new_balance{w.remove_funds(16)};
    CHECK(new_balance == 9);
}

TEST_CASE("Wallet - A wallet can't have more funds removed than it contains", "[Wallet]") {
    Wallet w{25};
    CHECK_THROWS_AS(w.remove_funds(27), std::out_of_range);
    CHECK(w.get_balance() == 25);
}