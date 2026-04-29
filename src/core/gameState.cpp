
#include "../include/core/GameState.h"

Minesweeper::GameState::GameState()
{
    status = GameStatus::PLAYING;
    currentDifficulty = Difficulty::EASY;
    firstMove = true;

    board.initialize(currentDifficulty.width,
        currentDifficulty.height,
        currentDifficulty.mineCount);
}

void Minesweeper::GameState::setStatus(GameStatus newStatus)
{
    status = newStatus;
    notifyStateChange();
}

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

void Minesweeper::GameState::resetGame()
{
    startNewGame(currentDifficulty);
}

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

void Minesweeper::GameState::handleFlag(int x, int y)
{
    if (!isPlaying())
    {
        return;
    }

    board.toggleFlag(x, y);
}

void Minesweeper::GameState::onGameStateChange(std::function<void(GameStatus)> callback)
{
    stateChangeCallbacks.push_back(callback);
}

void Minesweeper::GameState::notifyStateChange()
{
    for (int i = 0; i < stateChangeCallbacks.size(); i++)
    {
        stateChangeCallbacks[i](status);
    }
}