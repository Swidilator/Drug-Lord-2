//
// Created by kyle on 2025/12/01.
//
module;
#include <format>
#include <queue>
#include <unordered_map>
#include <utility>

export module Game.ItemCollection;
import Game.Item;

export class ItemCollection {
    std::unordered_map<std::string, std::pair<ItemType, std::queue<Item> > > item_map_{};

public:
    ItemCollection() = default;

    // Delete copy operations
    ItemCollection(const ItemCollection& other) = delete;

    auto operator=(const ItemCollection& other) -> ItemCollection& = delete;

    // Move operations
    ItemCollection(ItemCollection&& other) noexcept
        : item_map_(std::move(other.item_map_)) {
    }

    auto operator=(ItemCollection&& other) noexcept -> ItemCollection& {
        if (this == &other)
            return *this;
        item_map_ = std::move(other.item_map_);
        return *this;
    }

    // Destructor
    ~ItemCollection() = default;


    // Other operations
    auto add_item(Item&& item) -> void {
        if (!item_map_.contains(item.get_name())) {
            item_map_[item.get_name()] = {item.get_item_type(), {}};
        }

        if (item_map_[item.get_name()].first != item.get_item_type()) {
            throw std::runtime_error("Type mismatch between existing container type and new item type.");
        }

        item_map_[item.get_name()].second.push(std::move(item));
    }

    auto remove_item(const std::string& item_name) -> Item {
        if (!item_map_.contains(item_name) or item_map_[item_name].second.empty()) {
            throw std::runtime_error(std::format("No more {} in DrugCollection", item_name));
        }
        auto return_drug = std::move(item_map_[item_name].second.front());
        item_map_[item_name].second.pop();
        return std::move(return_drug);
    }

    [[nodiscard]] auto check_stock(const std::string& item_name) const -> std::size_t {
        if (!item_map_.contains(item_name)) {
            return 0;
        }

        return item_map_.at(item_name).second.size();
    }
};
