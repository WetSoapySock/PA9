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
        PLAYING,     // Game is active
        GAME_OVER,   // Lose
        VICTORY      // Win
    };

    class GameState {
    private:
        GameStatus status;                           
        Board board;                               
        Timer timer;    // Timer for seconds
        Difficulty currentDifficulty;    // Difficulty settings
        bool firstMove;    // First click flag
        
        // Gen AI, Callbacks
        std::vector<std::function<void(GameStatus)>> stateChangeCallbacks;

    public:
        // Constructor and destructor
        GameState();                      
        ~GameState() = default;               

        void setStatus(GameStatus newStatus); // Changes status

        GameStatus getStatus() const {return status;}
        bool isPlaying() const {return status == GameStatus::PLAYING;}
        bool isGameOver() const {return status == GameStatus::GAME_OVER;}
        bool isVictory() const {return status == GameStatus::VICTORY;}

        void startNewGame(const Difficulty& difficulty); 
        void resetGame();
        void handleReveal(int x, int y);                  // Left click
        void handleFlag(int x, int y);                    // Right click

        Board& getBoard() {return board;}
        const Board& getBoard() const {return board;}

        Timer& getTimer() {return timer;}
        const Timer& getTimer() const {return timer;}

        Difficulty getDifficulty() const {return currentDifficulty;}
        bool isFirstMove() const {return firstMove;}

        // Gen AI, Register callback
        void onGameStateChange(std::function<void(GameStatus)> callback); 

    private:
        void notifyStateChange();  // Gen AI, Notifies player of game state
    };
}

#endif // PA9_GAMESTATE_H
