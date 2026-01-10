//
// Created by kyle on 2025/11/30.
//
module;
#include <algorithm>
#include <format>
#include <memory>
#include <unordered_map>
#include <utility>

export module Game.Character;
import Game.Item;
import Game.Wallet;
import Game.ItemCollection;
import Game.Inventory;
import Game.CharacterRank;

export class Character {
    std::shared_ptr<Wallet> cash_{};
    std::shared_ptr<Wallet> bank_{};

    // TODO: Debt will be implemented later

    Inventory inventory_{};

    int health_{100};

    CharacterRank rank_;

public:
    explicit Character(CharacterRank rank)
        : cash_{std::make_shared<Wallet>()},
          bank_{std::make_shared<Wallet>()},
          rank_{std::move(rank)} {
    }

    [[nodiscard]]
    auto health() const -> int {
        return health_;
    }

    auto set_health(const int health) -> void {
        if (health < 0 || health > 100) {
            throw std::out_of_range("Health can only be set to between 0 and 100");
        }
        health_ = health;
    }

    [[nodiscard]]
    auto calculate_armour(const std::unordered_map<std::string, int>& armour_values) -> int {
        int max_armour_value{};

        for (
            const auto armour_collection{inventory_.collection_ptr<ItemType::Armour>().lock()}
            ; const auto& [armour_name, armour_value]
            : armour_collection->stock_count()) {
            if (armour_values.contains(armour_name)) {
                max_armour_value = std::max(max_armour_value, armour_values.at(armour_name));
            } else {
                throw std::logic_error(std::format("{} not valid armour name", armour_name));
            }
        }

        return max_armour_value;
    }

    [[nodiscard]]
    auto cash_wallet_ptr() -> std::weak_ptr<Wallet> {
        return cash_;
    }

    [[nodiscard]]
    auto bank_wallet_ptr() -> std::weak_ptr<Wallet> {
        return bank_;
    }

    [[nodiscard]]
    auto rank() const -> const CharacterRank& {
        return rank_;
    }

    auto set_rank(const CharacterRank& rank) -> void {
        rank_ = rank;
    }

    template<ItemType T>
    [[nodiscard]]
    auto inv_collection_ptr() -> std::weak_ptr<ItemCollection<T> > {
        return inventory_.collection_ptr<T>();
    }
};
