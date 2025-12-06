//
// Created by kyle on 2025/12/01.
//
module;
#include <format>
#include <memory>
#include <stack>
#include <unordered_map>

export module Game.ItemCollection;
import Game.Item;

export class ItemCollection {
    std::unordered_map<std::string, std::stack<std::unique_ptr<Item> > > drugs_{};

public:
    ItemCollection() = default;

    // Delete copy operations
    ItemCollection(const ItemCollection& other) = delete;

    auto operator=(const ItemCollection& other) -> ItemCollection& = delete;

    // Move operations
    ItemCollection(ItemCollection&& other) noexcept
        : drugs_(std::move(other.drugs_)) {
    }

    auto operator=(ItemCollection&& other) noexcept -> ItemCollection& {
        if (this == &other)
            return *this;
        drugs_ = std::move(other.drugs_);
        return *this;
    }

    // Destructor
    ~ItemCollection() = default;


    // Other operations
    auto add_drug(std::unique_ptr<Item>&& drug) -> void {
        drugs_[drug->get_name()].push(std::move(drug));
    }

    auto remove_drug(const std::string& drug_name) -> std::unique_ptr<Item> {
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
