#include "../include/core/cell.h"

/*
 Function: Cell (default constructor)
 Description: Initializes a cell at position (0,0) with EMPTY type and COVERED state.
              Used when creating cells before proper position assignment.
*/
Minesweeper::Cell::Cell()
{
    x = 0;
    y = 0;

    type = CellType::EMPTY;
    state = CellState::COVERED;
}

/*
 Function: Cell (parameterized constructor)
 Description: Initializes a cell at the specified coordinates with EMPTY type
              and COVERED state. Called when building the board grid.
              Parameters: posX = column coordinate, posY = row coordinate
*/
Minesweeper::Cell::Cell(int posX, int posY)
{
    x = posX;
    y = posY;
    type = CellType::EMPTY;
    state = CellState::COVERED;
}

/*
 Function: getNumber
 Description: Returns the numeric value associated with the cell type.
              For mines, returns -1 to distinguish from numbered cells (0-8).
              For numbered cells, returns the integer value (1-8).
              For empty cells (0 mines adjacent), returns 0.
*/
int Minesweeper::Cell::getNumber() const
{
    if (type == CellType::MINE)
    {
        return -1;
    }
    return static_cast<int>(type);
}

/*
 Function: setNumber
 Description: Sets the cell type based on the number of adjacent mines.
              Valid numbers are 0 through 8, which map to EMPTY and NUMBER_1 through NUMBER_8.
              Invalid numbers (outside 0-8) are ignored.
              Parameters: number = count of adjacent mines (0-8)
*/
void Minesweeper::Cell::setNumber(int number)
{
    if (number >= 0 && number <= 8)
    {
        type = static_cast<CellType>(number);
    }
}

/*
 Function: reset
 Description: Resets the cell to its default state for a new game.
              Sets type to EMPTY and state to COVERED, clearing any
              mine or number that was previously assigned.
*/
void Minesweeper::Cell::reset()
{
    type = CellType::EMPTY;
    state = CellState::COVERED;
}

/*
 Function: operator==
 Description: Equality comparison operator for cells. Returns true if all
              cell properties (position, state, and type) are identical.
              Used for testing and debugging purposes.
              Parameters: other = reference to cell to compare against
*/
bool Minesweeper::Cell::operator==(const Cell& other) const
{
    return (x == other.x && y == other.y && state == other.state && type == other.type);
}
