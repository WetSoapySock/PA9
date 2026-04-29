#ifndef PA9_GAMESTATE_H
#define PA9_GAMESTATE_H

#pragma once

#include <vector>
#include <functional>

#include "board.h"
#include "../utils/timer.h"
#include "../utils/constants.h"

namespace Minesweeper {
    enum class GameStatus {
        PLAYING,
        GAME_OVER,
        VICTORY
    };

    class GameState {
    private:
        GameStatus status;
        Board board;
        Timer timer;
        Difficulty currentDifficulty;
        bool firstMove;

        // Callback system for state changes (optional, can be removed if not used)
        std::vector<std::function<void(GameStatus)>> stateChangeCallbacks;

    public:
        // Constructor and destructor
        GameState();
        ~GameState() = default;

        // Setter
        void setStatus(GameStatus newStatus);

        // Getter
        GameStatus getStatus() const {return status;}

        bool isPlaying() const {return status == GameStatus::PLAYING;}
        bool isGameOver() const {return status == GameStatus::GAME_OVER;}
        bool isVictory() const {return status == GameStatus::VICTORY;}

        void startNewGame(const Difficulty& difficulty);
        void resetGame();
        void handleReveal(int x, int y);
        void handleFlag(int x, int y);

        Board& getBoard() {return board;}     // Get reference to board
        const Board& getBoard() const {return board;}   // Get read only reference to board

        Timer& getTimer() {return timer;}     // Get reference to timer
        const Timer& getTimer() const {return timer;} // Get read only reference to timer

        Difficulty getDifficulty() const {return currentDifficulty;}

        bool isFirstMove() const {return firstMove;}

        // ======================== CALLBACK SYSTEM (Optional) ========================
        void onGameStateChange(std::function<void(GameStatus)> callback);

    private:
        void notifyStateChange();
    };
}

#endif // PA9_GAMESTATE_H
