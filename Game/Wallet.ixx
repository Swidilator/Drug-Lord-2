//
// Created by kyle on 2025/12/01.
//
module;

export module Game.Wallet;

export class Wallet {
    long long int balance_{};

public:
    [[nodiscard]] auto get_balance() const -> long long int {
        return balance_;
    }

    auto add_funds(const long long int value) -> void {
        balance_ += value;
    }

    auto remove_funds(const long long int value) -> bool {
        if (value > balance_) {
            return false;
        }

        balance_ -= value;
        return true;
    }
};
