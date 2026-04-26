#ifndef PA9_CELL_H
#define PA9_CELL_H

#pragma once

namespace Minesweeper {
    class Cell {
    private:
        bool isMine;
        bool isRevealed;
        bool isFlagged;
        int adjacentMines;
    public:
        Cell();
        ~Cell();

        // Getters
        bool getIsMine() const {return isMine;}
        bool getIsRevealed() const {return isRevealed;}
        bool getIsFlagged() const {return isFlagged;}
        int getAdjacentMines() const {return adjacentMines;}

        // Setters
        void setIsMine(bool value) {isMine = value;}
        void setAdjacentMines(int value) {adjacentMines = value;}

        bool reveal();  // If returned true, player stepped on mine
        void toggleFlag();
        void reset();


    };
};

#endif //PA9_CELL_H
