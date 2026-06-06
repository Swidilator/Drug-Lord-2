//
// Created by kyle on 22/03/2026.
//
module;
#include <string>
#include <unordered_map>
#include <utility>

export module Game.PubSub:Message;

export struct Message {
    using MessageData = std::unordered_map<std::string, std::string>;

    std::string topic;
    MessageData data;

    Message() = default;

    explicit Message(std::string topic)
        : topic{std::move(topic)} {}

    Message(const char* topic)
        : topic{topic} {}

    Message(std::string topic, MessageData message_data)
        : topic{std::move(topic)}, data{std::move(message_data)} {}
};
