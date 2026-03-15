//
// Created by kyle on 13/03/2026.
//

#include <QApplication>

#include "MainWindow.h"

import Game.Item;
import Game.ItemCollection;
import Game.Character;
import Game.CharacterRank;
import Game.Location;
import Game.ItemMarket;

import Game.Game;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow main_window;
    main_window.show();
    return a.exec();
}