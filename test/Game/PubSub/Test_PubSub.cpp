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
    Publisher::publish("test_topic");
    Publisher::reset();
    CHECK(Publisher::list_subscribers().size() == 0);
    CHECK(Publisher::process_next()[0] == PublisherProcessResult::Empty);
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
    Publisher::publish("test_topic");
    Publisher::process_all();
    CHECK(test_subscriber->event_messages.size() == 1);
}

TEST_CASE("Subscriber: handles published events it subscribes to", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    Publisher::publish("test_topic");
    Publisher::publish("other_topic");
    Publisher::process_all();
    REQUIRE(test_subscriber->event_messages.size() == 1);
    CHECK(test_subscriber->event_messages[0].topic == "test_topic");
}

TEST_CASE("Subscriber: is only registered once for a particular topic", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic", test_subscriber);
    CHECK_THROWS_AS(Publisher::subscribe("test_topic", test_subscriber), std::logic_error);
}

TEST_CASE("Publisher: returns false only when encountering quit topic", "[Publisher]") {
    Publisher::reset();
    Publisher::publish("not_quit");
    Publisher::publish("quit");
    CHECK(Publisher::process_next()[0] != PublisherProcessResult::Quit);
    CHECK(Publisher::process_next()[0] == PublisherProcessResult::Quit);
}

TEST_CASE("Publisher: returns PublisherProcessResult::InvalidSubscriber when weak_ptr is invalid on action", "[Publisher]") {
    Publisher::reset();
    // Subscribe temporary object that will be invalid after this line
    Publisher::subscribe("test_topic", std::make_shared<TestSubscriber>());
    Publisher::publish("test_topic");
    CHECK(Publisher::process_next()[0] == PublisherProcessResult::InvalidSubscriber);
}

TEST_CASE("Subscriber: can be unsubscribed from selected topic", "[Subscriber]") {
    Publisher::reset();
    auto test_subscriber = std::make_shared<TestSubscriber>();
    Publisher::subscribe("test_topic",test_subscriber);
    Publisher::subscribe("test_topic_2",test_subscriber);
    REQUIRE(Publisher::unsubscribe("test_topic", test_subscriber) == true);
    Publisher::publish("test_topic");
    Publisher::publish("test_topic_2");
    Publisher::process_all();
    REQUIRE(test_subscriber->event_messages.size() == 1);
    CHECK(test_subscriber->event_messages[0].topic == "test_topic_2");

    CHECK(Publisher::unsubscribe("test_topic", test_subscriber) == false);
}