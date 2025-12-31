//
// Created by kyle on 2025/12/13.
//
module;
#include <memory>

export module Game.ItemTransaction;

import Game.Item;
import Game.ItemCollection;


export template<ItemType T>
class ItemTransaction {
    bool is_success_{false};
    bool is_extracted_{false};
    ItemCollection<T> item_collection_{};

public:
    ItemTransaction() = default;

    ItemTransaction(const bool& is_success, ItemCollection<T>&& dc)
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

    [[nodiscard]] auto extract_item_collection() -> ItemCollection<T>&& {
        if (is_extracted_) {
            throw std::logic_error("ItemCollections cannot be re-extracted.");
        }

        is_extracted_ = true;
        return std::move(item_collection_);
    }
};

