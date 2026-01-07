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

    [[nodiscard]] const std::string& title() const {
        return title_;
    }

    [[nodiscard]] long long int money_threshold() const {
        return money_threshold_;
    }

    [[nodiscard]] std::size_t inventory_size() const {
        return drug_inventory_size_;
    }
};
