//
// Created by MaGaroo on 1/31/18.
//

#include "game.h"

int main() {
    Map map;
    Game game;
    Pacman pacman;
    Ghost ghosts[MAX_GHOST_COUNT];
    initiateGame("res/map.txt", &map, &game, &pacman, ghosts);


    return 0;
}