//
// Created by kyle on 2026/01/06.
//
module;
#include <stdexcept>
#include <string>
#include <utility>

export module Game.CharacterRank;

export class CharacterRank {
    std::string title_{};
    long long int money_threshold_{};
    std::size_t drug_inventory_size_{};

public:
    CharacterRank(
        std::string title,
        const long long int money_threshold,
        const std::size_t drug_inventory_size
    )
        : title_{std::move(title)},
          money_threshold_{money_threshold},
          drug_inventory_size_{drug_inventory_size} {
        if (money_threshold < 0) {
            throw std::out_of_range("Money threshold should be positive");
        }
    }

    [[nodiscard]]
    auto title() const -> const std::string& {
        return title_;
    }

    [[nodiscard]]
    auto money_threshold() const -> long long int {
        return money_threshold_;
    }

    [[nodiscard]]
    auto drug_inventory_size() const -> std::size_t {
        return drug_inventory_size_;
    }
};