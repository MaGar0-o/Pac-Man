//
// Created by MaGaroo on 1/31/18.
//

#include <SDL_events.h>
#include "game.h"
#include "physics.h"

#define EPS 1e-2
#define CELL_SIZE 50
#define PACMAN_RADIUS 24

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
        return pos + len <= w - 1 ? pos + len : pos + len - w;
    if (dir == DIR_LEFT)
        return pos - len >= 0 ? pos - len : pos - len + w;
}

double moveY(const Map *map, double pos, Direction dir, double len) {
    int h = map->height;
    if (dir == DIR_NONE || dir == DIR_LEFT || dir == DIR_RIGHT)
        return pos;
    if (dir == DIR_DOWN)
        return pos + len <= h - 1 ? pos + len : pos + len - h;
    if (dir == DIR_UP)
        return pos - len >= 0 ? pos - len : pos - len + h;
}


int main() {
    Map map;
    Game game;
    Pacman pacman;
    Ghost ghosts[MAX_GHOST_COUNT];
    initiateGame("res/map.txt", &map, &game, &pacman, ghosts);
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2_gfx test", 100, 100, map.width * CELL_SIZE, map.height * CELL_SIZE,
                                          SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 3;
    }

    SDL_Event e;
    SDL_Keycode last_key = SDLK_n;

    int quit = 0;
    for (int cycle = 0; !quit; cycle++) {

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
                quit = 1;
                continue;
            } else if (e.type == SDL_KEYDOWN)
                last_key = e.key.keysym.sym;
        }

        checkEatables(&map, &game, &pacman, ghosts);
        for (int i = 0; i < 4; i++)
            checkGhostState(ghosts + i);
        for (int i = 0; i < 4; i++)
            checkGhostCollision(&pacman, ghosts + i);
        if (isGameFinished(&game, &pacman)) {
            quit = 1;
            continue;
        }


        if (isInt(&pacman.x) && isInt(&pacman.y)) {
            if (last_key != SDLK_n) {
                pacman.dir = decidePacman(&map, &pacman, convert_sdl_to_action(last_key));
                last_key = SDLK_n;
            }
            pacman.dir = decidePacman(&map, &pacman, ACTION_NONE);
        }

        pacman.x = moveX(&map, pacman.x, pacman.dir, pacman.speed / CYCLES_PER_SEC);
        pacman.y = moveY(&map, pacman.y, pacman.dir, pacman.speed / CYCLES_PER_SEC);


        for (int i = 0; i < 4; i++) {
            if (isInt(&ghosts[i].x) && isInt(&ghosts[i].y))
                ghosts[i].dir = decideGhost(&map, ghosts + i, &pacman, ghosts);
            ghosts[i].x = moveX(&map, ghosts[i].x, ghosts[i].dir, ghosts[i].speed / CYCLES_PER_SEC);
            ghosts[i].y = moveY(&map, ghosts[i].y, ghosts[i].dir, ghosts[i].speed / CYCLES_PER_SEC);
        }

        //now we should draw ^__^
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        for (int i = 0; i < map.width; i++)
            for (int j = 0; j < map.height; j++)
                if (map.cells[i][j] == CELL_BLOCK)
                    rectangleColor(renderer, i * CELL_SIZE, j * CELL_SIZE, (i + 1) * CELL_SIZE, (j + 1) * CELL_SIZE,
                                   0xFFFF0000);
        filledPieColor(renderer, (int) ((pacman.x * 2 + 1) * CELL_SIZE / 2), (int) ((pacman.y * 2 + 1) * CELL_SIZE / 2),
                       PACMAN_RADIUS, 45, -45, 0xFF00FF00);
        for (int i = 0; i < 4; i++)
            filledCircleColor(renderer, (int) ((ghosts[i].x * 2 + 1) * CELL_SIZE / 2),
                              (int) ((ghosts[i].y * 2 + 1) * CELL_SIZE / 2),
                              PACMAN_RADIUS, 0xFF0000FF);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / CYCLES_PER_SEC);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}