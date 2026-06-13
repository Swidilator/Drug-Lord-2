//
// Created by kyle on 29/05/2026.
//
module;
#include <unordered_map>
#include <string>
#include <memory>
#include <format>
#include <ranges>
#include <vector>
#include <type_traits>

export module Game.Statecharts:ActionList;
import :Action;

export template <typename DerivedAction>
concept ActionDerivitive = std::derived_from<DerivedAction, Action>;


export class ActionList {
    inline static std::unordered_map<std::string, std::shared_ptr<Action>> action_list_{};

public:
    static auto reset() -> void {
        for (auto& act : action_list_ | std::views::values) {
            Publisher::unsubscribe_all(act);
        }
        action_list_.clear();
    }

    /**
    * Register an ActionDerivative shared_ptr. The ActionList is intended to own that object.
    * @tparam T ActionDerivative, usually inferred.
    * @param action ActionDerivative shared_prt to be registered.
    */
    template <ActionDerivitive T>
    static auto register_action(std::shared_ptr<T> action) -> void {
        if (action->name().empty()) {
            throw std::logic_error("register_action: Cannot register Action with no name.");
        }
        if (action_list_.contains(action->name())) {
            throw std::logic_error(std::format("register_action: Action {} already exists.", action->name()));
        }
        action_list_[action->name()] = action;
    }

    static auto list() -> std::vector<std::string> {
        return action_list_ | std::views::keys | std::ranges::to<std::vector>();
    }

    static auto subscribe(const std::string& topic, const std::string& action_name) -> void {
        if (!action_list_.contains(action_name)) {
            throw std::logic_error(std::format("activate_subscription: Action {} does not exist.", action_name));
        }

        Publisher::subscribe(topic, action_list_.at(action_name));
    }

    static auto unsubscribe(const std::string& topic, const std::string& action_name) -> void {
        if (!action_list_.contains(action_name)) {
            throw std::logic_error(std::format("deactivate_subscription: Action {} does not exist.", action_name));
        }

        Publisher::unsubscribe(topic, action_list_.at(action_name));
    }
};
