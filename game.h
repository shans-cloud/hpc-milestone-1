#ifndef GAME_H
#define GAME_H

#include "main.h"

// Cell states
#define ALIVE 1
#define DEAD 0

struct Game {
    bool *board;
    bool *next_board;
    int rows;
    int columns;
    int padded_rows;
    int padded_columns;
    SDL_Renderer *renderer;
};

// Function declarations
bool game_new(struct Game **game, SDL_Renderer *renderer);
void game_run(struct Game *g);
void board_free(struct Game **game);
void board_populate(struct Game *game);
void board_print(const struct Game *g);
void board_update(struct Game *game);
int cell_neighbours(struct Game *g, int i, int j);

#endif // GAME_H