//
// Created by kyle on 2025/11/30.
//
module;
#include <string>
#include <utility>

export module Game.Item;

export enum class ItemType {
    None,
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
    Item(std::string name, const ItemType item_type)
        : name_{std::move(name)}, item_type_{item_type} {
    }

    Item(const Item&) = delete;

    auto operator=(const Item&) -> Item& = delete;

    Item(Item&&) = default;

    auto operator=(Item&&) -> Item& = default;

    ~Item() = default;

    [[nodiscard]] auto get_name() const -> const std::string& {
        return name_;
    }

    [[nodiscard]] auto get_item_type() const -> const ItemType& {
        return item_type_;
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
