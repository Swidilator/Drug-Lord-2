//
// Created by kyle on 2025/11/30.
//
module;
#include <string>
#include <utility>

export module Game.Item;

export enum class ItemType {
    Drug,
    Weapon,
    Ammo,
    Armour
};

export class Item {
    std::string name_{};
    ItemType item_type_;
    int price_last_bought_at_{0};
    int price_last_sold_at_{0};

public:
    explicit Item(std::string  name, const ItemType item_type)
        : name_{std::move(name)}, item_type_{item_type} {
    }

    [[nodiscard]] virtual auto get_name() const -> const std::string& {
        return name_;
    }

    [[nodiscard]] virtual auto get_price_last_bought_at() const -> int {
        return price_last_bought_at_;
    }

    [[nodiscard]] virtual auto get_price_last_sold_at() const -> int {
        return price_last_sold_at_;
    }

    virtual auto set_price_last_bought_at(const int price) -> void {
        price_last_bought_at_ = price;
    }

    virtual auto set_price_last_sold_at(const int price) -> void {
        price_last_sold_at_ = price;
    }
};
