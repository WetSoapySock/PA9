#include "../include/core/board.h"
#include <cstdlib>
#include <ctime>
#include <cstddef>

/*
 Function: Board (default constructor)
 Description: Initializes an empty board with zero dimensions, no mines,
              and sets the minesPlaced flag to false. Called before proper
              initialization via initialize().
*/
Minesweeper::Board::Board() {
    width = 0;
    height = 0;
    totalMines = 0;
    flaggedMines = 0;
    revealedCells = 0;
    minesPlaced = false;
}

/*
 Function: Board (parameterized constructor)
 Description: Creates a board with specified dimensions and mine count.
              Delegates initialization to the initialize() method.
              Parameters: w = width (columns), h = height (rows), mines = number of mines
*/
Minesweeper::Board::Board(int w, int h, int mines) {
    initialize(w, h, mines);
}

/*
 Function: initialize
 Description: Sets up the board with given dimensions and mine count.
              Creates a 2D vector of Cell objects, clears any existing data,
              seeds the random number generator, and resets all counters.
              Parameters: w = width, h = height, mines = total mines to place
*/
void Minesweeper::Board::initialize(int w, int h, int mines) {
    width = w;
    height = h;
    totalMines = mines;
    flaggedMines = 0;
    revealedCells = 0;
    minesPlaced = false;

    cells.clear();

    for (int row = 0; row < height; row++) {
        std::vector<Cell> newRow;

        for (int col = 0; col < width; col++) {
            newRow.push_back(Cell(col, row));
        }
        cells.push_back(newRow);
    }
    srand((unsigned int)time(nullptr));
}

/*
 Function: reset
 Description: Resets the board to initial state for a new game.
              Preserves dimensions and mine count but clears all flags,
              revealed cells, and resets every cell to covered/empty.
*/
void Minesweeper::Board::reset() {
    flaggedMines = 0;
    revealedCells = 0;
    minesPlaced = false;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            cells[row][col].reset();
        }
    }
}

/*
 Function: isValidPosition
 Description: Checks whether the given coordinates are within the board bounds.
              Returns true if x is between 0 and width-1, and y is between 0 and height-1.
              Parameters: x = column, y = row
*/
bool Minesweeper::Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

/*
 Function: getCell (non-const)
 Description: Returns a mutable pointer to the cell at the specified coordinates.
              Returns nullptr if coordinates are invalid. Used for modifying cells.
              Parameters: x = column, y = row
*/
Minesweeper::Cell* Minesweeper::Board::getCell(int x, int y) {
    if (isValidPosition(x, y)) {
        return &cells[y][x];
    }
    return nullptr;
}

/*
 Function: getCell (const)
 Description: Returns a read-only const pointer to the cell at specified coordinates.
              Returns nullptr if coordinates are invalid. Used for rendering and queries.
              Parameters: x = column, y = row
*/
const Minesweeper::Cell* Minesweeper::Board::getCell(int x, int y) const {
    if (isValidPosition(x, y)) {
        return &cells[y][x];
    }
    return nullptr;
}

/*
 Function: placeMines
 Description: Randomly distributes mines across the board while guaranteeing
              the first clicked cell (safeX, safeY) contains no mine.
              Places exactly totalMines mines. After placement, calls calculateNumbers().
              Parameters: safeX, safeY = coordinates of first player click
*/
void Minesweeper::Board::placeMines(int safeX, int safeY) {
    int count = 0;

    while (count < totalMines) {
        int x = rand() % width;
        int y = rand() % height;

        if (x == safeX && y == safeY) continue;

        if (!cells[y][x].isMine()) {
            cells[y][x].setType(CellType::MINE);
            count++;
        }
    }

    minesPlaced = true;
    calculateNumbers();
}

/*
 Function: calculateNumbers
 Description: Iterates through every non-mine cell and sets its number to the
              count of adjacent mines (0-8). Cells with 0 become EMPTY type.
*/
void Minesweeper::Board::calculateNumbers() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!cells[y][x].isMine()) {
                int count = getAdjacentMineCount(x, y);
                cells[y][x].setNumber(count);
            }
        }
    }
}

/*
 Function: getAdjacentMineCount
 Description: Counts how many mines are in the 8 neighboring cells of position (x,y).
              Uses the directions array to check all adjacent positions.
              Parameters: x, y = coordinates to check
*/
int Minesweeper::Board::getAdjacentMineCount(int x, int y) const {
    int count = 0;

    for (size_t i = 0; i < directions.size(); i++) {
        int newX = x + directions[i].first;
        int newY = y + directions[i].second;

        if (isValidPosition(newX, newY) && cells[newY][newX].isMine()) {
            count++;
        }
    }
    return count;
}

