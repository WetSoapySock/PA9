#ifndef PA9_GAME_H
#define PA9_GAME_H

#pragma once

#include "gameState.h"
#include "../graphics/renderer.h"
#include "../input/mouseHandler.h"
#include "../utils/constants.h"
#include "../network/networkClient.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>
#include <string>

/** Gen AI, Help create a class that orchestrates a game loop,
    input, rendering, and network features */

namespace Minesweeper {
    class Game {
    private:
        // Core game components
        // Gen AI, Create pointers for automatic memory management
        std::unique_ptr<GameState> gameState;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<MouseHandler> mouseHandler;
        std::unique_ptr<NetworkClient> networkClient;

        float timerAccumulator;

        // Game state flags
        bool running;                    // Game loop
        bool showLeaderboard;            // Shows leaderboard
        std::vector<LeaderboardEntry> currentLeaderboard; // Cached leaderboard data


    public:
        Game();
        ~Game() = default;

        // Gen AI, sets up SFML window and handlers
        bool initialize(int windowWidth, int windowHeight);
        void run();
        void shutdown();  // Frees resources after shutdown

        // Gen AI, Polls events and dispatches callbacks
        void processInput();
        void update(float deltaTime);    // Updates timer and game state
        void render();                   // Draws board and UI

        // Gen AI, SFML event callbacks
        void onMouseClick(const sf::Event& event);   // Handles left/right clicks
        void onKeyPress(const sf::Event& event);     // Gen AI: Handles keyboard shortcuts
        void onWindowResize(const sf::Event& event); // Adjusts viewport

        // Game actions
        void startNewGame(const Difficulty& difficulty);    // Resets board and timer
        void submitScoreToLeaderboard();    // Sends time score to server
        void showLeaderboardUI();           // Displays top 10 scores
        void showMenu();                    // Returns to main menu

        // Handles difficulty changes
        void setDifficulty(const Difficulty& difficulty);  // Changes game difficulty
        std::string getDifficultyName() const;             // Returns game difficulty Easy/Medium/Hard

    private:
        // Gen AI, Converts pixels to grid coordinates
        sf::Vector2i screenToBoardPosition(const sf::Vector2i& screenPos) const;
        void renderLeaderboard(); // Render leaderboard overlay
        void showNameInputDialog(); // Name prompt
    };
}

#endif // PA9_GAME_H
