#include "Cell.h"
#include <vector>
#include <functional>

#ifndef PA9_BOARD_H
#define PA9_BOARD_H

#pragma once

namespace Minesweeper {
    class Board {
    private:
        std::vector<std::vector<Cell>> cells;
        int width;
        int height;
        int totalMines;
        int flaggedMines;
        int revealedCells;
        bool minesPlaced;

        // Neighbor directions (including diagonals)
        const std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1},  {1, 0},  {1, 1}
        };

    public:
        Board();
        explicit Board(int w, int h, int mines);
        ~Board() = default;

        // Initialization
        void initialize(int w, int h, int mines);
        void reset();
        void placeMines(int safeX, int safeY);
        void calculateNumbers();

        // Game actions
        bool revealCell(int x, int y);
        void revealArea(int x, int y);  // Flood fill for empty cells
        void toggleFlag(int x, int y);
        void revealAllMines();

        // Queries
        Cell* getCell(int x, int y);
        const Cell* getCell(int x, int y) const;
        bool isValidPosition(int x, int y) const;
        bool isWinCondition() const;
        int getAdjacentMineCount(int x, int y) const;

        // Getters
        int getWidth() const {return width;}
        int getHeight() const {return height;}
        int getTotalMines() const {return totalMines;}
        int getFlaggedMines() const {return flaggedMines;}
        int getRemainingMines() const {return totalMines - flaggedMines;}
        int getRevealedCells() const {return revealedCells;}
        int getTotalCells() const {return width * height;}

        // Iterators for rendering
        void forEachCell(std::function<void(Cell&)> func);
        void forEachCellConst(std::function<void(const Cell&)> func) const;

    private:
        void updateNeighbors(int x, int y, int dx, int dy, int& count) const;
        void resetCellFlags();
    };
}

#endif //PA9_BOARD_H
