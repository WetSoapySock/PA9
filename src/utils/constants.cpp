#include "../include/utils/constants.h"

/*
 Constant: Difficulty::EASY
 Description: Predefined difficulty for beginner players. Creates a 9x9 grid
			  with 10 mines (12.3% mine density). Standard Google Minesweeper
			  dimensions for Easy mode.
*/
const Minesweeper::Difficulty Minesweeper::Difficulty::EASY =
{
	"Easy", 9, 9, 10
};

/*
 Constant: Difficulty::MEDIUM
 Description: Predefined difficulty for intermediate players. Creates a 16x16 grid
			  with 40 mines (15.6% mine density). Standard Google Minesweeper
			  dimensions for Medium mode.
*/
const Minesweeper::Difficulty Minesweeper::Difficulty::MEDIUM =
{
	"Medium", 16, 16, 40
};

/*
 Constant: Difficulty::HARD
 Description: Predefined difficulty for advanced players. Creates a 30x16 grid
			  (30 columns, 16 rows) with 99 mines (20.6% mine density).
			  Standard Google Minesweeper dimensions for Hard mode.
			  Note: Width=30, Height=16 (wider than tall)
*/
const Minesweeper::Difficulty Minesweeper::Difficulty::HARD =
{
	"Hard", 30, 16, 99
};

/*
 Constant: Difficulty::CUSTOM
 Description: Placeholder difficulty for user-defined settings. Defaults to
			  Easy dimensions (9x9, 10 mines). Actual values are set at
			  runtime based on player preferences.
*/
const Minesweeper::Difficulty Minesweeper::Difficulty::CUSTOM =
{
	"Custom", 9, 9, 10	// Doesn't do anything
};
