//
// Created by kyle on 2025/12/16.
//
module;
#include <memory>
#include <unordered_map>

export module Game.Inventory;

import Game.Item;
import Game.ItemCollection;


export class Inventory {
    std::unordered_map<ItemType, std::shared_ptr<ItemCollectionBase> > collection_map_{};

public:
    template<ItemType T>
    [[nodiscard]]
    auto collection_ptr() -> std::weak_ptr<ItemCollection<T> > {
        if (!collection_map_.contains(T)) {
            collection_map_[T] = std::make_shared<ItemCollection<T> >();
        }
        return std::dynamic_pointer_cast<ItemCollection<T> >(collection_map_[T]);
    }
};