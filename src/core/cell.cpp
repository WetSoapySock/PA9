#include "../include/core/Cell.h"

Minesweeper::Cell::Cell()
{
	x = 0;
	y = 0;

	type = CellType::EMPTY;
	state = CellState::COVERED;
}

Minesweeper::Cell::Cell(int posX, int posY)
{
	x = posX;
	y = posY;
	type = CellType::EMPTY;
	state = CellState::COVERED;
}

int Minesweeper::Cell::getNumber() const
{
	if (type == CellType::MINE)
	{
		return -1;
	}
	return static_cast<int>(type);
}

void Minesweeper::Cell::setNumber(int number)
{
	if (number >= 0 && number <= 8)
	{
		type = static_cast<CellType>(number);
	}
}

void Minesweeper::Cell::reset()
{
	type = CellType::EMPTY;
	state = CellState::COVERED;
}

bool Minesweeper::Cell::operator==(const Cell& other)const
{
	return (x == other.x && y == other.y && state == other.state&& type == other.type);
}