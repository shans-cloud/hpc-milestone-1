#include "game.h"

bool game_new(struct Game **game, SDL_Renderer *renderer) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Failed to allocate memory for Game struct.\n");
        return false;
    }

    struct Game *g = *game;
    g->renderer = renderer;
    g->rows = WINDOW_HEIGHT / CELL_SIZE;
    g->columns = WINDOW_WIDTH / CELL_SIZE;
    g->board = calloc((size_t)(g->rows * g->columns), sizeof(bool));
    if (g->board == NULL) {
        fprintf(stderr, "Failed to allocate memory for board.\n");
        return false;
    }

    g->next_board = calloc((size_t)(g->rows * g->columns), sizeof(bool));
    if (g->next_board == NULL) {
        fprintf(stderr, "Failed to allocate memory for next_board.\n");
        return false;
    }
    
    // Initialize the board with a random population.
    board_populate(g);
    return true;
}

void game_run(struct Game *game) {
    int iteration = 0;
    while (iteration < MAX_ITERATIONS) {
        board_update(game);
        iteration++;
    }
}

void board_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        if (g->board) {
            free(g->board);
            g->board = NULL;
            printf("Board memory freed.\n");
        }
        if (g->next_board) {
            free(g->next_board);
            g->next_board = NULL;
            printf("Next board memory freed.\n");
        }
        g->renderer = NULL;
        free(*game);
        *game = NULL;
        printf("Game resources freed.\n");
    }
}

void board_populate(struct Game *g) {
    srand(SEED);
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->columns; j++) {
            g->board[i * g->columns + j] = (rand() % 1000) < DENSITY ? ALIVE : DEAD;
        }
    }
}

void board_update(struct Game *g) {
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->columns; j++) {
            int alive_neighbours = cell_neighbours(g, i, j);

            // Apply Game of Life rules.
            if (g->board[i * g->columns + j] == ALIVE) {
                if (alive_neighbours < 2 || alive_neighbours > 3) {
                    g->next_board[i * g->columns + j] = DEAD; 
                } else {
                    g->next_board[i * g->columns + j] = ALIVE; 
                }
            } else {
                if (alive_neighbours == 3) {
                    g->next_board[i * g->columns + j] = ALIVE; 
                } else {
                    g->next_board[i * g->columns + j] = DEAD; 
                }
            }
        }
    }

    // Swap boards.
    bool *temp = g->board;
    g->board = g->next_board;
    g->next_board = temp;
}

int cell_neighbours(struct Game *g, int i, int j) {
    int alive_neighbours = 0;

    // Top-left
    if (i > 0 && j > 0) alive_neighbours += g->board[(i-1) * g->columns + (j-1)];
    
    // Top
    if (i > 0) alive_neighbours += g->board[(i-1) * g->columns + j];

    // Top-right
    if (i > 0 && j < g->columns - 1)
        alive_neighbours += g->board[(i-1) * g->columns + (j+1)];  

    // Left
    if (j > 0) {
        alive_neighbours += g->board[i * g->columns + (j-1)];
        if (alive_neighbours > 3) return alive_neighbours;
    }
    // Right
    if (j < g->columns - 1) {
        alive_neighbours += g->board[i * g->columns + (j+1)];
        if (alive_neighbours > 3) return alive_neighbours;
    }

    // Bottom-left
    if (i < g->rows - 1 && j > 0) {
        alive_neighbours += g->board[(i+1) * g->columns + (j-1)];
        if (alive_neighbours > 3) return alive_neighbours;
    }
    // Bottom
    if (i < g->rows - 1) {
        alive_neighbours += g->board[(i+1) * g->columns + j];
        if (alive_neighbours > 3) return alive_neighbours;
    }
    // Bottom-right
    if (i < g->rows - 1 && j < g->columns - 1) {
        alive_neighbours += g->board[(i+1) * g->columns + (j+1)];
        if (alive_neighbours > 3) return alive_neighbours;
    }

    return alive_neighbours;
}

void board_print(const struct Game *g) {
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->columns; j++) {
            if (g->board[i * g->columns + j] == ALIVE) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}