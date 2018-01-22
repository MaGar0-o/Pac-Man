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

void bfs(Map *map, int init_x, int init_y, int **dis) {
    for (int i = 0; i < map->height; i++)
        for (int j = 0; j < map->width; j++)
            dis[j][i] = -1;

    int xqueue[map->width * map->height],
            yqueue[map->width * map->height],
            queue_size = 0;

    dis[init_x][init_y] = 0;
    xqueue[queue_size] = init_x;
    yqueue[queue_size] = init_y;
    queue_size++;

    for (int i = 0; i < queue_size; i++) {
        int x = xqueue[i],
                y = yqueue[i];
        int nx, ny;
        for (int dir = 1; dir <= 4; dir++) {
            nx = (int) getNewX(map, x, (Direction) dir);
            ny = (int) getNewY(map, y, (Direction) dir);
            if (dis[nx][ny] == -1)
                dis[nx][ny] = dis[x][y] + 1;
            if (map->cells[nx][ny] == CELL_BLOCK)
                continue;
            xqueue[queue_size] = nx;
            yqueue[queue_size] = ny;
            queue_size++;
        }
    }
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

Direction decideGhost(const Map *map, Ghost *ghost, Pacman *pacman, Ghost *blinky) {
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
