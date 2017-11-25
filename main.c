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
    struct Movement *movement;
    int health;
};

struct Ghost {
    struct Movement *movement;
    int defense_mode;
    int remaining_defense_time;
};

struct Map {
    int n, m;
    char cells[100][100];
};

struct Time {
    int minutes, seconds;
};

struct Game {
    int score;
    struct Man *pacman;
    struct Ghost *blinky;
    struct Ghost *pinky;
    struct Ghost *clyde;
    struct Ghost *inky;
    struct Map *map;
    struct Time *game_time;
};

struct Movement *init_movement(void) {
    struct Movement *movement = (struct Movement *) malloc(sizeof(struct Movement));
    movement->current_x = movement->current_y = movement->initial_x = movement->initial_y = movement->direction = 0;
    return movement;
}

struct Man *init_man(void) {
    struct Man *man = (struct Man *) malloc(sizeof(struct Man));
    man->movement = init_movement();
    man->health = 0;
    return man;
}

struct Ghost *init_ghost(void) {
    struct Ghost *ghost = (struct Ghost *) malloc(sizeof(struct Ghost));
    ghost->movement = init_movement();
    ghost->defense_mode = 0;
    ghost->remaining_defense_time = 0;
    return ghost;
}

struct Map *init_map(void) {
    struct Map *map = (struct Map *) malloc(sizeof(struct Map));
    return map;
}

struct Time *init_time(void) {
    struct Time *time = (struct Time *) malloc(sizeof(struct Time));
    time->minutes = time->seconds = 0;
    return time;
}

struct Game *init_game(void) {
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

void get_ghost(struct Ghost *g) {
    return;
}

int main() {
    struct Game *game = init_game();

    struct Map *map = game->map;
    scanf("%d %d\n", &map->n, &map->m);
    for (int i = 0; i < map->n; i++)
        scanf("%s", map->cells[i]);

    struct Time *game_time = game->game_time;
    scanf("%d:%d %d\n", &game_time->minutes, &game_time->seconds, &game->score);

    struct Man *pacman = game->pacman;
    scanf("pacman: %d %d (%d,%d) (%d,%d)\n",
          &pacman->movement->direction,
          &pacman->health,
          &pacman->movement->initial_x,
          &pacman->movement->initial_y,
          &pacman->movement->current_x,
          &pacman->movement->current_y);

    struct Ghost *blinky = game->blinky;
    get_ghost(blinky);

    struct Ghost *pinky = game->pinky;
    get_ghost(pinky);

    struct Ghost *clyde = game->clyde;
    get_ghost(clyde);

    struct Ghost *inky = game->inky;
    get_ghost(inky);



    return 0;
}
