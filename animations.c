#include "animations.h"

void animation_run(struct Animation *a) {
    int iteration = 0;
    while (iteration < MAX_ITERATIONS && a->is_running) {
        while (SDL_PollEvent(&a->event)) {
            if (a->event.type == SDL_EVENT_QUIT) {
                a->is_running = false;
            }
        }
        board_update(a->game);
        animation_draw(a->game);
        SDL_RenderPresent(a->renderer);
        iteration++;
    }
}

void animation_draw(const struct Game *g) {
    // Background to black.
    SDL_SetRenderDrawColor(g->renderer, COLOUR_BLACK);
    SDL_RenderClear(g->renderer);

    // Cell colour to white.
    SDL_SetRenderDrawColor(g->renderer, COLOUR_WHITE);
    SDL_FRect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    for (int i = 0; i < g->rows; i++) {
        // Compute y only once.
        rect.y = i * CELL_SIZE;
        for (int j = 0; j < g->columns; j++) {
            if (g->board[i * g->columns + j] == ALIVE) {
                rect.x = j * CELL_SIZE;
                SDL_RenderFillRect(g->renderer, &rect);
            }
        }
    }
}

bool animation_init(struct Animation **anim) {
    *anim = calloc(1, sizeof(struct Animation));
    if (*anim == NULL) {
        fprintf(stderr, "Failed to allocate memory for Animation struct.\n");
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialise SDL: %s\n", SDL_GetError());
        return false;
    }

    struct Animation *a = *anim;

    a->window = SDL_CreateWindow(WINDOW_TITLE,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        SDL_WINDOW_RESIZABLE);
    if (!a->window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return false;
    }

    a->renderer = SDL_CreateRenderer(a->window, NULL);
    if (!a->renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(a->renderer, 1)) {
        fprintf(stderr, "Failed to set VSync: %s\n", SDL_GetError());
        return false;
    }

    // Initialise game.
    if (!game_new(&a->game, a->renderer)) {
        return false;
    }

    a->is_running = true;

    return true;                       
}

void animation_free(struct Animation **anim) {
    if (*anim) {

        if ((*anim)->game) {
            board_free(&(*anim)->game);
            (*anim)->game = NULL;
        }

        if ((*anim)->renderer) {
            SDL_DestroyRenderer((*anim)->renderer);
            (*anim)->renderer = NULL;
        }

        if ((*anim)->window) {
            SDL_DestroyWindow((*anim)->window);
            (*anim)->window = NULL;
        }

        SDL_Quit();

        free(*anim);
        *anim = NULL;
        printf("Animation resources freed.\n");
    }
}