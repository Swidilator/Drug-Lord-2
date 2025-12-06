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

export class DrugTransaction {
    bool is_success_{false};
    bool is_extracted_{false};
    ItemCollection drug_collection_{};

public:
    DrugTransaction() = default;

    DrugTransaction(const bool& is_success, ItemCollection&& dc)
        : is_success_{is_success}, drug_collection_{std::move(dc)} {
    }


    DrugTransaction(const DrugTransaction& other) = delete;

    auto operator=(const DrugTransaction& other) -> DrugTransaction& = delete;


    DrugTransaction(DrugTransaction&& other) = default;

    auto operator=(DrugTransaction&& other) noexcept -> DrugTransaction& = default;

    ~DrugTransaction() = default;

    [[nodiscard]] auto check_success() const -> const bool& {
        return is_success_;
    }

    [[nodiscard]] auto check_extracted() const -> const bool& {
        return is_extracted_;
    }

    [[nodiscard]] auto extract_drug_collection() -> ItemCollection&& {
        is_extracted_ = true;
        return std::move(drug_collection_);
    }
};


export class ItemMarket {
    ItemCollection drug_collection_{};

    std::unordered_map<std::string, int> drug_prices_{};

public:
    // test
    ItemMarket() {
        drug_prices_["hash"] = 10;
    }

    auto get_collection() const -> const ItemCollection& {
        return drug_collection_;
    }

    auto get_prices() const -> const std::unordered_map<std::string, int>& {
        return drug_prices_;
    }

    [[nodiscard]] auto buy_drug(const std::string& drug_name, const int num,
                                const std::weak_ptr<Wallet>& buyer_wallet) -> DrugTransaction {
        const auto wallet = buyer_wallet.lock();
        if (!wallet) {
            throw std::runtime_error("buyer_wallet weak_ptr invalid.");
        }

        if (num * drug_prices_[drug_name] > wallet->get_balance()) {
            return std::move(DrugTransaction{});
        }


        if (drug_collection_.check_stock(drug_name) < num) {
            return std::move(DrugTransaction{});
        }

        bool transaction_success{true};
        ItemCollection transaction_dc{};


        for (int i{0}; i < num; i++) {
            transaction_dc.add_drug(drug_collection_.remove_drug(drug_name));
            transaction_success = wallet->remove_funds(drug_prices_[drug_name]);
        }

        return std::move(DrugTransaction{transaction_success, std::move(transaction_dc)});
    }
};
