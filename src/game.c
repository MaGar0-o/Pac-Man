#include "game.h"
#include "map.h"
#include <stdio.h>

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
    printf("%d %d %d %d\n", outPacman->startX, outPacman->startY, (int)outPacman->x, (int)outPacman->y);
    outPacman->dir = (Direction) tmp;
    fscanf(input, "blinky: %*d %*d (%d,%d) (%lf,%lf)", &outGhosts[0].startX, &outGhosts[0].startY, &outGhosts[0].x,
           &outGhosts[0].y);
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
