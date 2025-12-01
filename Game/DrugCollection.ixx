//
// Created by kyle on 2025/12/01.
//
module;
#include <format>
#include <memory>
#include <stack>
#include <unordered_map>

export module Game.DrugCollection;
import Game.Drug;

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

    [[nodiscard]] auto check_stock(const std::string& drug_name) const -> std::size_t {
        if (!drugs_.contains(drug_name)) {
            return 0;
        }

        return drugs_.at(drug_name).size();
    }
};
