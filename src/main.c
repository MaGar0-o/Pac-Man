//
// Created by MaGaroo on 1/31/18.
//

#include <SDL_events.h>
#include "game.h"
#include "physics.h"

#define EPS 1e-2
#define CELL_SIZE 50
#define PACMAN_RADIUS 24
#define PACMAN_COLOR 0xFF42F4F1
#define CHEESE_RADIUS 3
#define CHEESE_COLOR 0xFFAAAAFF
#define PINEAPPLE_RADIUS 8
#define PINEAPPLE_COLOR 0xFFAAAAFF
#define CHERRY_RADIUS 8
#define CHERRY_DIST 5
#define CHERRY_COLOR 0xFF0000FF

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

int get_corner(double pos) {
    return (int) (pos * CELL_SIZE);
}

int get_center(double pos) {
    return get_corner(pos) + CELL_SIZE / 2;
}

int get_left_pacman_angle(Direction dir, int cycle) {
    cycle %= 90;
    static Direction previous_dir = DIR_LEFT;
    if (dir == DIR_NONE)
        dir = previous_dir;
    previous_dir = dir;
    switch (dir) {
        case DIR_RIGHT:
            if (cycle < 45)
                return cycle;
            return 90 - cycle;
        case DIR_LEFT:
            return cycle < 45 ? 180 + cycle : 270 - cycle;
        case DIR_DOWN:
            return cycle < 45 ? 90 + cycle : 180 - cycle;
        case DIR_UP:
            return cycle < 45 ? 270 + cycle : -cycle;
        default:
            if (cycle < 45)
                return cycle;
            return 90 - cycle;
    }
}

int get_right_pacman_angle(Direction dir, int cycle) {
    cycle %= 90;
    static Direction previous_dir = DIR_LEFT;
    if (dir == DIR_NONE)
        dir = previous_dir;
    previous_dir = dir;
    switch (dir) {
        case DIR_RIGHT:
            if (cycle < 45)
                return -cycle;
            return cycle - 90;
        case DIR_LEFT:
            return cycle < 45 ? 180 - cycle : 90 + cycle;
        case DIR_DOWN:
            return cycle < 45 ? 90 - cycle : cycle;
        case DIR_UP:
            return cycle < 45 ? 270 - cycle : 180 + cycle;
        default:
            if (cycle < 45)
                return -cycle;
            return cycle - 90;
    }
}

unsigned int get_ghost_color(GhostType t, bool isBlue) {
    if (isBlue)
        return 0xFFFF0000;
    switch (t) {
        case BLINKY:
            return 0xFF5555FF;
        case PINKY:
            return 0xFFFFBBFF;
        case INKY:
            return 0xFFFFFF77;
        case CLYDE:
            return 0xFF66AAFF;
        default:
            return 0xFFFFFFFF;
    }
}

void draw_ghost(SDL_Renderer *renderer, double x, double y, unsigned int color) {
    filledCircleColor(renderer,
                      get_center(x),
                      get_center(y),
                      PACMAN_RADIUS,
                      color);
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
    int pause = 0;
    for (int cycle = 0; !quit; cycle++) {

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
                continue;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        quit = 1;
                        break;
                    case SDLK_p:
                        pause = !pause;
                        break;
                    default:
                        last_key = e.key.keysym.sym;
                }
            }
        }

        if (pause) {
            SDL_Delay(1000 / CYCLES_PER_SEC);
            cycle--;
            continue;
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
                switch (map.cells[i][j]) {
                    case CELL_BLOCK:
                        rectangleColor(renderer, get_corner(i), get_corner(j), get_corner(i + 1), get_corner(j + 1),
                                       0xFFFF0000);
                        break;
                    case CELL_CHEESE:
                        filledCircleColor(renderer, get_center(i), get_center(j), CHEESE_RADIUS, CHEESE_COLOR);
                        break;
                    case CELL_PINEAPPLE:
                        filledCircleColor(renderer, get_center(i), get_center(j), PINEAPPLE_RADIUS, PINEAPPLE_COLOR);
                        break;
                    case CELL_CHERRY:
                        filledCircleColor(renderer, get_center(i) - CHERRY_DIST, get_center(j), CHERRY_RADIUS,
                                          CHERRY_COLOR);
                        filledCircleColor(renderer, get_center(i) + CHERRY_DIST, get_center(j), CHERRY_RADIUS,
                                          CHERRY_COLOR);
                        break;
                }
        filledPieColor(renderer,
                       get_center(pacman.x),
                       get_center(pacman.y),
                       PACMAN_RADIUS,
                       get_left_pacman_angle(pacman.dir, cycle),
                       get_right_pacman_angle(pacman.dir, cycle),
                       PACMAN_COLOR);

        for (int i = 0; i < 4; i++)
            draw_ghost(renderer, ghosts[i].x, ghosts[i].y, get_ghost_color(ghosts[i].type, ghosts[i].blue));

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / CYCLES_PER_SEC);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}