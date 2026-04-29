
#include "../include/core/Board.h"
#include <cstdlib>
#include <ctime>
#include <cstddef>

Minesweeper::Board::Board()
{
	width = 0;
	height = 0;
	totalMines = 0;
	flaggedMines = 0;
	revealedCells = 0;
	minesPlaced = false;
}

Minesweeper::Board::Board(int w, int h, int mines)
{
	initialize(w, h, mines);
}

void Minesweeper::Board::initialize(int w, int h, int mines)
{
	width = w;
	height = h;
	totalMines = mines;
	flaggedMines = 0;
	revealedCells = 0;
	minesPlaced = false;

	cells.clear();

	for (int row = 0; row < height; row++)
	{
		std::vector<Cell> newRow;

		for (int col = 0; col < width; col++)
		{
			newRow.push_back(Cell(col, row));
		}
		cells.push_back(newRow);
	}
	srand((unsigned int)time(nullptr));
}

void Minesweeper::Board::reset()
{
	flaggedMines = 0;
	revealedCells = 0;
	minesPlaced = false;

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			cells[row][col].reset();
		}
	}
}

bool Minesweeper::Board::isValidPosition(int x, int y)const
{
	return x >= 0 && x < width && y >= 0 && y < height;
}

Minesweeper::Cell* Minesweeper::Board::getCell(int x, int y)
{
	if (isValidPosition(x, y))
	{
		return &cells[y][x];
	}
	return nullptr;
}

const Minesweeper::Cell* Minesweeper::Board::getCell(int x, int y)const
{
	if (isValidPosition(x, y))
	{
		return& cells[y][x];
	}
	return nullptr;
}

void Minesweeper::Board::placeMines(int safeX, int safeY)
{
	int count = 0;

	while (count < totalMines)
	{
		int x = rand() % width;
		int y = rand() % height;

		if (x == safeX && y == safeY)
		{
			continue;
		}

		if (!cells[y][x].isMine())
		{
			cells[y][x].setType(CellType::MINE);
			count++;
		}
	}

	minesPlaced = true;
	calculateNumbers();
}

void Minesweeper::Board::calculateNumbers()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!cells[y][x].isMine())
			{
				int count = getAdjacentMineCount(x, y);
				cells[y][x].setNumber(count);
			}
		}
	}
}

int Minesweeper::Board::getAdjacentMineCount(int x, int y)const
{
	int count = 0;

	for (size_t i = 0; i < directions.size(); i++)
	{
		int newX = x + directions[i].first;
		int newY = y + directions[i].second;

		if (isValidPosition(newX, newY) && cells[newY][newX].isMine())
		{
			count++;
		}
	}

	return count;
}

bool Minesweeper::Board::revealCell(int x, int y)
{
	if (!isValidPosition(x, y))
	{
		return false;
	}

	if (!minesPlaced)
	{
		placeMines(x, y);
	}

	Cell& cell = cells[y][x];

	if (cell.isRevealed() || cell.isFlagged())
	{
		return false;
	}

	cell.reveal();

	if (cell.isMine())
	{
		revealAllMines();
		return true;
	}
	revealedCells++;

	if (cell.getNumber() == 0)
	{
		revealArea(x, y);
	}
	return false;
}

void Minesweeper::Board::revealArea(int x, int y)
{
	for (size_t i = 0; i < directions.size(); i++)
	{
		int newX = x + directions[i].first;
		int newY = y + directions[i].second;

		if (isValidPosition(newX, newY))
		{
			Cell& neighbor = cells[newY][newX];

			if (!neighbor.isRevealed() && !neighbor.isFlagged() && !neighbor.isMine())
			{
				neighbor.reveal();
				revealedCells++;

				if (neighbor.getNumber() == 0)
				{
					revealArea(newX, newY);
				}
			}
		}
	}
}

void Minesweeper::Board::toggleFlag(int x, int y)
{
	if (!isValidPosition(x, y))
	{
		return;
	}

	Cell& cell = cells[y][x];

	if (cell.isRevealed())
	{
		return;
	}

	if (cell.isFlagged())
	{
		flaggedMines--;
	}
	else
	{
		flaggedMines++;
	}

	cell.toggleFlag();
}

void Minesweeper::Board::revealAllMines()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (cells[y][x].isMine())
			{
				cells[y][x].reveal();
			}
		}
	}
}

bool Minesweeper::Board::isWinCondition()const
{
	int safeCells = width * height - totalMines;
	return revealedCells == safeCells;
}

void Minesweeper::Board::forEachCell(std::function<void(Cell&)> func)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			func(cells[y][x]);
		}
	}
}

void Minesweeper::Board::forEachCellConst(std::function<void(const Cell&)> func)const
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			func(cells[y][x]);
		}
	}
}

void Minesweeper::Board::updateNeighbors(int x, int y, int dx, int dy, int& count)const
{
	int newX = x + dx;
	int newY = y + dy;

	if (isValidPosition(newX, newY) && cells[newY][newX].isMine())
	{
		count++;
	}
}

void Minesweeper::Board::resetCellFlags()
{
	flaggedMines = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (cells[y][x].isFlagged())
			{
				cells[y][x].toggleFlag();
			}
		}
	}
}