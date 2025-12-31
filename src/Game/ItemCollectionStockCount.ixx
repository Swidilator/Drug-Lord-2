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
    ItemCollectionStockCount(std::unordered_map<std::string, std::size_t>&& item_stock)
        : item_type_{T}, item_stock_{std::move(item_stock)} {
    }

    auto operator[](const std::string& item_name) const -> std::size_t {
        if (!item_stock_.contains(item_name)) {
            return 0;
        }
        return item_stock_.at(item_name);
    }

    [[nodiscard]] auto size() const -> std::size_t {
        auto non_zero_view = item_stock_
                             | std::views::transform([](const auto& e) { return e.second; })
                             | std::views::filter([](const auto& e) {
                                 return e != 0;
                             });

        return std::ranges::fold_left(non_zero_view, 0, std::plus{});
    }

    [[nodiscard]] auto empty() const -> bool {
        return size() == 0;
    }

    [[nodiscard]] auto get_item_type() const -> ItemType {
        return item_type_;
    }
};
