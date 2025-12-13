//
// Created by kyle on 2025/12/12.
//

#include <catch2/catch_test_macros.hpp>

import Game.Wallet;

TEST_CASE("A new wallet is empty with the default constructor", "[Wallet]") {
    constexpr Wallet w{};
    CHECK(w.get_balance() == 0);
}

TEST_CASE("A wallet cannot be created with a negative balance", "[Wallet]") {
    CHECK_THROWS_AS(Wallet{-2}, std::domain_error);
}

TEST_CASE("A new wallet has funds matching the constructor argument value", "[Wallet]") {
    const Wallet w{25};
    CHECK(w.get_balance() == 25);
}

TEST_CASE("A wallet can have funds added", "[Wallet]") {
    Wallet w{25};
    w.add_funds(26);
    CHECK(w.get_balance() == 51);
}

TEST_CASE("A wallet can have funds removed if it contains enough funds", "[Wallet]") {
    Wallet w{25};
    const auto result{w.remove_funds(16)};
    CHECK(result == true);
    CHECK(w.get_balance() == 9);
}

TEST_CASE("A wallet can't have more funds removed than it contains", "[Wallet]") {
    Wallet w{25};
    const auto result{w.remove_funds(27)};
    CHECK(result == false);
    CHECK(w.get_balance() == 25);
}
