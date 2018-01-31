#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "physics.h"

void get_ghost(FILE *input, Ghost *ghost) {
    int tmp1, tmp2;
    fscanf(input, "%*s %d %d ", &tmp1, &tmp2);
    ghost->dir = (Direction) tmp1;
    ghost->blue = !(bool) tmp2;
    if (ghost->blue)
        fscanf(input, "%d ", &tmp1);
    ghost->blueCounterDown = (unsigned long long int) tmp1;
    ghost->speed = GHOST_DEFAULT_SPEED;
    fscanf(input, "(%d,%d) (%lf,%lf)", &ghost->startY, &ghost->startX, &ghost->y, &ghost->x);
}

void initiateGame(char *filename, Map *outMap, Game *outGame, Pacman *outPacman, Ghost *outGhosts) {
    FILE *input = fopen(filename, "r");
    printf("%s\n", filename);
    fscanf(input, "%d %d\n", &outMap->height, &outMap->width);
    outGame->cheeses = outGame->cherries = outGame->pineapples = 0;
    outGame->ghosts = 4;

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
    outPacman->health = 3;
    printf("%d %d %d %d\n", outPacman->startX, outPacman->startY, (int) outPacman->x, (int) outPacman->y);
    outPacman->dir = (Direction) tmp;

    //outGhosts = (Ghost *) malloc(4 * sizeof(Ghost));
    for (int i = 0; i < 4; i++)
        get_ghost(input, outGhosts + i);
    outGhosts[0].type = BLINKY;
    outGhosts[1].type = PINKY;
    outGhosts[2].type = CLYDE;
    outGhosts[3].type = INKY;
}

bool is_near_cell(double x, double y, int i, int j) {
    double dx = x - i,
            dy = y - j;
    double dis2 = dx * dx + dy * dy;
    return dis2 <= .25;
}

void checkSingleEatable(Map *map, Game *outGame, Pacman *outPacman, Ghost *outGhosts, int i, int j) {
    if (i < 0)
        i += map->width;
    if (i >= map->width)
        i -= map->width;
    if (j < 0)
        j += map->height;
    if (j >= map->height)
        j -= map->height;
    if (map->cells[i][j] == CELL_BLOCK)
        return;
    switch (map->cells[i][j]) {
        case CELL_PINEAPPLE:
            outGame->score += PINEAPPLE_SCORE;
            outGame->pineapples--;
            for (int p = 0; p < 4; p++) {
                outGhosts[p].blue = true;
                outGhosts[p].blueCounterDown = BLUE_DURATION;
            }
            break;
        case CELL_CHERRY:
            outGame->score += CHERRY_SCORE;
            outGame->cherries--;
            break;
        case CELL_CHEESE:
            outGame->score += CHEESE_SCORE;
            outGame->cheeses--;
            break;
        default:
            break;
    }
    map->cells[i][j] = CELL_EMPTY;
}

void checkEatables(Map *map, Game *outGame, Pacman *outPacman, Ghost *outGhosts) {
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (is_near_cell(outPacman->x, outPacman->y, (int) outPacman->x + i, (int) outPacman->y + j))
                checkSingleEatable(map, outGame, outPacman, outGhosts, (int) outPacman->x + i, (int) outPacman->y + j);
}

void checkGhostCollision(Pacman *outPacman, Ghost *outGhost) {
    double dx = outPacman->x - outGhost->x,
            dy = outPacman->y - outGhost->y;
    double dis2 = dx * dx + dy * dy;
    if (dis2 > .25)
        return;
    if (outGhost->blue) {
        outGhost->speed /= 2.;
        outGhost->blue = false;
        outGhost->x = outGhost->startX;
        outGhost->y = outGhost->startY;
    } else {
        outPacman->x = outPacman->startX;
        outPacman->y = outPacman->startY;
        outPacman->speed = PACMAN_DEFAULT_SPEED;
        outPacman->health--;
    }
}

bool isGameFinished(Game *game, Pacman *pacman) {
    return pacman->health <= 0 || (game->cheeses + game->pineapples == 0);
}

void checkGhostState(Ghost *ghost) {
    if (!ghost->blue)
        return;
    ghost->blueCounterDown--;
    if (ghost->blueCounterDown == 0)
        ghost->blue = false;
}
