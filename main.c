//
// Created by magaroo on 11/25/17.
//

#include <stdio.h>

struct Man {
    int x, y;
    int direction;
    int health;
};

struct Ghost {
    int x, y;
    int defense_mode;
    int remaining_defense_time;
    int direction;
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
