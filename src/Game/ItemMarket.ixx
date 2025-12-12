//
// Created by kyle on 2025/12/01.
//
module;
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

export module Game.ItemMarket;
import Game.Item;
import Game.ItemCollection;
import Game.Wallet;

export class ItemTransaction {
    bool is_success_{false};
    bool is_extracted_{false};
    ItemCollection item_collection_{};

public:
    ItemTransaction() = default;

    ItemTransaction(const bool& is_success, ItemCollection&& dc)
        : is_success_{is_success}, item_collection_{std::move(dc)} {
    }


    ItemTransaction(const ItemTransaction& other) = delete;

    auto operator=(const ItemTransaction& other) -> ItemTransaction& = delete;


    ItemTransaction(ItemTransaction&& other) = default;

    auto operator=(ItemTransaction&& other) noexcept -> ItemTransaction& = default;

    ~ItemTransaction() = default;

    [[nodiscard]] auto check_success() const -> const bool& {
        return is_success_;
    }

    [[nodiscard]] auto check_extracted() const -> const bool& {
        return is_extracted_;
    }

    [[nodiscard]] auto extract_item_collection() -> ItemCollection&& {
        is_extracted_ = true;
        return std::move(item_collection_);
    }
};


export class ItemMarket {
    ItemCollection item_collection_{};

    std::unordered_map<std::string, int> item_prices_{};

public:
    ItemMarket() = default;

    explicit ItemMarket(ItemCollection&& item_collection)
        : item_collection_{std::move(item_collection)} {
    }

    auto get_collection() const -> const ItemCollection& {
        return item_collection_;
    }

    auto get_prices() const -> const std::unordered_map<std::string, int>& {
        return item_prices_;
    }

    [[nodiscard]] auto buy_drug(const std::string& item_name, const int num,
                                const std::weak_ptr<Wallet>& buyer_wallet) -> ItemTransaction {
        const auto wallet = buyer_wallet.lock();
        if (!wallet) {
            throw std::runtime_error("buyer_wallet weak_ptr invalid.");
        }

        if (num * item_prices_[item_name] > wallet->get_balance()) {
            return std::move(ItemTransaction{});
        }


        if (item_collection_.check_stock(item_name) < num) {
            return std::move(ItemTransaction{});
        }

        bool transaction_success{true};
        ItemCollection transaction_ic{};


        for (int i{0}; i < num; i++) {
            transaction_ic.add_item(item_collection_.remove_item(item_name));
            transaction_success = wallet->remove_funds(item_prices_[item_name]);
        }

        return std::move(ItemTransaction{transaction_success, std::move(transaction_ic)});
    }
};
