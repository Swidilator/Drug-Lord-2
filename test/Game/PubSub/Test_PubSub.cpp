//
// Created by kyle on 22/03/2026.
//

#include <iostream>
#include <memory>

#include "test_setup.hpp"

import Game.PubSub;

namespace {
    class TestSubscriber : public Subscriber {
    public:
        std::vector<Message> event_messages{};

        auto handle_event(const Message& message) -> void override {
            event_messages.emplace_back(message);
        }
    };
}

TEST_CASE("PubSub: Need more tests", "[Placeholder]") {
    Publisher::reset();
    CHECK(false);
}

TEST_CASE("Publisher: can be reset, clearing its subscribers and queue", "[Publisher]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    Publisher::reset();
    CHECK(Publisher::list_subscribers().size() == 0);
}

TEST_CASE("Subscriber: can be subscribed to a particular topic", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    CHECK(Publisher::list_subscribers().size() == 1);
}

TEST_CASE("Publisher: is static and does not need to be instantiated", "[Publisher]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    Publisher::publish({"test_topic", {}});
    Publisher::action_all();
    CHECK(test_subscriber->event_messages.size() == 1);
}

TEST_CASE("Subscriber: handles published events it subscribes to", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    Publisher::publish({"test_topic", {}});
    Publisher::publish({"other_topic", {}});
    Publisher::action_all();
    REQUIRE(test_subscriber->event_messages.size() == 1);
    CHECK(test_subscriber->event_messages[0].topic == "test_topic");
}

TEST_CASE("Subscriber: is only registered once for a particular topic", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    CHECK_THROWS_AS(Publisher::subscribe("test_topic", test_subscriber), std::logic_error);
}