#include "physics.h"
#include "map.h"
#include <stdlib.h>
#include <zconf.h>

double getNewX(const Map *map, double pos, Direction dir) {
    int w = map->width;
    if (dir == DIR_NONE || dir == DIR_UP || dir == DIR_DOWN)
        return pos;
    if (dir == DIR_RIGHT)
        return pos + 1 <= w - 1 ? pos + 1 : pos + 1 - w;
    if (dir == DIR_LEFT)
        return pos - 1 >= 0 ? pos - 1 : pos - 1 + w;
}

double getNewY(const Map *map, double pos, Direction dir) {
    int h = map->height;
    if (dir == DIR_NONE || dir == DIR_LEFT || dir == DIR_RIGHT)
        return pos;
    if (dir == DIR_DOWN)
        return pos + 1 <= h - 1 ? pos + 1 : pos + 1 - h;
    if (dir == DIR_UP)
        return pos - 1 >= 0 ? pos - 1 : pos - 1 + h;
}

void bfs(const Map *map, int init_x, int init_y, int dis[MAP_MAX_SIZE][MAP_MAX_SIZE]) {
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
            if (map->cells[nx][ny] == CELL_BLOCK)
                continue;
            if (dis[nx][ny] == -1) {
                dis[nx][ny] = dis[x][y] + 1;
                xqueue[queue_size] = nx;
                yqueue[queue_size] = ny;
                queue_size++;
            }
        }
    }
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

Direction getNearestDirection(const Map *map, Ghost *ghost, int distance[MAP_MAX_SIZE][MAP_MAX_SIZE]) {
    Direction result = DIR_NONE;
    int minimum_distance = INT_MAX;
    for (int dir = 1; dir <= 4; dir++) {
        int x = (int) getNewX(map, ghost->x, (Direction) dir);
        int y = (int) getNewY(map, ghost->y, (Direction) dir);
        if (distance[x][y] < minimum_distance && distance[x][y] != -1 && map->cells[x][y] != CELL_BLOCK) {
            result = (Direction) dir;
            minimum_distance = distance[x][y];
        }
    }
    return result;
}

Direction decideGhost(const Map *map, Ghost *ghost, Pacman *pacman, Ghost *blinky) {
    //return DIR_NONE;
    int distance[MAP_MAX_SIZE][MAP_MAX_SIZE];
    Direction result = DIR_NONE;
    switch (ghost->type) {
        case BLINKY:
            bfs(map, (int) pacman->x, (int) pacman->y, distance);
            break;
        case PINKY:;
            int current_x = (int) pacman->x;
            int current_y = (int) pacman->y;
            for (int i = 0; i < 4; i++) {
                int new_x = (int) getNewX(map, current_x, pacman->dir),
                        new_y = (int) getNewY(map, current_y, pacman->dir);
                if (map->cells[new_x][new_y] == CELL_BLOCK)
                    break;
                current_x = new_x;
                current_y = new_y;
            }
            bfs(map, current_x, current_y, distance);
            break;
        case INKY:;
            int xPacmanNext = (int) pacman->x,
                    yPacmanNext = (int) pacman->y;
            xPacmanNext = (int) getNewX(map, getNewX(map, xPacmanNext, pacman->dir), pacman->dir);
            yPacmanNext = (int) getNewY(map, getNewY(map, yPacmanNext, pacman->dir), pacman->dir);
            int symBlinkyX = ((int) (2 * xPacmanNext - blinky->x) + 10 * map->width) % map->width;
            int symBlinkyY = ((int) (2 * yPacmanNext - blinky->y) + 10 * map->height) % map->height;
            bfs(map, symBlinkyX, symBlinkyY, distance);
            break;
        case CLYDE:
            if (manhattanDistance(ghost->x, ghost->y, pacman->x, pacman->y) <= 8)
                bfs(map, 0, 0, distance);
            else
                bfs(map, (int) pacman->x, (int) pacman->y, distance);
            break;
        default:
            break;
    }
    return getNearestDirection(map, ghost, distance);
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
