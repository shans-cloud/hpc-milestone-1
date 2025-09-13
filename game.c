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

    // Optimisation: Padding board to avoid boundary checks.
    g->padded_rows = g->rows + 2;
    g->padded_columns = g->columns + 2;

    g->board = calloc((size_t)(g->padded_rows * g->padded_columns), sizeof(bool));
    g->next_board = calloc((size_t)(g->padded_rows * g->padded_columns), sizeof(bool));

    if (!g->board || !g->next_board) {
        fprintf(stderr, "Failed to allocate memory for boards.\n");
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
        }
        if (g->next_board) {
            free(g->next_board);
            g->next_board = NULL;
        }
        printf("Boards freed.\n");

        g->renderer = NULL;
        free(*game);
        *game = NULL;
        printf("Game resources freed.\n");
    }
}

void board_populate(struct Game *g) {
    srand(SEED);
    // Initialising only the inner board, leaving the padding as DEAD.
    for (int i = 1; i <= g->rows; i++) {
        for (int j = 1; j <= g->columns; j++) {
            g->board[i * g->padded_columns + j] = 
            (rand() % 1000) < DENSITY ? ALIVE : DEAD;
        }
    }
}

void board_update(struct Game *g) {
    for (int i = 1; i <= g->rows; i++) {
        for (int j = 1; j <= g->columns; j++) {
            int alive_neighbours = cell_neighbours(g, i, j);

            // Apply Game of Life rules.
            if (g->board[i * g->padded_columns + j] == ALIVE) {
                g->next_board[i * g->padded_columns + j] =
                    (alive_neighbours == 2 || alive_neighbours == 3) ? ALIVE : DEAD;
            } else {
                g->next_board[i * g->padded_columns + j] =
                    (alive_neighbours == 3) ? ALIVE : DEAD;
            }
        }
    }

    // Swap boards.
    bool *temp = g->board;
    g->board = g->next_board;
    g->next_board = temp;
}

int cell_neighbours(struct Game *g, int i, int j) {
    int count = 0;
    // Top row
    count += g->board[(i-1) * g->padded_columns + (j-1)];
    count += g->board[(i-1) * g->padded_columns + (j)];
    count += g->board[(i-1) * g->padded_columns + (j+1)];

    // Middle row
    count += g->board[(i)   * g->padded_columns + (j-1)];
    count += g->board[(i)   * g->padded_columns + (j+1)];

    // Bottom row
    count += g->board[(i+1) * g->padded_columns + (j-1)];
    count += g->board[(i+1) * g->padded_columns + (j)];
    count += g->board[(i+1) * g->padded_columns + (j+1)];

    return count;
}

void board_print(const struct Game *g) {
    for (int i = 0; i < g->padded_rows; i++) {
        for (int j = 0; j < g->padded_columns; j++) {
            if (i == 0 || i == g->padded_rows - 1 || j == 0 || j == g->padded_columns - 1) {
                // Padding border.
                printf("-");
            } else {
                // Real board cells.
                printf("%c", g->board[i * g->padded_columns + j] == ALIVE ? '#' : '.');
            }
        }
        printf("\n");
    }
    printf("\n");
}