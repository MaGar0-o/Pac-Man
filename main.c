//
// Created by magaroo on 11/25/17.
//

#include <stdio.h>

struct Movement {
    int x, y;
    int direction;
};

struct Man {
    Movement *movement;
    int health;
};

struct Ghost {
    Movement *movement;
    int defense_mode;
    int remaining_defense_time;
};

struct Map {
    int n, m;
    char **cells;
};

struct Time {
    int minutes, seconds;
};

struct Game {
    int score;
    Man *pacman;
    Ghost *blinky;
    Ghost *pinky;
    Ghost *clyde;
    Ghost *inky;
    Map *map;
};

int main() {

    return 0;
}
