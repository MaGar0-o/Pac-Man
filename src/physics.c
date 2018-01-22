#include "physics.h"
#include <stdlib.h>

double getNewX(const Map *map, double pos, Direction dir) {
    int w = map->width;
    if (dir == DIR_NONE || dir == DIR_UP || dir == DIR_DOWN)
        return pos;
    if (dir == DIR_RIGHT)
        return pos + 1 < w ? pos + 1 : pos + 1 - w;
    if (dir == DIR_LEFT)
        return pos - 1 >= 0 ? pos - 1 : pos - 1 + w;
}

double getNewY(const Map *map, double pos, Direction dir) {
    int h = map->height;
    if (dir == DIR_NONE || dir == DIR_LEFT || dir == DIR_RIGHT)
        return pos;
    if (dir == DIR_DOWN)
        return pos + 1 < h ? pos + 1 : pos + 1 - h;
    if (dir == DIR_UP)
        return pos - 1 >= 0 ? pos - 1 : pos - 1 + h;
}

Direction decideGhost(const Map *map, Ghost *ghost, Pacman *pacman, Ghost *blinky) {
    int sz = 1;
    Direction allowedDirs[5];
    allowedDirs[0] = DIR_NONE;
    for (int dir = DIR_UP; dir <= DIR_LEFT; dir++) {
        int newX = (int) getNewX(map, ghost->x, (Direction) dir);
        int newY = (int) getNewY(map, ghost->y, (Direction) dir);
        if (map->cells[newX][newY] != CELL_BLOCK)
            allowedDirs[sz++] = (Direction) dir;
    }
    return allowedDirs[rand() % sz];
}

Direction decidePacman(const Map *map, Pacman *pacman, Action action) {
    bool thisWayIsBlock;
    {
        Direction newDir = pacman->dir;
        int newX = (int) getNewX(map, pacman->x, (Direction) newDir);
        int newY = (int) getNewY(map, pacman->y, (Direction) newDir);
        thisWayIsBlock = (map->cells[newX][newY] == CELL_BLOCK);
    }
    if (action & MOVE_ACTION_MASK == 0)
        return thisWayIsBlock ? DIR_NONE : pacman->dir;

    Direction newDir;
    switch (action) {
        case ACTION_DOWN:
            newDir = DIR_DOWN;
            break;
        case ACTION_LEFT:
            newDir = DIR_LEFT;
            break;
        case ACTION_RIGHT:
            newDir = DIR_RIGHT;
            break;
        case ACTION_UP:
            newDir = DIR_UP;
            break;
        default:
            newDir = DIR_NONE;
    }

    int newX = (int) getNewX(map, pacman->x, (Direction) newDir);
    int newY = (int) getNewY(map, pacman->y, (Direction) newDir);

    if (map->cells[newX][newY] == CELL_BLOCK)
        return thisWayIsBlock ? DIR_NONE : pacman->dir;
    return newDir;
}
