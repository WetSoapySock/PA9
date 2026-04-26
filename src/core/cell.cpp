
#include "../include/core/Cell.h"

/*
  Function: Minesweeper::Cell::Cell()
  Date Created : 04/26/2026
  Date Last Modified : 04/26/2026
  Description : Initialize a new cell obj
  Input parameters : N\A
  Returns : N\A
  Preconditions :
  Postconditions:
  */
Minesweeper::Cell::Cell()
{
	isMine = false;
	isRevealed = false;
	isFlagged = false;
	adjacentMines = 0;

	/*this creates an empty cell that can be modified later
	to include any of these conditions */
}

/*
  Function: Minesweeper::Cell::~Cell()
  Date Created : 04/26/2026
  Date Last Modified : 04/26/2026
  Description : Destructs the cell obj
  Input parameters : N\A
  Returns : N\A
  Preconditions :
  Postconditions:
  */
Minesweeper::Cell::~Cell()
{
	//No dynamic memory is used so this is empty
}

/*
  Function: bool Minesweeper::Cell::reveal()
  Date Created : 04/26/2026
  Date Last Modified : 04/26/2026
  Description : Reveals the cell
  Input parameters : N\A
  Returns : bool
  Preconditions : Cannot be a flagged cell or an already revealed cell
  Postconditions:
  */
bool Minesweeper::Cell::reveal()
{
	//check to make sure the cell is not flagged
	if (isFlagged == true)
	{
		return false;
	}
	//check to make sure its not already revealed
	if (isRevealed == true)
	{
		return false;
	}
	
	//change the cell state to revealed
	isRevealed = true;

	//once revealed check to see if its a mine
	if (isMine == true)
	{
		//use this to indicate whether its gameover or not
		return true;
	}

	return false;
}

/*
  Function: void Minesweeper::Cell::toggleFlag()
  Date Created : 04/26/2026
  Date Last Modified : 04/26/2026
  Description : Toggles the flag condition on a cell
  Input parameters : N\A
  Returns :N\A
  Preconditions :Cell cannot be revealed
  Postconditions:
  */
void Minesweeper::Cell::toggleFlag()
{
	//check to see if the cell is revealed
	if (isRevealed == false)
	{
		//We check to see if its flagged so we can switch the values
		if (isFlagged == true)
		{
			isFlagged = false;
		}
		// This is for the case it isn't flagged
		else
		{
			isFlagged = true;
		}
	}

}

/*
  Function: void Minesweeper::Cell::reset()
  Date Created : 04/26/2026
  Date Last Modified : 04/26/2026
  Description : This resets the cell to its base conditions
  Input parameters : N\A
  Returns : N\A
  Preconditions :
  Postconditions:
  */
void Minesweeper::Cell::reset()
{
	isMine = false;
	isRevealed = false;
	isFlagged = false;
	adjacentMines = 0;
}
