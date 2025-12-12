//
// Created by kyle on 2025/11/30.
//
module;
#include <string>

export module Game.Character;


export class Character {
    long long cash_{0};
    long long bank_{0};
    long long debt_{0};

    int health_{100};

    int day_{1};

    std::string location_{"Austin, USA"};

public:
    Character() {
    }
};
