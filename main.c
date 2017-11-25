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
    Time *game_time;
};

Movement *init_movement(void) {

}

Man *init_man(void) {

}

Ghost *init_ghost(void) {

}

Time *init_time(void) {

}

Map *init_map(void) {

}

Game *init_game(void) {
    Game *game = (Game *) malloc(sizeof(Game));
    game->pacman = init_man();
    game->blinky = init_ghost();
    game->pinky = init_ghost();
    game->clyde = init_ghost();
    game->inky = init_ghost();
    game->map = init_map();
    game->game_time = init_time();
    return game;
}

int main() {
    Game *game = init_game();
    return 0;
}
