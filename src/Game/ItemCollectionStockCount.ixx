//
// Created by kyle on 2025/12/30.
//
module;
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <string>
export module Game.ItemCollectionStockCount;
import Game.Item;


export template<ItemType T>
class ItemCollectionStockCount {
    ItemType item_type_{};
    std::unordered_map<std::string, std::size_t> item_stock_{};
public:


    ItemCollectionStockCount(const std::unordered_map<std::string, std::size_t>& item_stock)
        : item_type_{T}, item_stock_{item_stock} {
    }

    [[nodiscard]] auto get_item_type() const -> ItemType {
        return item_type_;
    }

    [[nodiscard]] auto get_item_stock() const -> const std::unordered_map<std::string, std::size_t>& {
        return item_stock_;
    }

    [[nodiscard]] auto total_items() const -> std::size_t {
        auto non_zero_view = item_stock_
                             | std::views::transform([](const auto& e) { return e.second; })
                             | std::views::filter([](const auto& e) {
                                 return e != 0;
                             });

        return std::ranges::fold_left(non_zero_view, 0, std::plus{});
    }
};
