//
// Created by kyle on 2025/12/01.
//
module;
#include <stdexcept>
export module Game.Wallet;


export class Wallet {
    long long int balance_{};

public:
    Wallet() = default;

    Wallet(const long long int balance) : balance_{balance} {
        if (balance < 0) {
            throw std::runtime_error("Wallet balance is initialised as negative");
        }
    }

    [[nodiscard]] auto get_balance() const -> long long int {
        return balance_;
    }

    auto add_funds(const long long int value) -> void {
        balance_ += value;
    }

    [[nodiscard]] auto remove_funds(const long long int value) -> bool {
        if (value > balance_) {
            return false;
        }

        balance_ -= value;
        return true;
    }
};
