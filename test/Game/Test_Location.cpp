//
// Created by kyle on 2026/01/10.
//

#include "test_setup.hpp"

import Game.Item;
import Game.Location;


TEST_CASE("Location: name can be accessed", "[Location]") {
    Location l{"test_name"};
    CHECK(l.name() == "test_name");
}

TEST_CASE("Location: market is accessible and mutable", "[Location]") {
    Location l{"test_name"};
}

TEST_CASE("Location: drug vault reference can only be accessed when unlocked", "[Location]") {
    Location l{"test_name"};

    // vault should be unlocked initially
    CHECK(l.vault_locked() == false);
    CHECK_NOTHROW(l.vault(false));

    // vault should still be unlocked
    CHECK(l.vault_locked() == false);
    CHECK_NOTHROW(l.vault(true));

    // vault should be locked after above
    CHECK(l.vault_locked() == true);
    CHECK_THROWS_AS(l.vault(false), std::logic_error);

    // manually unlock vault
    l.set_vault_locked(false);
    CHECK(l.vault_locked() == false);
    CHECK_NOTHROW(l.vault(false));

    // manually relock vault
    l.set_vault_locked(true);
    CHECK(l.vault_locked() == true);
    CHECK_THROWS_AS(l.vault(true), std::logic_error);
}

TEST_CASE("Location: drug market member mutable reference can be accessed", "[Location]") {
    Location l{"test_name"};
    l.drug_market().set_prices({{"test_item", 4}});
    l.drug_market().add_item({"test_item"});
    CHECK(l.drug_market().stock_count()["test_item"] == 1);
}
