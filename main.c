#include "game.h"

int main() {
    struct Game *game = NULL;

    if (!game_new(&game)) {
        board_free(&game);
        return EXIT_FAILURE;
    }

    game_run(game);
    
    board_free(&game);

    return EXIT_SUCCESS;
}