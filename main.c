#include "game.h"
#include "animations.h"

int main() {

    if (WITH_ANIMATIONS) {
        // Run with animations.
        struct Animation *anim = NULL;

        if (!animation_init(&anim)) {
            animation_free(&anim);
            return EXIT_FAILURE;
        }

        animation_run(anim);
        
        animation_free(&anim);
    } else {
        // Run logic only.
        struct Game *game = NULL;

        if (!game_new(&game, NULL)) {
            board_free(&game);
            return EXIT_FAILURE;
        }

        game_run(game);

        board_free(&game);
    }

    return EXIT_SUCCESS;
}