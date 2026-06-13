//
// Created by kyle on 13/06/2026.
//

#pragma once

#include <memory>
#include <print>

import Game.PubSub;
import Game.Statecharts;

namespace TestStatecharts {
    class OnAction : public Action {
    public:
        OnAction() = default;

        OnAction(std::string action_name)
            : Action(std::move(action_name)) {}

        auto handle_event(const Message& message) -> void override {
            std::println("ActionOn: Event handled");
        }

        ~OnAction() override = default;
    };


    class OffAction : public Action {
    public:
        OffAction() = default;

        OffAction(std::string action_name)
            : Action(std::move(action_name)) {}

        auto handle_event(const Message& message) -> void override {
            std::println("ActionOff: Event handled");
        }

        ~OffAction() override = default;
    };


    auto setup_actions() -> void {
        //ActionList::register_action()
    }
}
