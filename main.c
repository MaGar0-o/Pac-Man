//
// Created by magaroo on 11/25/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int dx[5] = {0, -1, 0, +1, 0};
int dy[5] = {0, 0, +1, 0, -1};

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
    scanf("%*s %d %d ",
          &g->movement->direction,
          &g->defense_mode
    );

    if (g->defense_mode == 0)
        scanf("%d ", &g->remaining_defense_time);

    scanf("(%d,%d) (%d,%d)",
          &g->movement->initial_x,
          &g->movement->initial_y,
          &g->movement->current_x,
          &g->movement->current_y
    );
    return;
}

bool same_place(struct Movement *m1, struct Movement *m2) {
    return m1->current_x == m2->current_x &&
           m1->current_y == m2->current_y;
}

bool man_can_eat(struct Man *pacman, struct Ghost *ghost) {
    return ghost->defense_mode == 0 &&
           same_place(pacman->movement, ghost->movement);
}

bool ghost_can_eat(struct Ghost *ghost, struct Man *pacman) {
    return ghost->defense_mode == 1 &&
           same_place(pacman->movement, ghost->movement);
}

void make_ghosts_defensive(struct Game *game) {
    game->blinky->defense_mode = game->clyde->defense_mode = game->pinky->defense_mode = game->inky->defense_mode = 0;
    return;
}

void move_forward(struct Game *game) {
    struct Man *pacman = game->pacman;
    struct Movement *mvmnt = pacman->movement;
    struct Map *map = game->map;
    int n = map->n;
    int m = map->m;

    int new_x = mvmnt->current_x + dx[mvmnt->direction];
    new_x = (new_x % n + n) % n;
    int new_y = mvmnt->current_y + dy[mvmnt->direction];
    new_y = (new_y % m + m) % m;

    if (map->cells[new_x][new_y] == '#')
        return;

    mvmnt->current_x = new_x;
    mvmnt->current_y = new_y;

    switch (map->cells[new_x][new_y]) {
        case '*':
            game->score++;
            break;
        case '^':
            game->score += 20;
            break;
        case 'O':
            make_ghosts_defensive(game);
            break;
    }
    map->cells[new_x][new_y] = '_';

    //can pacman eat ghosts?
    if (man_can_eat(pacman, game->blinky))
        game->score += 50;
    if (man_can_eat(pacman, game->pinky))
        game->score += 50;
    if (man_can_eat(pacman, game->clyde))
        game->score += 50;
    if (man_can_eat(pacman, game->inky))
        game->score += 50;

    //can ghosts eat pacman?
    bool pacman_died = false;
    if (ghost_can_eat(game->blinky, pacman))
        pacman_died = true;
    if (ghost_can_eat(game->pinky, pacman))
        pacman_died = true;
    if (ghost_can_eat(game->clyde, pacman))
        pacman_died = true;
    if (ghost_can_eat(game->inky, pacman))
        pacman_died = true;
    if (pacman_died) {
        pacman->health--;
        mvmnt->current_x = mvmnt->initial_x;
        mvmnt->current_y = mvmnt->initial_y;
    }
    return;
}

void print_current_location(struct Game *g) {
    printf("(%d,%d)\n",
           g->pacman->movement->current_x,
           g->pacman->movement->current_y
    );
    return;
}

void print_current_score(struct Game *g) {
    printf("%d\n", g->score);
    return;
}

void print_winning_state(struct Game *g) {
    bool state = true;
    struct Map *map = g->map;
    for (int i = 0; i < map->n; i++)
        for (int j = 0; j < map->m; j++)
            if (map->cells[i][j] == '*' || map->cells[i][j] == 'O')
                state = false;
    printf(state ? "Yes\n" : "No\n");
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
          &pacman->movement->current_y
    );

    struct Ghost *blinky = game->blinky;
    get_ghost(blinky);

    struct Ghost *pinky = game->pinky;
    get_ghost(pinky);

    struct Ghost *clyde = game->clyde;
    get_ghost(clyde);

    struct Ghost *inky = game->inky;
    get_ghost(inky);

    move_forward(game);

    print_current_location(game);
    print_current_score(game);
    print_winning_state(game);

    return 0;
}
