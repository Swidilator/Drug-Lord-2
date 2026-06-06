//
// Created by kyle on 22/03/2026.
//
module;
#include <functional>
#include <memory>
#include <print>
#include <queue>
#include <string>

export module Game.PubSub:Publisher;
import :Message;
import :Subscriber;


export enum class PublisherProcessResult {
    Success,
    InvalidSubscriber,
    Quit,
    Empty,
};


static auto to_string(const PublisherProcessResult e) -> const char* {
    switch (e) {
    case PublisherProcessResult::Success: return "Success";
    case PublisherProcessResult::InvalidSubscriber: return "InvalidSubscriber";
    case PublisherProcessResult::Quit: return "Quit";
    case PublisherProcessResult::Empty: return "Empty";
    default: return "Unknown";
    }
}


export class Publisher {
    inline static std::unordered_map<std::string, std::vector<std::weak_ptr<Subscriber>>> subscribers_;

    inline static std::queue<Message> queue_{};

    Publisher() = default;

public:
    static auto reset() -> void {
        subscribers_.clear();
        for (std::size_t i = 0; i < queue_.size(); ++i) {
            queue_.pop();
        }
    }

    static auto list_subscribers() -> const auto& {
        return subscribers_;
    }

    static auto unsubscribe(const std::string& topic, std::weak_ptr<Subscriber> subscriber) -> bool {
        if (subscriber.expired()) {
            throw std::logic_error(std::format("Provided Subscriber for topic {} already expired", topic));
        }

        auto sub_ptr = subscriber.lock();

        for (std::size_t i = 0; i < subscribers_.at(topic).size(); ++i) {
            auto& comp_ptr{subscribers_.at(topic)[i]};
            if (!comp_ptr.expired() && comp_ptr.lock() == sub_ptr) {
                subscribers_.at(topic).erase(subscribers_.at(topic).begin() + i);
                return true;
            }
        }

        return false;
    }

    static auto subscribe(const std::string& topic, std::weak_ptr<Subscriber> subscriber) -> void {
        if (subscriber.expired()) {
            throw std::logic_error(std::format("New Subscriber for topic {} already expired", topic));
        }

        if (subscribers_.contains(topic)) {
            for (const auto& wp : subscribers_.at(topic)) {
                if (wp.lock() == subscriber.lock()) {
                    throw std::logic_error(std::format("New Subscriber is already subscribed to topic {}", topic));
                }
            }
        }
        subscribers_[topic].emplace_back(subscriber);
    }

    static auto publish(const Message& message) -> void {
        queue_.push(message);
    }

    static auto action_next() -> bool {
        if (queue_.empty()) {
            return true;
        }

        const auto next_message{queue_.front()};
        queue_.pop();

        if (next_message.topic == "quit") {
            return false;
        }

        if (!subscribers_.contains(next_message.topic)) {
            return true;
        }

        for (auto& ptr : subscribers_.at(next_message.topic)) {
            if (const auto p = ptr.lock()) {
                p->handle_event(next_message);
            }

            else {
                std::println("Invalid Subscriber on topic {}", next_message.topic);
            }
        }

        return true;
    }

    static auto action_all() -> bool {
        bool final_status{};
        while (!queue_.empty()) {
            final_status = action_next();
        }
        return final_status;
    }
};
