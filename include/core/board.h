#include "cell.h"
// AI Gen, libraries
#include <vector>
#include <functional>
#include <utility>

#ifndef PA9_BOARD_H
#define PA9_BOARD_H

#pragma once

/** AI Gen, "For creating a 2d gameboard, would arrays or vectors be more useful and simplistic?
    If vectors are, help create a structure for the board. **/

namespace Minesweeper {
    class Board {     // Mages the Minesweeper game board
    private:
        std::vector<std::vector<Cell>> cells;
        int width;
        int height;
        int totalMines;
        int flaggedMines;
        int revealedCells;
        bool minesPlaced;

        // Gen AI, Neighbor directions (including diagonals)
        const std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},     // Top cells, Top left, center, and right
            {0, -1},           {0, 1},      // Left and right cells
            {1, -1},  {1, 0},  {1, 1}   // Bottom cells, Bottom left, center, and right
        };

    public:
        Board();
        // Gen AI, Creates board with specified dimensions and mine count
        explicit Board(int w, int h, int mines);
        ~Board() = default;

        // Initialization
        void initialize(int w, int h, int mines);
        void reset();
        void placeMines(int safeX, int safeY);  // Places mines on the board with cell safety and guarantees first mine is safe
        void calculateNumbers();    // Calculate cell values 1 - 8 using iterative algorithim

        // Game actions
        bool revealCell(int x, int y);
        // Gen AI, If clicked on an empty space, flood fills all adjacent empty spaces
        void revealArea(int x, int y);  // Flood fill for empty cells
        void toggleFlag(int x, int y);  // Toggles flag and pos
        void revealAllMines();

        // Queries
        // Gen AI, Gets a constant pointer to cell coords
        Cell* getCell(int x, int y);
        const Cell* getCell(int x, int y) const;

        // AI Gen, Checks if coordinates are within playable area
        bool isValidPosition(int x, int y) const;
        bool isWinCondition() const;        // Checks for remaining mine count
        int getAdjacentMineCount(int x, int y) const;   // Checks for adjacent mines for the respective cell

        // Getters
        int getWidth() const {return width;}
        int getHeight() const {return height;}
        int getTotalMines() const {return totalMines;}
        int getFlaggedMines() const {return flaggedMines;}
        int getRemainingMines() const {return totalMines - flaggedMines;}
        int getRevealedCells() const {return revealedCells;}
        int getTotalCells() const {return width * height;}

        // Iterators helpers for rendering
        void forEachCell(std::function<void(Cell&)> func);
        void forEachCellConst(std::function<void(const Cell&)> func) const;

    private:
        void updateNeighbors(int x, int y, int dx, int dy, int& count) const;
        void resetCellFlags();
    };
}

#endif //PA9_BOARD_H
