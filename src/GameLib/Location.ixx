//
// Created by kyle on 2025/11/30.
//
module;
#include <memory>

export module Game.Location;
import Game.Item;
import Game.ItemMarket;
import Game.ItemCollection;

export class Location {
    std::string name_{};
    bool vault_locked_{false};

    ItemMarket<ItemType::Drug> drug_market_{{}, {}};
    ItemCollection<ItemType::Drug> vault_{};

public:
    Location(const std::string& name)
        : name_{name} {}


    [[nodiscard]]
    auto name() const -> const std::string& {
        return name_;
    }

    [[nodiscard]]
    auto drug_market() -> ItemMarket<ItemType::Drug>& {
        return drug_market_;
    }

    [[nodiscard]]
    auto vault_locked() const -> bool {
        return vault_locked_;
    }

    auto set_vault_locked(const bool vault_locked) -> void {
        vault_locked_ = vault_locked;
    }

    [[nodiscard]]
    auto vault(const bool lock_vault) -> ItemCollection<ItemType::Drug>& {
        if (vault_locked_) {
            throw std::logic_error("Vault is locked.");
        }

        if (lock_vault) {
            vault_locked_ = lock_vault;
        }

        return vault_;
    }
};
