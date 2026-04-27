#ifndef PA9_CELL_H
#define PA9_CELL_H

#pragma once

#pragma once
#include "../utils/Constants.h"

namespace Minesweeper {
    class Cell {
    private:
        CellType type;
        CellState state;
        int x, y;  // Position in grid

    public:
        Cell();
        explicit Cell(int posX, int posY);
        ~Cell() = default;

        // Getters
        CellType getType() const {return type;}
        CellState getState() const {return state;}
        int getX() const {return x;}
        int getY() const {return y;}
        int getNumber() const;
        bool isMine() const {return type == CellType::MINE;}
        bool isRevealed() const {return state == CellState::REVEALED;}
        bool isCovered() const {return state == CellState::COVERED;}
        bool isFlagged() const {return state == CellState::FLAGGED;}

        // Setters
        void setType(CellType newType) {type = newType;}
        void setState(CellState newState) {state = newState;}
        void setNumber(int number);

        // Actions
        void reveal() {if (!isFlagged()) state = CellState::REVEALED;}
        void toggleFlag() {
            if (isCovered()) state = CellState::FLAGGED;
            else if (isFlagged()) state = CellState::COVERED;
        }
        void reset();

        // Utility
        bool operator==(const Cell& other) const;
    };
};

#endif //PA9_CELL_H
