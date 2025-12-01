//
// Created by kyle on 2025/11/30.
//
module;
#include <format>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

export module Game.Drug;

export class Drug {
    std::string name_{};
    int price_last_bought_at_{0};
    int price_last_sold_at_{0};

public:
    explicit Drug(std::string name)
        : name_{std::move(name)} {
    }

    [[nodiscard]] auto get_name() const -> const std::string& {
        return name_;
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

export class DrugCollection {
    std::unordered_map<std::string, std::stack<std::unique_ptr<Drug> > > drugs_{};

public:
    auto add_drug(std::unique_ptr<Drug>&& drug) -> void {
        drugs_[drug->get_name()].push(std::move(drug));
    }

    auto remove_drug(const std::string& drug_name) -> std::unique_ptr<Drug> {
        if (drugs_[drug_name].empty()) {
            throw std::runtime_error(std::format("No more {} in DrugCollection", drug_name));
        }
        auto return_drug = std::move(drugs_[drug_name].top());
        drugs_[drug_name].pop();
        return return_drug;
    }
};
