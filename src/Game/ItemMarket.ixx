//
// Created by kyle on 2025/12/01.
//
module;
#include <algorithm>
#include <format>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

export module Game.ItemMarket;
import Game.Item;
import Game.ItemCollection;
import Game.Wallet;

export using ItemPrices = std::unordered_map<std::string, int>;


export template <ItemType T>
class ItemMarket {
    ItemCollection<T> item_collection_{};
    ItemPrices item_prices_{};

public:
    ItemMarket(ItemCollection<T>&& item_collection, ItemPrices item_prices)
        : item_collection_{std::move(item_collection)}, item_prices_{std::move(item_prices)} {}

    auto item_collection() const -> const ItemCollection<T>& {
        return item_collection_;
    }

    auto prices() const -> const ItemPrices& {
        return item_prices_;
    }

    auto set_prices(const ItemPrices& item_prices) -> void {
        item_prices_ = item_prices;
    }

    [[nodiscard]]
    auto stock_count() const -> ItemStockCount {
        auto out = item_collection_.stock_count();

        for (const auto& item_name : item_prices_ | std::views::keys) {
            if (!out.contains(item_name)) {
                out[item_name] = 0;
            }
        }
        auto ks{std::views::keys(out)};

        for (const std::vector<std::string> keys{ks.begin(), ks.end()}; const auto& item_name : keys) {
            if (!item_prices_.contains(item_name)) {
                out.erase(item_name);
            }
        }

        return out;
    }

    [[nodiscard]]
    auto total_items() const -> std::size_t {
        return std::ranges::fold_left(stock_count() | std::views::values, 0, std::plus{});
    }

    auto add_item(Item<T>&& item) {
        item_collection_.add_item(std::move(item));
    }

    auto retrieve_item(const std::string& item_name) -> Item<T> {
        return item_collection_.retrieve_item(item_name);
    }

    auto sell_items(ItemCollection<T>&& offered_items, Wallet& wallet) -> void {
        long long int total_price{};
        for (const auto& [item_name, num_items] : offered_items.stock_count()) {
            if (!item_prices_.contains(item_name)) {
                throw std::domain_error("Item being sold has no price.");
            }

            total_price += num_items * item_prices_[item_name];
        }

        wallet.add_funds(total_price);

        for (const auto& [item_name, num_items] : offered_items.stock_count()) {
            for (int i{}; i < num_items; i++) {
                item_collection_.add_item(offered_items.retrieve_item(item_name));
            }
        }
    }

    [[nodiscard]]
    auto buy_items(const std::unordered_map<std::string, int>& requested_items,
                   Wallet& wallet) -> ItemCollection<T> {
        long long int total_price{};
        auto current_stock = item_collection_.stock_count();

        for (const auto& [item_name, num_items] : requested_items) {
            if (!item_prices_.contains(item_name)) {
                throw std::domain_error("Item being bought has no price.");
            }

            if (current_stock.at(item_name) < num_items) {
                throw std::range_error("Not enough items in market.");
            }

            total_price += num_items * item_prices_[item_name];
        }

        if (total_price > wallet.balance()) {
            throw std::range_error("Not enough money in wallet.");
        }

        ItemCollection<T> output_ic{};

        wallet.remove_funds(total_price);

        for (const auto& item_name : requested_items | std::views::keys) {
            output_ic.add_item(item_collection_.retrieve_item(item_name));
        }

        return output_ic;
    }
};