/*
 Function: revealCell
 Description: Handles left-click reveal logic. If mines not placed yet, places them
              with current cell as safe. Reveals the cell and checks for mine hit.
              Returns true if mine was revealed (game over), false otherwise.
              Triggers flood fill for empty cells (number == 0).
              Parameters: x, y = coordinates to reveal
*/
bool Minesweeper::Board::revealCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        return false;
    }

    if (!minesPlaced) {
        placeMines(x, y);
    }

    Cell& cell = cells[y][x];

    if (cell.isRevealed() || cell.isFlagged()) {
        return false;
    }

    cell.reveal();

    if (cell.isMine()) {
        revealAllMines();
        return true;
    }
    revealedCells++;

    if (cell.getNumber() == 0) {
        revealArea(x, y);
    }
    
    return false;
}

/*
 Function: revealArea
 Description: Flood fill algorithm that recursively reveals all connected empty cells
              (cells with 0 adjacent mines). Also reveals numbered cells adjacent to
              empty areas. Uses DFS approach through neighbor directions.
              Parameters: x, y = starting coordinates for flood fill
*/
void Minesweeper::Board::revealArea(int x, int y) {
    for (size_t i = 0; i < directions.size(); i++) {
        int newX = x + directions[i].first;
        int newY = y + directions[i].second;

        if (isValidPosition(newX, newY)) {
            Cell& neighbor = cells[newY][newX];

            if (!neighbor.isRevealed() && !neighbor.isFlagged() && !neighbor.isMine()) {
                neighbor.reveal();
                revealedCells++;

                if (neighbor.getNumber() == 0) {
                    revealArea(newX, newY);
                }
            }
        }
    }
}

/*
 Function: toggleFlag
 Description: Handles right-click flag placement and removal. Toggles cell state
              between COVERED and FLAGGED. Updates flaggedMines counter accordingly.
              Cannot flag already revealed cells.
              Parameters: x, y = coordinates to toggle flag
*/
void Minesweeper::Board::toggleFlag(int x, int y) {
    if (!isValidPosition(x, y)) return;

    Cell& cell = cells[y][x];

    if (cell.isRevealed()) return;
    
    if (cell.isFlagged()) {
        flaggedMines--;
    } else {
        flaggedMines++;
    }

    cell.toggleFlag();
}

/*
 Function: revealAllMines
 Description: Reveals all mine cells on the board. Called when player loses
              (hits a mine) to show where all mines were located.
*/
void Minesweeper::Board::revealAllMines() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (cells[y][x].isMine()) {
                cells[y][x].reveal();
            }
        }
    }
}

/*
 Function: isWinCondition
 Description: Checks if player has won the game. Victory occurs when the number
              of revealed cells equals total non-mine cells.
              Returns true if all safe cells are revealed.
*/
bool Minesweeper::Board::isWinCondition() const {
    int safeCells = width * height - totalMines;
    return revealedCells == safeCells;
}

/*
 Function: forEachCell
 Description: Iterates through every cell in the board and applies the given
              function to each cell (mutable access). Used for rendering and updates.
              Parameters: func = function to apply to each cell
*/
void Minesweeper::Board::forEachCell(std::function<void(Cell&)> func) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            func(cells[y][x]);
        }
    }
}

/*
 Function: forEachCellConst
 Description: Iterates through every cell in the board and applies the given
              function to each cell (read-only access). Used for const rendering.
              Parameters: func = function to apply to each cell
*/
void Minesweeper::Board::forEachCellConst(std::function<void(const Cell&)> func) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            func(cells[y][x]);
        }
    }
}

/*
 Function: updateNeighbors
 Description: Helper function that checks a single neighbor direction and increments
              count if that neighbor contains a mine. Used internally during number calculation.
              Parameters: x, y = base coordinates, dx, dy = direction offset, count = reference to increment
*/
void Minesweeper::Board::updateNeighbors(int x, int y, int dx, int dy, int& count) const {
    int newX = x + dx;
    int newY = y + dy;

    if (isValidPosition(newX, newY) && cells[newY][newX].isMine()) {
        count++;
    }
}

/*
 Function: resetCellFlags
 Description: Removes all flags from the board by toggling any flagged cell back to covered.
              Resets flaggedMines counter to zero. Called during board reset.
*/
void Minesweeper::Board::resetCellFlags() {
    flaggedMines = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (cells[y][x].isFlagged()) {
                cells[y][x].toggleFlag();
            }
        }
    }
}
