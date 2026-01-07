//
// Created by kyle on 2026/01/06.
//
module;
#include <string>
#include <utility>

export module Game.CharacterRank;

export class CharacterRank {
    std::string title_{};
    long long int money_threshold_{};
    std::size_t inventory_size_{};

public:
    CharacterRank(
        std::string title,
        const long long int money_threshold,
        const std::size_t inventory_size
    )
        : title_{std::move(title)},
          money_threshold_{money_threshold},
          inventory_size_{inventory_size} {
    }

    [[nodiscard]] const std::string& title() const {
        return title_;
    }

    [[nodiscard]] long long int money_threshold() const {
        return money_threshold_;
    }

    [[nodiscard]] std::size_t inventory_size() const {
        return inventory_size_;
    }
};
