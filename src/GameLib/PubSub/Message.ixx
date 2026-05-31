//
// Created by kyle on 22/03/2026.
//
module;
#include <string>
#include <unordered_map>

export module Game.PubSub:Message;

export struct Message {
    std::string topic{};
    std::unordered_map<std::string, std::string> data{};
};