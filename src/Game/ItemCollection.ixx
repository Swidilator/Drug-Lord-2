//
// Created by kyle on 2025/12/01.
//
module;
#include <algorithm>
#include <format>
#include <queue>
#include <unordered_map>
#include <utility>

export module Game.ItemCollection;
import Game.Item;

export template<ItemType T>
class ItemCollection {
    std::unordered_map<std::string, std::queue<Item<T> > > item_map_{};

public:
    ItemCollection() = default;

    // Delete copy operations
    ItemCollection(const ItemCollection& other) = delete;

    auto operator=(const ItemCollection& other) -> ItemCollection& = delete;

    // Move operations
    ItemCollection(ItemCollection&& other) = default;

    auto operator=(ItemCollection&& other) -> ItemCollection& = default;

    // Destructor
    ~ItemCollection() = default;


    // Other operations
    auto add_item(Item<T>&& item) -> void {
        if (!item_map_.contains(item.get_name())) {
            item_map_[item.get_name()] = {};
        }

        item_map_[item.get_name()].push(std::move(item));
    }

    auto retrieve_item(const std::string& item_name) -> Item<T> {
        if (!item_map_.contains(item_name) or item_map_[item_name].empty()) {
            throw std::out_of_range(std::format("No more {} in ItemCollection", item_name));
        }
        auto return_item = std::move(item_map_[item_name].front());
        item_map_[item_name].pop();

        if (item_map_[item_name].empty()) {
            item_map_.erase(item_name);
        }

        return return_item;
    }

    [[nodiscard]] auto get_stock_count() const -> std::unordered_map<std::string, std::size_t> {
        std::unordered_map<std::string, std::size_t> out;

        for (const auto& [key, value]: item_map_) {
            out[key] = value.size();
        }

        return out;
    }

    [[nodiscard]] auto total_items() const -> std::size_t {
        auto non_zero_view = get_stock_count()
                             | std::views::transform([](const auto& e) { return e.second; })
                             | std::views::filter([](const auto& e) {
                                 return e != 0;
                             });

        return std::ranges::fold_left(non_zero_view, 0, std::plus{});
    }
};
