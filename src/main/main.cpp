#include "../include/core/Game.h"
#include "../include/utils/Constants.h"

int main()
{
    Minesweeper::Game game;

    if (!game.initialize(
        Minesweeper::MIN_WINDOW_WIDTH,
        Minesweeper::MIN_WINDOW_HEIGHT))
    {
        return 1;
    }

    game.run();

    return 0;
}