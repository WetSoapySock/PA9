#ifndef PA9_GAME_H
#define PA9_GAME_H

#pragma once

#include "board.h"
#include "gameState.h"
#include "renderer.h"
#include "inputHandler.h"
#include "timer.h"

namespace Minesweeper {
    class Game {
    private:
        Board board;
        GameState state;
        Renderer renderer;
        InputHandler inputHandler;
        Timer timer;
        bool isRunning;
    public:
        Game();
        Game(int width, int height, int mines);
        ~Game();

        // Initizalize board
        void init(int width, int height, int mines);
        void reset();

        // Game loop
        void run();
        void update();
        void render();
        void handleInput();

        GameState getGameState() const {return state;}
        bool isRunning() const {return isRunning;}
    private:
        void checkWin();
        void lose();


    };
};
#endif //PA9_GAME_H
