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
        int index = i * g->padded_columns;
        for (int j = 1; j <= g->columns; j++) {
            if (rand() % 10 < DENSITY) {
                g->board[index + j] = ALIVE;
            }
        }
    }
}

void board_update(struct Game *g) {
    int stride = g->padded_columns;
    int start = stride + 1; // Start at (1,1)
    /* End at (rows, columns) because:
        (g->padded_rows * stride): Whole padded board
        - stride: Last padded row
        - 1: Last padded column
    */
    int end = (g->padded_rows * stride) - stride - 1;

    for (int index = start; index < end; index++) {
        int col = index % stride;
        if (col == 0 || col == stride - 1) continue; // Skip padded columns.

        // The indices of the top and bottom neighbours.
        int top = index - stride;
        int bottom = index + stride;

        int alive_neighbours =
            g->board[top - 1] +     g->board[top] +     g->board[top + 1] +
            g->board[index - 1] +                       g->board[index + 1] +
            g->board[bottom - 1] +  g->board[bottom] +  g->board[bottom + 1];

        // Apply Game of Life rules.
        bool cell = g->board[index];
        if (cell == ALIVE) {
            g->next_board[index] =
                (alive_neighbours == 2 || alive_neighbours == 3) ? ALIVE : DEAD;
        } else {
            g->next_board[index] =
                (alive_neighbours == 3) ? ALIVE : DEAD;
        }
    }

    // Swap boards.
    bool *temp = g->board;
    g->board = g->next_board;
    g->next_board = temp;
}

void board_print(const struct Game *g) {
    for (int i = 0; i < g->padded_rows; i++) {
        for (int j = 0; j < g->padded_columns; j++) {
            bool cell = g->board[i * g->padded_columns + j];
            if (cell == ALIVE) {
                printf("X ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}