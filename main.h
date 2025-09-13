#ifndef MAIN_H
#define MAIN_H

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define WINDOW_TITLE "Cellular Automaton - Game of Life"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define CELL_SIZE 5
#define DENSITY 357 // 35.7% chance of a cell being alive at start.
#define COLOUR_WHITE 255, 255, 255, 255
#define COLOUR_BLACK 0, 0, 0, 255
#define SEED 42 // Fixed seed for reproducibility.
#define MAX_ITERATIONS 1000 // Benchmarking limit.
#define WITH_ANIMATIONS 0 // 1 for yes, 0 for no

#endif // MAIN_H