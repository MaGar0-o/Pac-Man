//
// Created by MaGaroo on 1/31/18.
//

#include <SDL_events.h>
#include "game.h"
#include "physics.h"

#define EPS 1e-3

bool isInt(double *d) {
    if (*d - floor(*d) <= EPS) {
        *d = floor(*d);
        return true;
    }
    if (ceil(*d) - *d <= EPS) {
        *d = ceil(*d);
        return true;
    }
    return false;
}

double moveX(const Map *map, double pos, Direction dir, double len) {
    int w = map->width;
    if (dir == DIR_NONE || dir == DIR_UP || dir == DIR_DOWN)
        return pos;
    if (dir == DIR_RIGHT)
        return pos + len < w ? pos + len : pos + len - w;
    if (dir == DIR_LEFT)
        return pos - len >= 0 ? pos - len : pos - len + w;
}

double moveY(const Map *map, double pos, Direction dir, double len) {
    int h = map->height;
    if (dir == DIR_NONE || dir == DIR_LEFT || dir == DIR_RIGHT)
        return pos;
    if (dir == DIR_DOWN)
        return pos + len < h ? pos + len : pos + len - h;
    if (dir == DIR_UP)
        return pos - len >= 0 ? pos - len : pos - len + h;
}


int main() {
    Map map;
    Game game;
    Pacman pacman;
    Ghost ghosts[MAX_GHOST_COUNT];
    initiateGame("res/map.txt", &map, &game, &pacman, ghosts);

    SDL_Event e;

    int quit = 0;
    for (int cycle = 0; !quit; cycle++) {

        checkEatables(&map, &game, &pacman, ghosts);
        for (int i = 0; i < 4; i++)
            checkGhostState(ghosts + i);
        for (int i = 0; i < 4; i++)
            checkGhostCollision(&pacman, ghosts + i);
        if (isGameFinished(&game, &pacman)) {
            quit = 1;
            continue;
        }


        if (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) {
                quit = 1;
                continue;
            } else if (e.type == SDL_KEYDOWN && isInt(&pacman.x) && isInt(&pacman.y))
                pacman.dir = decidePacman(&map, &pacman, convert_sdl_to_action(e));
        pacman.x = moveX(&map, pacman.x, pacman.dir, pacman.speed / CYCLES_PER_SEC);
        pacman.y = moveY(&map, pacman.y, pacman.dir, pacman.speed / CYCLES_PER_SEC);


        for (int i = 0; i < 4; i++) {
            if (isInt(&ghosts[i].x) && isInt(&ghosts[i].y))
                ghosts[i].dir = decideGhost(&map, ghosts + i, &pacman, ghosts);
            ghosts[i].x = moveX(&map, ghosts[i].x, ghosts[i].dir, ghosts[i].speed / CYCLES_PER_SEC);
            ghosts[i].y = moveY(&map, ghosts[i].y, ghosts[i].dir, ghosts[i].speed / CYCLES_PER_SEC);
        }
    }

    return 0;
}