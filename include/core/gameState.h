#ifndef PA9_GAMESTATE_H
#define PA9_GAMESTATE_H

#pragma once

#include <vector>
#include <functional>

#include "Board.h"
#include "../utils/Timer.h"
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
        int score;
        bool firstMove;

    public:
        GameState();
        ~GameState() = default;

        // State management
        void setStatus(GameStatus newStatus);
        GameStatus getStatus() const { return status; }
        bool isPlaying() const { return status == GameStatus::PLAYING; }
        bool isGameOver() const { return status == GameStatus::GAME_OVER; }
        bool isVictory() const { return status == GameStatus::VICTORY; }

        // Game actions
        void startNewGame(const Difficulty& difficulty);
        void resetGame();
        void handleReveal(int x, int y);
        void handleFlag(int x, int y);
        void pauseGame();
        void resumeGame();

        // Accessors
        Board& getBoard() { return board; }
        const Board& getBoard() const { return board; }
        Timer& getTimer() { return timer; }
        const Timer& getTimer() const { return timer; }
        Difficulty getDifficulty() const { return currentDifficulty; }

        // Events
        using GameEventCallback = std::function<void(GameStatus)>;
        void onGameStateChange(GameEventCallback callback);

    private:
        std::vector<GameEventCallback> stateChangeCallbacks;
        void notifyStateChange();
    };
}
#endif //PA9_GAMESTATE_H
