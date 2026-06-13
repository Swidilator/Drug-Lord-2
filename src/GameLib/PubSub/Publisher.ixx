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
        // I am unsure about this method; perhaps it should not return subscribers_ as is...
        return subscribers_;
    }

    static auto unsubscribe(const std::string& topic, std::weak_ptr<Subscriber> subscriber) -> bool {
        if (subscriber.expired()) {
            throw std::logic_error(std::format("unsubscribe: Provided Subscriber for topic {} already expired", topic));
        }

        if (!subscribers_.contains(topic)) {
            return false;
        }

        auto sub_ptr = subscriber.lock();

        for (std::size_t i = 0; i < subscribers_.at(topic).size(); ++i) {
            auto& comp_ptr{subscribers_.at(topic)[i]};
            if (!comp_ptr.expired() && comp_ptr.lock() == sub_ptr) {
                subscribers_.at(topic).erase(subscribers_.at(topic).begin() + i);
                if (subscribers_.at(topic).size() == 0) {
                    subscribers_.erase(topic);
                }
                return true;
            }
        }

        return false;
    }

    static auto unsubscribe_all(std::weak_ptr<Subscriber> subscriber) {
        // Get key_list before for loop due to unsubscribe potentially removing keys and breaking the view

        for (const auto key_list{subscribers_ | std::views::keys | std::ranges::to<std::vector>()}; const auto& k :
             key_list) {
            unsubscribe(k, subscriber);
        }
    }

    static auto subscribe(const std::string& topic, std::weak_ptr<Subscriber> subscriber) -> void {
        if (subscriber.expired()) {
            throw std::logic_error(std::format("subscribe: Subscriber for topic {} already expired", topic));
        }

        if (subscribers_.contains(topic)) {
            for (const auto& wp : subscribers_.at(topic)) {
                if (wp.lock() == subscriber.lock()) {
                    return;
                }
            }
        }
        subscribers_[topic].emplace_back(subscriber);
    }

    static auto publish(const Message& message) -> void {
        queue_.push(message);
    }

    static auto process_next() -> std::vector<PublisherProcessResult> {
        if (queue_.empty()) {
            return {PublisherProcessResult::Empty};
        }

        const auto next_message{queue_.front()};
        queue_.pop();

        if (next_message.topic == "quit") {
            return {PublisherProcessResult::Quit};
        }

        if (!subscribers_.contains(next_message.topic)) {
            return {PublisherProcessResult::Empty};
        }

        std::vector<PublisherProcessResult> results{};

        for (auto& ptr : subscribers_.at(next_message.topic)) {
            if (const auto p = ptr.lock()) {
                p->handle_event(next_message);
                results.push_back(PublisherProcessResult::Success);
            }

            else {
                std::println("Invalid Subscriber on topic {}", next_message.topic);
                results.push_back(PublisherProcessResult::InvalidSubscriber);
            }
        }

        return results;
    }

    static auto process_all() -> std::vector<PublisherProcessResult> {
        std::vector<PublisherProcessResult> final_status{};
        while (!queue_.empty()) {
            final_status = process_next();
        }
        return final_status;
    }
};
