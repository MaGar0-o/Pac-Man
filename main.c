//
// Created by magaroo on 11/25/17.
//

#include <stdio.h>
#include <stdlib.h>

struct Movement {
    int current_x, current_y;
    int initial_x, initial_y;
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
    struct Movement *movement = (struct Movement *) malloc(sizeof(struct Movement));
    movement->current_x = movement->y = movement->initial_x = movement->initial_y = movement->direction = 0;
    return movement;
}

Man *init_man(void) {
    struct Man *man = (struct Man *) malloc(sizeof(struct Man));
    man->movement = init_movement();
    man->health = 0;
    return man;
}

Ghost *init_ghost(void) {
    struct Ghost *ghost = (struct Ghost *) malloc(sizeof(struct Ghost));
    ghost->movement = init_movement();
    ghost->defense_mode = 0;
    ghost->remaining_defense_time = 0;
    return ghost;
}

Map *init_map(void) {
    struct Map *map = (struct Map *) malloc(sizeof(struct Map));
    return map;
}

Time *init_time(void) {
    struct Time *time = (struct Time *) malloc(sizeof(struct Time));
    time->minutes = time->seconds = 0;
    return time;
}

Game *init_game(void) {
    struct Game *game = (struct Game *) malloc(sizeof(struct Game));
    game->score = 0;
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
