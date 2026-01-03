//
// Created by kyle on 2025/12/16.
//
module;
#include <memory>

export module Game.Inventory;

import Game.Item;
import Game.ItemCollection;


export class Inventory {
    std::shared_ptr<ItemCollection<ItemType::Drug> > drugs_ptr_;
    std::shared_ptr<ItemCollection<ItemType::Armour> > armours_ptr_;
    std::shared_ptr<ItemCollection<ItemType::Weapon> > weapons_ptr_;
    std::shared_ptr<ItemCollection<ItemType::Ammo> > ammo_ptr_;

public:
    Inventory() : drugs_ptr_{std::make_shared<ItemCollection<ItemType::Drug> >()},
                  armours_ptr_{std::make_shared<ItemCollection<ItemType::Armour> >()},
                  weapons_ptr_{std::make_shared<ItemCollection<ItemType::Weapon> >()},
                  ammo_ptr_{std::make_shared<ItemCollection<ItemType::Ammo> >()} {
    }

    auto get_drugs_ptr() -> std::weak_ptr<ItemCollection<ItemType::Drug> > {
        return drugs_ptr_;
    }

    auto get_armours_ptr() -> std::weak_ptr<ItemCollection<ItemType::Armour> > {
        return armours_ptr_;
    }

    auto get_weapons_ptr() -> std::weak_ptr<ItemCollection<ItemType::Weapon> > {
        return weapons_ptr_;
    }

    auto get_ammo_ptr() -> std::weak_ptr<ItemCollection<ItemType::Ammo> > {
        return ammo_ptr_;
    }
};
