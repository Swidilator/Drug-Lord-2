//
// Created by kyle on 2025/12/01.
//
module;
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

export template<ItemType T>
class ItemMarket {
    ItemCollection<T> item_collection_{};
    std::unordered_map<std::string, int> item_prices_{};

public:
    ItemMarket(ItemCollection<T>&& item_collection, std::unordered_map<std::string, int> item_prices)
        : item_collection_{std::move(item_collection)}, item_prices_{std::move(item_prices)} {
        for (const auto& [item_name, num_item]: item_collection_.get_stock_count()) {
            if (!item_prices_.contains(item_name)) {
                throw std::domain_error("item_prices does not contain item all item_names");
            }
        }
    }

    auto get_collection() const -> const ItemCollection<T>& {
        return item_collection_;
    }

    auto get_prices() const -> const std::unordered_map<std::string, int>& {
        return item_prices_;
    }

    [[nodiscard]] auto get_stock_count() const -> std::unordered_map<std::string, std::size_t> {
        auto out = item_collection_.get_stock_count();

        for (const auto& item_name: item_prices_ | std::views::keys) {
            if (!out.contains(item_name)) {
                out[item_name] = 0;
            }
        }

        return out;
    }

    [[nodiscard]] auto total_items() const -> std::size_t {
        return item_collection_.total_items();
    }

    auto sell_items(ItemCollection<T>&& offered_items, const std::weak_ptr<Wallet>& wallet) -> void {
        const auto wallet_p = wallet.lock();
        if (!wallet_p) {
            throw std::logic_error("Wallet weak_ptr invalid.");
        }

        long long int total_price{};
        for (const auto& [item_name, num_items]: offered_items.get_stock_count()) {
            if (!item_prices_.contains(item_name)) {
                throw std::domain_error("Item being sold has no price.");
            }

            total_price += num_items * item_prices_[item_name];
        }

        wallet_p->add_funds(total_price);

        for (const auto& [item_name, num_items]: offered_items.get_stock_count()) {
            for (int i{}; i < num_items; i++) {
                item_collection_.add_item(offered_items.retrieve_item(item_name));
            }
        }
    }

    [[nodiscard]] auto buy_items(const std::unordered_map<std::string, int>& requested_items,
                                 const std::weak_ptr<Wallet>& wallet) -> ItemCollection<T> {
        const auto wallet_p = wallet.lock();
        if (!wallet_p) {
            throw std::logic_error("Wallet weak_ptr invalid.");
        }

        long long int total_price{};
        auto current_stock = item_collection_.get_stock_count();

        for (const auto& [item_name, num_items]: requested_items) {
            if (!item_prices_.contains(item_name)) {
                throw std::domain_error("Item being bought has no price.");
            }

            if (current_stock.at(item_name) < num_items) {
                throw std::range_error("Not enough items in market.");
            }

            total_price += num_items * item_prices_[item_name];
        }

        if (total_price > wallet_p->get_balance()) {
            throw std::range_error("Not enough money in wallet.");
        }

        ItemCollection<T> output_ic{};

        wallet_p->remove_funds(total_price);

        for (const auto& item_name: requested_items | std::views::keys) {
            output_ic.add_item(item_collection_.retrieve_item(item_name));
        }

        return output_ic;
    }
};
