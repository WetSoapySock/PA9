#ifndef PA9_CONSTANTS_H
#define PA9_CONSTANTS_H

#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>

/** Create a Constants.hpp file for Minesweeper in C++ with SFML containing:
    A border size of 1 cell for safety
    Maximum board size if 16x30 for Hard difficulty
    Minimum window dimensions of 800x600
    A Difficulty structure with EASY, MEDIUM, HARD, CUSTOM presets
    Color namespace similar to Google Minesweeper colors (background, checkerboard tiles, UI text) **/

namespace Minesweeper {
    // Size of each cell
    constexpr int CELL_SIZE = 32;

    // Cell boarder for safety
    constexpr int BORDER_SIZE = 1;

    // Maximum board size
    constexpr int MAX_BOARD_SIZE = 30;

    // AI Gen, Minimum window width  and height to fit largest board
    constexpr int MIN_WINDOW_WIDTH = 800;
    constexpr int MIN_WINDOW_HEIGHT = 600;

    // AI Gen, Window title displayed
    const std::string WINDOW_TITLE = "Minesweeper";

    // Cell states
    enum class CellState {
        COVERED = 0,    // Cell not yet revealed
        REVEALED = 1,   // Cell has been clicked
        FLAGGED = 2,    // Cell marked as flag
        };

    // Cell types
    enum class CellType {
        EMPTY = 0,      // Cell with 0 adjacent mines, also has "flood fill" so any "empty" spaces adjacent are all revealed
        MINE = 9,       // Cell has a mine
        NUMBER_1 = 1,   // Cell shows "1" - one adjacent mine
        NUMBER_2 = 2,   // ...
        NUMBER_3 = 3,   // ...
        NUMBER_4 = 4,   // ...
        NUMBER_5 = 5,   // ...
        NUMBER_6 = 6,   // ...
        NUMBER_7 = 7,   // ...
        NUMBER_8 = 8    // Cell shows "8" - eight adjacent mines
    };


    struct Difficulty {
        std::string name;    // Name of difficulty
        int width;           // Cell width
        int height;          // Cell height
        int mineCount;       // Mines to place based on difficulty

        /** Easy:  9x9 board with 10 mines
            Medium: 16x16 board with 40 mines
            Hard:   16x30 board with 99 mines
            Custom: Placeholder - values to be set at runtime */
        static const Difficulty EASY;
        static const Difficulty MEDIUM;
        static const Difficulty HARD;
        static const Difficulty CUSTOM;
    };

    /** AI Gen, Color scheme for Google Minesweeper style, can change to personalize later on
        "Create color constants for Google Minesweeper style,
        including background, covered cells (checkerboard pattern),
        and mine text colors" **/

    namespace Colors {
        // Background colors
        const sf::Color BACKGROUND(50, 50, 50);        // Window background

        // Covered cell colors with checkerboard pattern
        const sf::Color COVERED_LIGHT(82, 170, 255);  // Light tiles
        const sf::Color COVERED_DARK(40, 130, 220);   // Dark tiles

        // Revealed cell colors
        const sf::Color REVEALED(190, 190, 190);       // White revealed cell

        // UI Text colors
        const sf::Color TEXT_PRIMARY(255, 255, 255);   // White text
        const sf::Color TEXT_SECONDARY(200, 200, 200); // Gray secondary text

        // Status message colors
        const sf::Color SUCCESS(76, 175, 80);          ///< Green for win message
        const sf::Color ERROR(244, 67, 54);            ///< Red for game over
        const sf::Color WARNING(255, 152, 0);          ///< Orange for warnings

        const sf::Color NUMBER_1(25, 118, 210);
        const sf::Color NUMBER_2(56, 142, 60);
        const sf::Color NUMBER_3(211, 47, 47);
        const sf::Color NUMBER_4(123, 31, 162);
        const sf::Color NUMBER_5(255, 143, 0);
        const sf::Color NUMBER_6(0, 150, 136);
        const sf::Color NUMBER_7(50, 50, 50);
        const sf::Color NUMBER_8(100, 100, 100);
    }

}

#endif //PA9_CONSTANTS_H
