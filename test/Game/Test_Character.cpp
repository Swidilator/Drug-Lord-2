//
// Created by kyle on 2025/12/16.
//

#include <unordered_map>

#include "test_setup.hpp"

import Game.Item;
import Game.ItemCollection;
import Game.Character;
import Game.CharacterRank;
import Game.Wallet;

TEST_CASE("Character: should have 100 health on creation", "[Character]") {
    Character c{{"test_rank", 200000, 50000}};

    CHECK(c.health() == 100);
}

TEST_CASE("Character: wallets can be retrieved and mutated", "[Character]") {
    Character c{{"test_rank", 200000, 50000}};

    CHECK(typeid(c.cash_wallet_ref()) == typeid(Wallet&));
    CHECK(typeid(c.bank_wallet_ref()) == typeid(Wallet&));

    CHECK(c.cash_wallet_ref().balance() == 0);
    CHECK(c.bank_wallet_ref().balance() == 0);

    c.cash_wallet_ref().add_funds(20);
    c.bank_wallet_ref().add_funds(20);

    CHECK(c.cash_wallet_ref().balance() == 20);
    CHECK(c.bank_wallet_ref().balance() == 20);
}

TEST_CASE("Character: health can be set only between 0 and 100", "[Character]") {
    Character c{{"test_rank", 200000, 50000}};

    CHECK_NOTHROW(c.set_health(0));
    CHECK(c.health() == 0);
    CHECK_NOTHROW(c.set_health(15));
    CHECK(c.health() == 15);
    CHECK_NOTHROW(c.set_health(100));
    CHECK(c.health() == 100);

    CHECK_THROWS_AS(c.set_health(-1), std::out_of_range);
    CHECK_THROWS_AS(c.set_health(101), std::out_of_range);
}

TEST_CASE("Character: Inventory ItemCollections can be accessed", "[Character]") {
    Character c{{"test_rank", 200000, 50000}};

    auto drug_collection{c.inv_collection_ptr<ItemType::Drug>().lock()};
    CHECK(drug_collection != nullptr);

    auto armour_collection{c.inv_collection_ptr<ItemType::Armour>().lock()};
    CHECK(armour_collection != nullptr);
}


TEST_CASE("Character: CharacterRank can be accessed and set", "[Character]") {
    const CharacterRank cr{"test_rank", 200000, 50000};
    Character c{cr};

    CHECK(c.rank().title() == cr.title());

    const CharacterRank cr_new{"test_rank_new", 20, 50};
    c.set_rank(cr_new);
    CHECK(c.rank().title() == cr_new.title());
}

TEST_CASE("Character: resultant armour value from items can be calculated", "[Character]") {
    const std::unordered_map<std::string, int> armour_values{{"heavy leather coat", 5}, {"bullet proof vest", 10}};

    Character c{{"test_rank", 200000, 50000}};
    CHECK(c.calculate_armour(armour_values) == 0);

    auto armour_collection{c.inv_collection_ptr<ItemType::Armour>().lock()};
    armour_collection->add_item({"heavy leather coat"});
    CHECK(c.calculate_armour(armour_values) == 5);

    armour_collection->add_item({"bullet proof vest"});
    CHECK(c.calculate_armour(armour_values) == 10);

    armour_collection->add_item({"heavy leather coat"});
    CHECK(c.calculate_armour(armour_values) == 10);
}

TEST_CASE("Character: cannot calculate armour when holding invalid armour", "[Character]") {
    Character c{{"test_rank", 200000, 50000}};
    c.inv_collection_ptr<ItemType::Armour>().lock()->add_item({"unknown armour"});
    CHECK_THROWS_AS(c.calculate_armour({{"known armour", 10}}), std::logic_error);
}
