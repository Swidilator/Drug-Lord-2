//
// Created by kyle on 13/06/2026.
//
#include <print>

#include "test_setup.hpp"
#include "test_statecharts_setup.hpp"

import Game.PubSub;
import Game.Statecharts;

namespace {
    TEST_CASE("ActionList: can register an action", "[ActionList]") {
        Publisher::reset();
        ActionList::reset();
        CHECK_NOTHROW(ActionList::register_action(std::make_shared<TestStatecharts::OnAction>("OnAction")));
        CHECK(ActionList::list().size() == 1);
        CHECK(ActionList::list()[0] == "OnAction");
    }

    TEST_CASE("ActionList: cannot register action with no name", "[ActionList]") {
        Publisher::reset();
        ActionList::reset();
        CHECK_THROWS(ActionList::register_action(std::make_shared<TestStatecharts::OnAction>()));
    }

    TEST_CASE("ActionList: can subscribe and unsubscribe a registered action to a topic", "[ActionList]") {
        Publisher::reset();
        ActionList::reset();
        ActionList::register_action(std::make_shared<TestStatecharts::OnAction>("OnAction"));
        CHECK(Publisher::list_subscribers().size() == 0);
        ActionList::subscribe("test_topic", "OnAction");
        CHECK(Publisher::list_subscribers().size() == 1);
        ActionList::unsubscribe("test_topic", "OnAction");
        CHECK(Publisher::list_subscribers().size() == 0);
    }

    TEST_CASE("ActionList: reset() member unsubscribes all held actions and clears list", "[ActionList]") {
        Publisher::reset();
        ActionList::reset();
        // Separate subscriber that should not be reset on ActionList::reset()
        auto separate_subscriber{std::make_shared<TestStatecharts::OffAction>()};
        Publisher::subscribe("separate_topic", separate_subscriber);

        CHECK(ActionList::list().size() == 0);

        ActionList::register_action(std::make_shared<TestStatecharts::OnAction>("OnAction"));
        ActionList::subscribe("test_topic", "OnAction");

        ActionList::reset();
        CHECK(Publisher::list_subscribers().size() == 1);
        CHECK(ActionList::list().size() == 0);
    }
}
