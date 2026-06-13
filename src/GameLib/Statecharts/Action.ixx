//
// Created by kyle on 27/05/2026.
//
module;
#include <memory>
#include <utility>


export module Game.Statecharts:Action;
import Game.PubSub;


export class Action : public Subscriber {
    std::string name_;

public:
    Action() = default;

    Action(std::string action_name)
        : name_{std::move(action_name)} {}

    [[nodiscard]] virtual auto name() const -> const std::string& {
        return name_;
    };
};
