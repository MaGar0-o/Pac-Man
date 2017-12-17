#include "game.h"
#include "map.h"
#include <stdio.h>

void get_ghost(FILE *input, Ghost *ghost) {
    int tmp1, tmp2;
    fscanf(input, "%*s %d %d ", &tmp1, &tmp2);
    ghost->dir = (Direction) tmp1;
    //printf("%d %d\n", tmp1, tmp2);
    //fscanf(input, "%d", tmp1);
    ghost->blue = (bool) tmp2;
    if (!tmp2)
        fscanf(input, "%d ", &tmp1);
    ghost->blueCounterDown = (unsigned long long int) tmp1;
    fscanf(input, "(%d,%d) (%lf,%lf)", &ghost->startY, &ghost->startX, &ghost->y, &ghost->x);
    //printf("%d %d %d %d", ghost->startX, ghost->startY, (int)ghost->x, (int)ghost->y);
}

void initiateGame(char *filename, Map *outMap, Game *outGame, Pacman *outPacman, Ghost *outGhosts) {
    FILE *input = fopen(filename, "r");
    printf("%s\n", filename);
    fscanf(input, "%d %d\n", &outMap->height, &outMap->width);
    outGame->cheeses = outGame->cherries = outGame->pineapples = 0;
    outGame->ghosts = 4;
    printf("%d %d\n", outMap->height, outMap->width);

    for (int i = 0; i < outMap->height; i++)
        for (int j = 0; j < outMap->width; j++) {
            char c;
            fscanf(input, "%c", &c);
            outMap->cells[j][i] = (Cell) c;
            switch (c) {
                case CELL_CHEESE:
                    outGame->cheeses++;
                    break;
                case CELL_CHERRY:
                    outGame->cherries++;
                    break;
                case CELL_PINEAPPLE:
                    outGame->pineapples++;
                    break;
                default:
                    break;
            }
            fscanf(input, "\n");
        }
    fscanf(input, "%d\n", &outGame->score);
    int tmp;
    fscanf(input, "pacman: %lf %d (%d,%d) (%lf,%lf)\n", &outPacman->speed, &tmp, &outPacman->startX, &outPacman->startY,
           &outPacman->x, &outPacman->y);
    printf("%d %d %d %d\n", outPacman->startX, outPacman->startY, (int) outPacman->x, (int) outPacman->y);
    outPacman->dir = (Direction) tmp;

    for (int i = 0; i < 4; i++)
        get_ghost(input, outGhosts + i);
    outGhosts[0].type = BLINKY;
    outGhosts[1].type = PINKY;
    outGhosts[2].type = CLYDE;
    outGhosts[3].type = INKY;
}

void checkEatables(Map *map, Game *outGame, Pacman *outPacman, Ghost *outGhosts) {
    // fill me
}

void checkGhostCollision(Pacman *outPacman, Ghost *outGhost) {
    // fill me
}

bool isGameFinished(Game *game, Pacman *pacman) {
    //return true;
    // fill me
}

void checkGhostState(Ghost *ghost) {
    // fill me
}
