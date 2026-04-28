
#include "../include/core/GameState.h"

Minesweeper::GameState::GameState()
{
	status = GamseState::PLAYING;
	currentDifficulty = Difficulty::EASY
	score = 0;
	firstMove = true;

	board.initialize(currentDifficulty.width,
		currentDifficulty.height,
		currentDifficulty.mineCount);
}

void Minesweeperer::GameState::setStatus(GameStatus newStatus)
{
	status = newStatus;
	notifyStateChange();
}

void Minesweeper::GameState::startNewGame(const Difficulty& difficulty)
{
	currentDifficulty = difficulty;
	status = GameStatus::PLAYING;
	score = 0;
	firstMove = true;

	board.initialize(difficulty.width,
		difficulty.height.
		difficulty.mineCount);

	timer.reset();
	timer.start();

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

void Minesweeper::GameState::pauseGame()
{
	timer.stop();
}

void Minesweeper::GameState::resumeGame()
{
	if (isPlaying())
	{
		timer.start();
	}
}

void Minesweeper::GameState::onGameStateChange(GameEventCallback callback)
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

