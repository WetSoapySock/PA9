#ifndef PA9_CONSTANTS_H
#define PA9_CONSTANTS_H

#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>

namespace Minesweeper {
    // Game Constants
    constexpr int CELL_SIZE = 32;
    constexpr int BORDER_SIZE = 1;
    constexpr int MAX_BOARD_SIZE = 30;

    // Window Constants
    constexpr int MIN_WINDOW_WIDTH = 800;
    constexpr int MIN_WINDOW_HEIGHT = 600;
    const std::string WINDOW_TITLE = "Minesweeper";

    // Cell States
    enum class CellState {
        COVERED = 0,
        REVEALED = 1,
        FLAGGED = 2,
        QUESTION = 3  // Optional: for question mark mode
    };

    // Cell Types
    enum class CellType {
        EMPTY = 0,
        MINE = 9,
        NUMBER_1 = 1,
        NUMBER_2 = 2,
        NUMBER_3 = 3,
        NUMBER_4 = 4,
        NUMBER_5 = 5,
        NUMBER_6 = 6,
        NUMBER_7 = 7,
        NUMBER_8 = 8
    };

    // Difficulty Settings
    struct Difficulty {
        std::string name;
        int width;
        int height;
        int mineCount;

        static const Difficulty EASY;
        static const Difficulty MEDIUM;
        static const Difficulty HARD;
        static const Difficulty CUSTOM;
    };
    
    namespace Colors {
        const sf::Color BACKGROUND(50, 50, 50);
        const sf::Color COVERED_LIGHT(200, 200, 200);
        const sf::Color COVERED_DARK(170, 170, 170);
        const sf::Color REVEALED(220, 220, 220);
        const sf::Color BORDER(100, 100, 100);
        const sf::Color TEXT_PRIMARY(255, 255, 255);
        const sf::Color TEXT_SECONDARY(200, 200, 200);
        const sf::Color SUCCESS(76, 175, 80);
        const sf::Color ERROR(244, 67, 54);
        const sf::Color WARNING(255, 152, 0);
    }
}

#endif //PA9_CONSTANTS_H
