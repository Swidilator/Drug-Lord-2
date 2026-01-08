//
// Created by kyle on 2026/01/06.
//

#include <test_setup.hpp>
import Game.CharacterRank;

TEST_CASE("CharacterRank: getter methods return initialisation values", "[CharacterRank]") {
    const CharacterRank c{"test_rank", 200000, 50000};
    CHECK(c.title() == "test_rank");
    CHECK(c.money_threshold() == 200000);
    CHECK(c.drug_inventory_size() == 50000);
}

TEST_CASE("CharacterRank: money threshold should be positive", "[CharacterRank]") {
    CHECK_NOTHROW(CharacterRank("test_rank", 200000, 50000));

    CHECK_THROWS_AS(CharacterRank("test_rank", -6, 50000), std::out_of_range);
}
