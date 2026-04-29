#ifndef PA9_CELL_H
#define PA9_CELL_H

#pragma once

#include "../utils/constants.h"

/** Gen AI, "We need help creating a Cell class for our Minesweeper game that can
    represent the state of each cell on the board. We need:
    A type to indicate if it's a mine, a number, or empty
    A state to indicate if it's covered, revealed, or flagged
    A method to check if the cell is a mine or not" **/

namespace Minesweeper {
    class Cell {    // Class for a single cell on the board
    private:
        CellType type;      // Mine, 1-8 for adjacent mines, and 0 for empty space
        CellState state;    // Visual states COVERED, REVEALED, and FLAGGED
        int x, y;           // Coords for position tracking
    public:
        Cell();
        explicit Cell(int posX, int posY);
        ~Cell() = default;

        // Getters
        CellType getType() const {return type;}      // Returns cell type
        CellState getState() const {return state;}   // Returns visual state
        int getX() const {return x;}                 // Returns column pos
        int getY() const {return y;}                 // Returns row pos
        int getNumber() const;                       // Converts type to an int (0-9)
        bool isMine() const {return type == CellType::MINE;}  // True if cell is a mine
        bool isRevealed() const {return state == CellState::REVEALED;}  // True if clicked
        bool isCovered() const {return state == CellState::COVERED;}    // True if hidden
        bool isFlagged() const {return state == CellState::FLAGGED;}    // True if flagged

        // Setters
        void setType(CellType newType) {type = newType;}        // Sets the mine or number
        void setState(CellState newState) {state = newState;}   // Sets the visual state
        void setNumber(int number);                             // Sets type from int


        void reveal() {if (!isFlagged()) state = CellState::REVEALED;}  // Shows cell unless flagged
        void toggleFlag() {                // Gen AI, Toggle cycle for COVERED->FLAGGED->COVERED
            if (isCovered()) state = CellState::FLAGGED;
            else if (isFlagged()) state = CellState::COVERED;
        }
        void reset();                      // Resets to COVERED or EMPTY for new game

        bool operator==(const Cell& other) const;  // Gen AI, Compares two cells for equality
    };
}

#endif //PA9_CELL_H
