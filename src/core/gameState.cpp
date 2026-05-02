#include "../include/core/gameState.h"

/*
 Function: GameState (constructor)
 Description: Initializes the game state with PLAYING status, EASY difficulty,
              and firstMove flag set to true. Creates a default board with
              Easy difficulty dimensions (9x9, 10 mines).
*/
Minesweeper::GameState::GameState()
{
    status = GameStatus::PLAYING;
    currentDifficulty = Difficulty::EASY;
    firstMove = true;

    board.initialize(currentDifficulty.width,
        currentDifficulty.height,
        currentDifficulty.mineCount);
}

/*
 Function: setStatus
 Description: Changes the current game status (PLAYING, GAME_OVER, VICTORY)
              and notifies all registered callbacks of the state change.
              Parameters: newStatus = the new game status to set
*/
void Minesweeper::GameState::setStatus(GameStatus newStatus)
{
    status = newStatus;
    notifyStateChange();
}

/*
 Function: startNewGame
 Description: Begins a fresh game with the specified difficulty settings.
              Resets the board, timer, and flags. Sets status to PLAYING.
              Parameters: difficulty = struct containing width, height, and mine count
*/
void Minesweeper::GameState::startNewGame(const Difficulty& difficulty)
{
    currentDifficulty = difficulty;
    status = GameStatus::PLAYING;
    firstMove = true;

    board.initialize(difficulty.width,
        difficulty.height,
        difficulty.mineCount);

    timer.reset();

    notifyStateChange();
}

/*
 Function: resetGame
 Description: Restarts the current game using the existing difficulty settings.
              Equivalent to calling startNewGame with the current difficulty.
*/
void Minesweeper::GameState::resetGame()
{
    startNewGame(currentDifficulty);
}

/*
 Function: handleReveal
 Description: Processes a left-click reveal action on a cell. If it's the first move,
              starts the timer. Reveals the cell and checks for mine hit or win condition.
              Updates game status to GAME_OVER or VICTORY accordingly.
              Parameters: x, y = coordinates of the cell to reveal
*/
void Minesweeper::GameState::handleReveal(int x, int y)
{
    if (!isPlaying())
    {
        return;
    }

    if (firstMove)
    {
        timer.start();
    }

    bool hitMine = board.revealCell(x, y);
    firstMove = false;

    if (hitMine)
    {
        timer.stop();
        setStatus(GameStatus::GAME_OVER);
        return;
    }

    if (board.isWinCondition())
    {
        timer.stop();
        setStatus(GameStatus::VICTORY);
    }
}

/*
 Function: handleFlag
 Description: Processes a right-click flag action on a cell. Toggles a flag
              on the specified cell if the game is currently in PLAYING state.
              Parameters: x, y = coordinates of the cell to flag/unflag
*/
void Minesweeper::GameState::handleFlag(int x, int y)
{
    if (!isPlaying())
    {
        return;
    }

    board.toggleFlag(x, y);
}

/*
 Function: onGameStateChange
 Description: Registers a callback function to be invoked whenever the game
              status changes. Used by UI components to react to win/loss events.
              Parameters: callback = function to call with new GameStatus
*/
void Minesweeper::GameState::onGameStateChange(std::function<void(GameStatus)> callback)
{
    stateChangeCallbacks.push_back(callback);
}

/*
 Function: notifyStateChange
 Description: Internal helper function that iterates through all registered
              callbacks and invokes each one with the current game status.
              Called automatically when setStatus() changes the state.
*/
void Minesweeper::GameState::notifyStateChange()
{
    for (int i = 0; i < stateChangeCallbacks.size(); i++)
    {
        stateChangeCallbacks[i](status);
    }
}
