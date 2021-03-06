#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "game.h"
#include "input.h"

#define PACMAN_DEFAULT_SPEED 2.
#define GHOST_DEFAULT_SPEED 2.
#define SIZE 10000

double getNewX(const Map *map, double pos, Direction dir);
double getNewY(const Map *map, double pos, Direction dir);

// TO WRITE
Direction decidePacman(const Map *map, Pacman *pacman, Action action);

// TO WRITE
Direction decideGhost(const Map *map, Ghost *ghost, Pacman *pacman, Ghost *blinky);

Direction rand_dir(const Map *map, int gx, int gy);

Direction getDir(int x0, int y0, int x, int y, int width, int height);

Direction shortestPath(const Map *map, int x0, int y0, int x, int y);

#endif