#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "main.h"
#include "game.h"

struct Animation {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    struct Game *game;
    bool is_running;
};

// Function declarations
void animation_run(struct Animation *a);
void animation_draw(const struct Game *g);
bool animation_init(struct Animation **anim);
void animation_free(struct Animation **anim);

#endif // ANIMATIONS_H