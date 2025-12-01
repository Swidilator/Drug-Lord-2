//
// Created by kyle on 2025/11/30.
//
module;
#include <string>

export module Game.Drug;

export class Drug {
    std::string name_{};
    int price_last_bought_at_{0};
    int price_last_sold_at_{0};

public:
    explicit Drug(std::string name)
        : name_{std::move(name)} {
    }

    [[nodiscard]] auto get_name() const -> const std::string& {
        return name_;
    }

    [[nodiscard]] auto get_price_last_bought_at() const -> int {
        return price_last_bought_at_;
    }

    [[nodiscard]] auto get_price_last_sold_at() const -> int {
        return price_last_sold_at_;
    }

    auto set_price_last_bought_at(const int price) -> void {
        price_last_bought_at_ = price;
    }

    auto set_price_last_sold_at(const int price) -> void {
        price_last_sold_at_ = price;
    }
};
