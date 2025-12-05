//
// Created by kyle on 2025/12/01.
//
module;
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

export module Game.DrugMarket;
import Game.Drug;
import Game.DrugCollection;
import Game.Wallet;

export class DrugTransaction {
    bool is_success_{false};
    bool is_extracted_{false};
    DrugCollection drug_collection_{};

public:
    DrugTransaction() = default;

    DrugTransaction(const bool& is_success, DrugCollection&& dc)
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

    [[nodiscard]] auto extract_drug_collection() -> DrugCollection&& {
        is_extracted_ = true;
        return std::move(drug_collection_);
    }
};


export class DrugMarket {
    DrugCollection drug_collection_{};

    std::unordered_map<std::string, int> drug_prices_{};

public:
    // test
    DrugMarket() {
        drug_prices_["hash"] = 10;
    }

    auto get_collection() const -> const DrugCollection& {
        return drug_collection_;
    }

    auto get_prices() const -> const std::unordered_map<std::string, int>& {
        return drug_prices_;
    }

    [[nodiscard]] auto buy_drug(const std::string& drug_name, const int num,
                                const std::weak_ptr<Wallet>& buyer_wallet) -> DrugCollection {
        DrugCollection transfer_dc{};

        auto wallet = buyer_wallet.lock();
        if (!wallet) {
            throw std::runtime_error("buyer_wallet weak_ptr invalid.");
        }

        if (num * drug_prices_[drug_name] > wallet->get_balance()) {
            throw std::runtime_error("buyer_wallet has insufficient funds.");
        }


        if (drug_collection_.check_stock(drug_name) < num) {
            throw std::runtime_error(std::format("Too few {} in DrugCollection", drug_name));
        }

        for (int i{0}; i < num; i++) {
            transfer_dc.add_drug(drug_collection_.remove_drug(drug_name));
            wallet->remove_funds(drug_prices_[drug_name]);
        }

        return transfer_dc;
    }
};
