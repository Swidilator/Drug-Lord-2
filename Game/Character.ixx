//
// Created by kyle on 2025/11/30.
//
module;
#include <string>

export module Game.Character;


export class Character {

public:
    long long cash{0};
    long long bank{0};
    long long debt{0};

    int health{100};

    int day{1};

    std::string location{"Austin, USA"};


    Character() {

    }
};
