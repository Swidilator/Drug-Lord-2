//
// Created by kyle on 22/03/2026.
//
module;

export module Game.PubSub:Subscriber;
import :Message;

export class Subscriber {
public:
    virtual auto handle_event(const Message& message) -> void = 0;

    virtual ~Subscriber() = default;
};